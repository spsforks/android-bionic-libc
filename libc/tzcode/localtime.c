/*
** This file is in the public domain, so clarified as of
** 1996-06-05 by Arthur David Olson.
*/

/*
** Leap second handling from Bradley White.
** POSIX-style TZ environment variable handling from Guy Harris.
*/

/*LINTLIBRARY*/

#define LOCALTIME_IMPLEMENTATION
#include "private.h"

#include "tzfile.h"
#include "fcntl.h"

#if THREAD_SAFE
# include <pthread.h>
static pthread_mutex_t locallock = PTHREAD_MUTEX_INITIALIZER;
static int lock(void) { return pthread_mutex_lock(&locallock); }
static void unlock(void) { pthread_mutex_unlock(&locallock); }
#else
static int lock(void) { return 0; }
static void unlock(void) { }
#endif

/* NETBSD_INSPIRED_EXTERN functions are exported to callers if
   NETBSD_INSPIRED is defined, and are private otherwise.  */
#if NETBSD_INSPIRED
# define NETBSD_INSPIRED_EXTERN
#else
# define NETBSD_INSPIRED_EXTERN static
#endif

#ifndef TZ_ABBR_MAX_LEN
#define TZ_ABBR_MAX_LEN 16
#endif /* !defined TZ_ABBR_MAX_LEN */

#ifndef TZ_ABBR_CHAR_SET
#define TZ_ABBR_CHAR_SET \
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 :+-._"
#endif /* !defined TZ_ABBR_CHAR_SET */

#ifndef TZ_ABBR_ERR_CHAR
#define TZ_ABBR_ERR_CHAR    '_'
#endif /* !defined TZ_ABBR_ERR_CHAR */

/*
** SunOS 4.1.1 headers lack O_BINARY.
*/

#ifdef O_BINARY
#define OPEN_MODE   (O_RDONLY | O_BINARY)
#endif /* defined O_BINARY */
#ifndef O_BINARY
#define OPEN_MODE   O_RDONLY
#endif /* !defined O_BINARY */

#ifndef WILDABBR
/*
** Someone might make incorrect use of a time zone abbreviation:
**  1.  They might reference tzname[0] before calling tzset (explicitly
**      or implicitly).
**  2.  They might reference tzname[1] before calling tzset (explicitly
**      or implicitly).
**  3.  They might reference tzname[1] after setting to a time zone
**      in which Daylight Saving Time is never observed.
**  4.  They might reference tzname[0] after setting to a time zone
**      in which Standard Time is never observed.
**  5.  They might reference tm.TM_ZONE after calling offtime.
** What's best to do in the above cases is open to debate;
** for now, we just set things up so that in any of the five cases
** WILDABBR is used. Another possibility: initialize tzname[0] to the
** string "tzname[0] used before set", and similarly for the other cases.
** And another: initialize tzname[0] to "ERA", with an explanation in the
** manual page of what this "time zone abbreviation" means (doing this so
** that tzname[0] has the "normal" length of three characters).
*/
#define WILDABBR    "   "
#endif /* !defined WILDABBR */

static const char       wildabbr[] = WILDABBR;

static const char gmt[] = "GMT";

/*
** The DST rules to use if TZ has no rules and we can't load TZDEFRULES.
** We default to US rules as of 1999-08-17.
** POSIX 1003.1 section 8.1.1 says that the default DST rules are
** implementation dependent; for historical reasons, US rules are a
** common default.
*/
#ifndef TZDEFRULESTRING
#define TZDEFRULESTRING ",M4.1.0,M10.5.0"
#endif /* !defined TZDEFDST */

struct ttinfo {              /* time type information */
    int_fast32_t tt_gmtoff;  /* UT offset in seconds */
    bool         tt_isdst;   /* used to set tm_isdst */
    int          tt_abbrind; /* abbreviation list index */
    bool         tt_ttisstd; /* transition is std time */
    bool         tt_ttisgmt; /* transition is UT */
};

struct lsinfo {              /* leap second information */
    time_t       ls_trans;   /* transition time */
    int_fast64_t ls_corr;    /* correction to apply */
};

#define SMALLEST(a, b)	(((a) < (b)) ? (a) : (b))
#define BIGGEST(a, b)   (((a) > (b)) ? (a) : (b))

#ifdef TZNAME_MAX
#define MY_TZNAME_MAX   TZNAME_MAX
#endif /* defined TZNAME_MAX */
#ifndef TZNAME_MAX
#define MY_TZNAME_MAX   255
#endif /* !defined TZNAME_MAX */

struct state {
    int           leapcnt;
    int           timecnt;
    int           typecnt;
    int           charcnt;
    bool          goback;
    bool          goahead;
    time_t        ats[TZ_MAX_TIMES];
    unsigned char types[TZ_MAX_TIMES];
    struct ttinfo ttis[TZ_MAX_TYPES];
    char          chars[BIGGEST(BIGGEST(TZ_MAX_CHARS + 1, sizeof gmt),
                  (2 * (MY_TZNAME_MAX + 1)))];
    struct lsinfo lsis[TZ_MAX_LEAPS];
    int           defaulttype; /* for early times or if no transitions */
};

enum r_type {
  JULIAN_DAY,		/* Jn = Julian day */
  DAY_OF_YEAR,		/* n = day of year */
  MONTH_NTH_DAY_OF_WEEK	/* Mm.n.d = month, week, day of week */
};

struct rule {
	enum r_type	r_type;		/* type of rule */
    int          r_day;  /* day number of rule */
    int          r_week; /* week number of rule */
    int          r_mon;  /* month number of rule */
    int_fast32_t r_time; /* transition time of rule */
};

static struct tm *gmtsub(struct state const *, time_t const *, int_fast32_t,
			 struct tm *);
static bool increment_overflow(int *, int);
static bool increment_overflow_time(time_t *, int_fast32_t);
static bool normalize_overflow32(int_fast32_t *, int *, int);
static struct tm *timesub(time_t const *, int_fast32_t, struct state const *,
			  struct tm *);
static bool typesequiv(struct state const *, int, int);
static bool tzparse(char const *, struct state *, bool);

#ifdef ALL_STATE
static struct state * lclptr;
static struct state * gmtptr;
#endif /* defined ALL_STATE */

#ifndef ALL_STATE
static struct state lclmem;
static struct state gmtmem;
#define lclptr      (&lclmem)
#define gmtptr      (&gmtmem)
#endif /* State Farm */

#ifndef TZ_STRLEN_MAX
#define TZ_STRLEN_MAX 255
#endif /* !defined TZ_STRLEN_MAX */

static char lcl_TZname[TZ_STRLEN_MAX + 1];
static int  lcl_is_set;

char * tzname[2] = {
    (char *) wildabbr,
    (char *) wildabbr
};

/*
** Section 4.12.3 of X3.159-1989 requires that
**  Except for the strftime function, these functions [asctime,
**  ctime, gmtime, localtime] return values in one of two static
**  objects: a broken-down time structure and an array of char.
** Thanks to Paul Eggert for noting this.
*/

static struct tm	tm;

#ifdef USG_COMPAT
long			timezone;
int			daylight;
#endif /* defined USG_COMPAT */

#ifdef ALTZONE
long			altzone;
#endif /* defined ALTZONE */

/* Initialize *S to a value based on GMTOFF, ISDST, and ABBRIND.  */
static void
init_ttinfo(struct ttinfo *s, int_fast32_t gmtoff, bool isdst, int abbrind)
{
  s->tt_gmtoff = gmtoff;
  s->tt_isdst = isdst;
  s->tt_abbrind = abbrind;
  s->tt_ttisstd = false;
  s->tt_ttisgmt = false;
}

static int_fast32_t
detzcode(const char *const codep)
{
	register int_fast32_t	result;
	register int		i;
	int_fast32_t one = 1;
	int_fast32_t halfmaxval = one << (32 - 2);
	int_fast32_t maxval = halfmaxval - 1 + halfmaxval;
	int_fast32_t minval = -1 - maxval;

	result = codep[0] & 0x7f;
	for (i = 1; i < 4; ++i)
		result = (result << 8) | (codep[i] & 0xff);

	if (codep[0] & 0x80) {
	  /* Do two's-complement negation even on non-two's-complement machines.
	     If the result would be minval - 1, return minval.  */
	  result -= !TWOS_COMPLEMENT(int_fast32_t) && result != 0;
	  result += minval;
	}
	return result;
}

static int_fast64_t
detzcode64(const char *const codep)
{
	register uint_fast64_t result;
	register int	i;
	int_fast64_t one = 1;
	int_fast64_t halfmaxval = one << (64 - 2);
	int_fast64_t maxval = halfmaxval - 1 + halfmaxval;
	int_fast64_t minval = -TWOS_COMPLEMENT(int_fast64_t) - maxval;

	result = codep[0] & 0x7f;
	for (i = 1; i < 8; ++i)
		result = (result << 8) | (codep[i] & 0xff);

	if (codep[0] & 0x80) {
	  /* Do two's-complement negation even on non-two's-complement machines.
	     If the result would be minval - 1, return minval.  */
	  result -= !TWOS_COMPLEMENT(int_fast64_t) && result != 0;
	  result += minval;
	}
	return result;
}

static void
update_tzname_etc(struct state const *sp, struct ttinfo const *ttisp)
{
  tzname[ttisp->tt_isdst] = (char *) &sp->chars[ttisp->tt_abbrind];
#ifdef USG_COMPAT
  if (!ttisp->tt_isdst)
    timezone = - ttisp->tt_gmtoff;
#endif
#ifdef ALTZONE
  if (ttisp->tt_isdst)
    altzone = - ttisp->tt_gmtoff;
#endif
}

static void
settzname(void)
{
	register struct state * const	sp = lclptr;
	register int			i;

	tzname[0] = tzname[1] = (char *) wildabbr;
#ifdef USG_COMPAT
	daylight = 0;
	timezone = 0;
#endif /* defined USG_COMPAT */
#ifdef ALTZONE
	altzone = 0;
#endif /* defined ALTZONE */
	if (sp == NULL) {
		tzname[0] = tzname[1] = (char *) gmt;
		return;
	}
	/*
	** And to get the latest zone names into tzname. . .
	*/
	for (i = 0; i < sp->typecnt; ++i) {
		register const struct ttinfo * const	ttisp = &sp->ttis[i];
		update_tzname_etc(sp, ttisp);
	}
	for (i = 0; i < sp->timecnt; ++i) {
		register const struct ttinfo * const	ttisp =
							&sp->ttis[
								sp->types[i]];
		update_tzname_etc(sp, ttisp);
#ifdef USG_COMPAT
		if (ttisp->tt_isdst)
			daylight = 1;
#endif /* defined USG_COMPAT */
	}
}

static void
scrub_abbrs(struct state *sp)
{
	int i;
	/*
	** First, replace bogus characters.
	*/
	for (i = 0; i < sp->charcnt; ++i)
		if (strchr(TZ_ABBR_CHAR_SET, sp->chars[i]) == NULL)
			sp->chars[i] = TZ_ABBR_ERR_CHAR;
	/*
	** Second, truncate long abbreviations.
	*/
	for (i = 0; i < sp->typecnt; ++i) {
		register const struct ttinfo * const	ttisp = &sp->ttis[i];
		register char *				cp = &sp->chars[ttisp->tt_abbrind];

		if (strlen(cp) > TZ_ABBR_MAX_LEN &&
			strcmp(cp, GRANDPARENTED) != 0)
				*(cp + TZ_ABBR_MAX_LEN) = '\0';
	}
}

static bool
differ_by_repeat(const time_t t1, const time_t t0)
{
    if (TYPE_BIT(time_t) - TYPE_SIGNED(time_t) < SECSPERREPEAT_BITS)
        return 0;
#if defined(__LP64__) // 32-bit Android only has a signed 32-bit time_t; 64-bit Android is fixed.
    return t1 - t0 == SECSPERREPEAT;
#endif
}

/* Input buffer for data read from a compiled tz file.  */
union input_buffer {
  /* The first part of the buffer, interpreted as a header.  */
  struct tzhead tzhead;

  /* The entire buffer.  */
  char buf[2 * sizeof(struct tzhead) + 2 * sizeof (struct state)
	   + 4 * TZ_MAX_TIMES];
};

/* Local storage needed for 'tzloadbody'.  */
union local_storage {
  /* The file name to be opened.  */
  char fullname[FILENAME_MAX + 1];

  /* The results of analyzing the file's contents after it is opened.  */
  struct {
    /* The input buffer.  */
    union input_buffer u;

    /* A temporary state used for parsing a TZ string in the file.  */
    struct state st;
  } u;
};

static int __bionic_open_tzdata(const char*);

/* Load tz data from the file named NAME into *SP.  Read extended
   format if DOEXTEND.  Use *LSP for temporary storage.  Return 0 on
   success, an errno value on failure.  */
static int
tzloadbody(char const *name, struct state *sp, bool doextend,
	   union local_storage *lsp)
{
	register int			i;
	register int			fid;
	register int			stored;
	register ssize_t		nread;
#if !defined(__ANDROID__)
	register bool doaccess;
	register char *fullname = lsp->fullname;
#endif
	register union input_buffer *up = &lsp->u.u;
	register int tzheadsize = sizeof (struct tzhead);

	sp->goback = sp->goahead = false;

	if (! name) {
		name = TZDEFAULT;
		if (! name)
		  return EINVAL;
	}

#if defined(__ANDROID__)
	fid = __bionic_open_tzdata(name);
#else
	if (name[0] == ':')
		++name;
	doaccess = name[0] == '/';
	if (!doaccess) {
		char const *p = TZDIR;
		if (! p)
		  return EINVAL;
		if (sizeof lsp->fullname - 1 <= strlen(p) + strlen(name))
		  return ENAMETOOLONG;
		strcpy(fullname, p);
		strcat(fullname, "/");
		strcat(fullname, name);
		/* Set doaccess if '.' (as in "../") shows up in name.  */
		if (strchr(name, '.'))
			doaccess = true;
		name = fullname;
	}
	if (doaccess && access(name, R_OK) != 0)
	  return errno;
	fid = open(name, OPEN_MODE);
#endif
	if (fid < 0)
	  return errno;

	nread = read(fid, up->buf, sizeof up->buf);
	if (nread < tzheadsize) {
	  int err = nread < 0 ? errno : EINVAL;
	  close(fid);
	  return err;
	}
	if (close(fid) < 0)
	  return errno;
	for (stored = 4; stored <= 8; stored *= 2) {
		int_fast32_t ttisstdcnt = detzcode(up->tzhead.tzh_ttisstdcnt);
		int_fast32_t ttisgmtcnt = detzcode(up->tzhead.tzh_ttisgmtcnt);
		int_fast32_t leapcnt = detzcode(up->tzhead.tzh_leapcnt);
		int_fast32_t timecnt = detzcode(up->tzhead.tzh_timecnt);
		int_fast32_t typecnt = detzcode(up->tzhead.tzh_typecnt);
		int_fast32_t charcnt = detzcode(up->tzhead.tzh_charcnt);
		char const *p = up->buf + tzheadsize;
		if (! (0 <= leapcnt && leapcnt < TZ_MAX_LEAPS
		       && 0 < typecnt && typecnt < TZ_MAX_TYPES
		       && 0 <= timecnt && timecnt < TZ_MAX_TIMES
		       && 0 <= charcnt && charcnt < TZ_MAX_CHARS
		       && (ttisstdcnt == typecnt || ttisstdcnt == 0)
		       && (ttisgmtcnt == typecnt || ttisgmtcnt == 0)))
		  return EINVAL;
		if (nread
		    < (tzheadsize		/* struct tzhead */
		       + timecnt * stored	/* ats */
		       + timecnt		/* types */
		       + typecnt * 6		/* ttinfos */
		       + charcnt		/* chars */
		       + leapcnt * (stored + 4)	/* lsinfos */
		       + ttisstdcnt		/* ttisstds */
		       + ttisgmtcnt))		/* ttisgmts */
		  return EINVAL;
		sp->leapcnt = leapcnt;
		sp->timecnt = timecnt;
		sp->typecnt = typecnt;
		sp->charcnt = charcnt;

		/* Read transitions, discarding those out of time_t range.
		   But pretend the last transition before time_t_min
		   occurred at time_t_min.  */
		timecnt = 0;
		for (i = 0; i < sp->timecnt; ++i) {
			int_fast64_t at
			  = stored == 4 ? detzcode(p) : detzcode64(p);
			sp->types[i] = at <= time_t_max;
			if (sp->types[i]) {
			  time_t attime
			    = ((TYPE_SIGNED(time_t) ? at < time_t_min : at < 0)
			       ? time_t_min : at);
			  if (timecnt && attime <= sp->ats[timecnt - 1]) {
			    if (attime < sp->ats[timecnt - 1])
			      return EINVAL;
			    sp->types[i - 1] = 0;
			    timecnt--;
			  }
			  sp->ats[timecnt++] = attime;
			}
			p += stored;
		}

		timecnt = 0;
		for (i = 0; i < sp->timecnt; ++i) {
			unsigned char typ = *p++;
			if (sp->typecnt <= typ)
			  return EINVAL;
			if (sp->types[i])
				sp->types[timecnt++] = typ;
		}
		sp->timecnt = timecnt;
		for (i = 0; i < sp->typecnt; ++i) {
			register struct ttinfo *	ttisp;
			unsigned char isdst, abbrind;

			ttisp = &sp->ttis[i];
			ttisp->tt_gmtoff = detzcode(p);
			p += 4;
			isdst = *p++;
			if (! (isdst < 2))
			  return EINVAL;
			ttisp->tt_isdst = isdst;
			abbrind = *p++;
			if (! (abbrind < sp->charcnt))
			  return EINVAL;
			ttisp->tt_abbrind = abbrind;
		}
		for (i = 0; i < sp->charcnt; ++i)
			sp->chars[i] = *p++;
		sp->chars[i] = '\0';	/* ensure '\0' at end */

		/* Read leap seconds, discarding those out of time_t range.  */
		leapcnt = 0;
		for (i = 0; i < sp->leapcnt; ++i) {
		  int_fast64_t tr = stored == 4 ? detzcode(p) : detzcode64(p);
		  int_fast32_t corr = detzcode(p + stored);
		  p += stored + 4;
		  if (tr <= time_t_max) {
		    time_t trans
		      = ((TYPE_SIGNED(time_t) ? tr < time_t_min : tr < 0)
			 ? time_t_min : tr);
		    if (leapcnt && trans <= sp->lsis[leapcnt - 1].ls_trans) {
		      if (trans < sp->lsis[leapcnt - 1].ls_trans)
			return EINVAL;
		      leapcnt--;
		    }
		    sp->lsis[leapcnt].ls_trans = trans;
		    sp->lsis[leapcnt].ls_corr = corr;
		    leapcnt++;
		  }
		}
		sp->leapcnt = leapcnt;

		for (i = 0; i < sp->typecnt; ++i) {
			register struct ttinfo *	ttisp;

			ttisp = &sp->ttis[i];
			if (ttisstdcnt == 0)
				ttisp->tt_ttisstd = false;
			else {
				if (*p != true && *p != false)
				  return EINVAL;
				ttisp->tt_ttisstd = *p++;
			}
		}
		for (i = 0; i < sp->typecnt; ++i) {
			register struct ttinfo *	ttisp;

			ttisp = &sp->ttis[i];
			if (ttisgmtcnt == 0)
				ttisp->tt_ttisgmt = false;
			else {
				if (*p != true && *p != false)
						return EINVAL;
				ttisp->tt_ttisgmt = *p++;
			}
		}
		/*
		** If this is an old file, we're done.
		*/
		if (up->tzhead.tzh_version[0] == '\0')
			break;
		nread -= p - up->buf;
		memmove(up->buf, p, nread);
	}
	if (doextend && nread > 2 &&
		up->buf[0] == '\n' && up->buf[nread - 1] == '\n' &&
		sp->typecnt + 2 <= TZ_MAX_TYPES) {
			struct state	*ts = &lsp->u.st;

			up->buf[nread - 1] = '\0';
			if (tzparse(&up->buf[1], ts, false)
			    && ts->typecnt == 2) {

			  /* Attempt to reuse existing abbreviations.
			     Without this, America/Anchorage would stop
			     working after 2037 when TZ_MAX_CHARS is 50, as
			     sp->charcnt equals 42 (for LMT CAT CAWT CAPT AHST
			     AHDT YST AKDT AKST) and ts->charcnt equals 10
			     (for AKST AKDT).  Reusing means sp->charcnt can
			     stay 42 in this example.  */
			  int gotabbr = 0;
			  int charcnt = sp->charcnt;
			  for (i = 0; i < 2; i++) {
			    char *tsabbr = ts->chars + ts->ttis[i].tt_abbrind;
			    int j;
			    for (j = 0; j < charcnt; j++)
			      if (strcmp(sp->chars + j, tsabbr) == 0) {
				ts->ttis[i].tt_abbrind = j;
				gotabbr++;
				break;
			      }
			    if (! (j < charcnt)) {
			      int tsabbrlen = strlen(tsabbr);
			      if (j + tsabbrlen < TZ_MAX_CHARS) {
				strcpy(sp->chars + j, tsabbr);
				charcnt = j + tsabbrlen + 1;
				ts->ttis[i].tt_abbrind = j;
				gotabbr++;
			      }
			    }
			  }
			  if (gotabbr == 2) {
			    sp->charcnt = charcnt;
			    for (i = 0; i < ts->timecnt; i++)
			      if (sp->ats[sp->timecnt - 1] < ts->ats[i])
				break;
			    while (i < ts->timecnt
				   && sp->timecnt < TZ_MAX_TIMES) {
			      sp->ats[sp->timecnt] = ts->ats[i];
			      sp->types[sp->timecnt] = (sp->typecnt
							+ ts->types[i]);
			      sp->timecnt++;
			      i++;
			    }
			    sp->ttis[sp->typecnt++] = ts->ttis[0];
			    sp->ttis[sp->typecnt++] = ts->ttis[1];
			  }
			}
	}
	if (sp->timecnt > 1) {
		for (i = 1; i < sp->timecnt; ++i)
			if (typesequiv(sp, sp->types[i], sp->types[0]) &&
				differ_by_repeat(sp->ats[i], sp->ats[0])) {
					sp->goback = true;
					break;
				}
		for (i = sp->timecnt - 2; i >= 0; --i)
			if (typesequiv(sp, sp->types[sp->timecnt - 1],
				sp->types[i]) &&
				differ_by_repeat(sp->ats[sp->timecnt - 1],
				sp->ats[i])) {
					sp->goahead = true;
					break;
		}
	}
	/*
	** If type 0 is is unused in transitions,
	** it's the type to use for early times.
	*/
	for (i = 0; i < sp->timecnt; ++i)
		if (sp->types[i] == 0)
			break;
	i = i < sp->timecnt ? -1 : 0;
	/*
	** Absent the above,
	** if there are transition times
	** and the first transition is to a daylight time
	** find the standard type less than and closest to
	** the type of the first transition.
	*/
	if (i < 0 && sp->timecnt > 0 && sp->ttis[sp->types[0]].tt_isdst) {
		i = sp->types[0];
		while (--i >= 0)
			if (!sp->ttis[i].tt_isdst)
				break;
	}
	/*
	** If no result yet, find the first standard type.
	** If there is none, punt to type zero.
	*/
	if (i < 0) {
		i = 0;
		while (sp->ttis[i].tt_isdst)
			if (++i >= sp->typecnt) {
				i = 0;
				break;
			}
	}
	sp->defaulttype = i;
	return 0;
}

/* Load tz data from the file named NAME into *SP.  Read extended
   format if DOEXTEND.  Return 0 on success, an errno value on failure.  */
static int
tzload(char const *name, struct state *sp, bool doextend)
{
#ifdef ALL_STATE
  union local_storage *lsp = malloc(sizeof *lsp);
  if (!lsp)
    return errno;
  else {
    int err = tzloadbody(name, sp, doextend, lsp);
    free(lsp);
    return err;
  }
#else
  union local_storage ls;
  return tzloadbody(name, sp, doextend, &ls);
#endif
}

static bool
typesequiv(const struct state *sp, int a, int b)
{
	register bool result;

	if (sp == NULL ||
		a < 0 || a >= sp->typecnt ||
		b < 0 || b >= sp->typecnt)
			result = false;
	else {
		register const struct ttinfo *	ap = &sp->ttis[a];
		register const struct ttinfo *	bp = &sp->ttis[b];
		result = ap->tt_gmtoff == bp->tt_gmtoff &&
			ap->tt_isdst == bp->tt_isdst &&
			ap->tt_ttisstd == bp->tt_ttisstd &&
			ap->tt_ttisgmt == bp->tt_ttisgmt &&
			strcmp(&sp->chars[ap->tt_abbrind],
			&sp->chars[bp->tt_abbrind]) == 0;
	}
	return result;
}

static const int	mon_lengths[2][MONSPERYEAR] = {
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static const int	year_lengths[2] = {
	DAYSPERNYEAR, DAYSPERLYEAR
};

/*
** Given a pointer into a time zone string, scan until a character that is not
** a valid character in a zone name is found. Return a pointer to that
** character.
*/

static const char * ATTRIBUTE_PURE
getzname(register const char *strp)
{
	register char	c;

	while ((c = *strp) != '\0' && !is_digit(c) && c != ',' && c != '-' &&
		c != '+')
			++strp;
	return strp;
}

/*
** Given a pointer into an extended time zone string, scan until the ending
** delimiter of the zone name is located. Return a pointer to the delimiter.
**
** As with getzname above, the legal character set is actually quite
** restricted, with other characters producing undefined results.
** We don't do any checking here; checking is done later in common-case code.
*/

static const char * ATTRIBUTE_PURE
getqzname(register const char *strp, const int delim)
{
	register int	c;

	while ((c = *strp) != '\0' && c != delim)
		++strp;
	return strp;
}

/*
** Given a pointer into a time zone string, extract a number from that string.
** Check that the number is within a specified range; if it is not, return
** NULL.
** Otherwise, return a pointer to the first character not part of the number.
*/

static const char *
getnum(register const char *strp, int *const nump, const int min, const int max)
{
	register char	c;
	register int	num;

	if (strp == NULL || !is_digit(c = *strp))
		return NULL;
	num = 0;
	do {
		num = num * 10 + (c - '0');
		if (num > max)
			return NULL;	/* illegal value */
		c = *++strp;
	} while (is_digit(c));
	if (num < min)
		return NULL;		/* illegal value */
	*nump = num;
	return strp;
}

/*
** Given a pointer into a time zone string, extract a number of seconds,
** in hh[:mm[:ss]] form, from the string.
** If any error occurs, return NULL.
** Otherwise, return a pointer to the first character not part of the number
** of seconds.
*/

static const char *
getsecs(register const char *strp, int_fast32_t *const secsp)
{
	int	num;

	/*
	** 'HOURSPERDAY * DAYSPERWEEK - 1' allows quasi-Posix rules like
	** "M10.4.6/26", which does not conform to Posix,
	** but which specifies the equivalent of
	** "02:00 on the first Sunday on or after 23 Oct".
	*/
	strp = getnum(strp, &num, 0, HOURSPERDAY * DAYSPERWEEK - 1);
	if (strp == NULL)
		return NULL;
	*secsp = num * (int_fast32_t) SECSPERHOUR;
	if (*strp == ':') {
		++strp;
		strp = getnum(strp, &num, 0, MINSPERHOUR - 1);
		if (strp == NULL)
			return NULL;
		*secsp += num * SECSPERMIN;
		if (*strp == ':') {
			++strp;
			/* 'SECSPERMIN' allows for leap seconds.  */
			strp = getnum(strp, &num, 0, SECSPERMIN);
			if (strp == NULL)
				return NULL;
			*secsp += num;
		}
	}
	return strp;
}

/*
** Given a pointer into a time zone string, extract an offset, in
** [+-]hh[:mm[:ss]] form, from the string.
** If any error occurs, return NULL.
** Otherwise, return a pointer to the first character not part of the time.
*/

static const char *
getoffset(register const char *strp, int_fast32_t *const offsetp)
{
	register bool neg = false;

	if (*strp == '-') {
		neg = true;
		++strp;
	} else if (*strp == '+')
		++strp;
	strp = getsecs(strp, offsetp);
	if (strp == NULL)
		return NULL;		/* illegal time */
	if (neg)
		*offsetp = -*offsetp;
	return strp;
}

/*
** Given a pointer into a time zone string, extract a rule in the form
** date[/time]. See POSIX section 8 for the format of "date" and "time".
** If a valid rule is not found, return NULL.
** Otherwise, return a pointer to the first character not part of the rule.
*/

static const char *
getrule(const char *strp, register struct rule *const rulep)
{
	if (*strp == 'J') {
		/*
		** Julian day.
		*/
		rulep->r_type = JULIAN_DAY;
		++strp;
		strp = getnum(strp, &rulep->r_day, 1, DAYSPERNYEAR);
	} else if (*strp == 'M') {
		/*
		** Month, week, day.
		*/
		rulep->r_type = MONTH_NTH_DAY_OF_WEEK;
		++strp;
		strp = getnum(strp, &rulep->r_mon, 1, MONSPERYEAR);
		if (strp == NULL)
			return NULL;
		if (*strp++ != '.')
			return NULL;
		strp = getnum(strp, &rulep->r_week, 1, 5);
		if (strp == NULL)
			return NULL;
		if (*strp++ != '.')
			return NULL;
		strp = getnum(strp, &rulep->r_day, 0, DAYSPERWEEK - 1);
	} else if (is_digit(*strp)) {
		/*
		** Day of year.
		*/
		rulep->r_type = DAY_OF_YEAR;
		strp = getnum(strp, &rulep->r_day, 0, DAYSPERLYEAR - 1);
	} else	return NULL;		/* invalid format */
	if (strp == NULL)
		return NULL;
	if (*strp == '/') {
		/*
		** Time specified.
		*/
		++strp;
		strp = getoffset(strp, &rulep->r_time);
	} else	rulep->r_time = 2 * SECSPERHOUR;	/* default = 2:00:00 */
	return strp;
}

/*
** Given a year, a rule, and the offset from UT at the time that rule takes
** effect, calculate the year-relative time that rule takes effect.
*/

static int_fast32_t ATTRIBUTE_PURE
transtime(const int year, register const struct rule *const rulep,
          const int_fast32_t offset)
{
    register bool         leapyear;
    register int_fast32_t value;
    register int          i;
    int d, m1, yy0, yy1, yy2, dow;

    INITIALIZE(value);
    leapyear = isleap(year);
    switch (rulep->r_type) {

    case JULIAN_DAY:
        /*
        ** Jn - Julian day, 1 == January 1, 60 == March 1 even in leap
        ** years.
        ** In non-leap years, or if the day number is 59 or less, just
        ** add SECSPERDAY times the day number-1 to the time of
        ** January 1, midnight, to get the day.
        */
        value = (rulep->r_day - 1) * SECSPERDAY;
        if (leapyear && rulep->r_day >= 60)
            value += SECSPERDAY;
        break;

    case DAY_OF_YEAR:
        /*
        ** n - day of year.
        ** Just add SECSPERDAY times the day number to the time of
        ** January 1, midnight, to get the day.
        */
        value = rulep->r_day * SECSPERDAY;
        break;

    case MONTH_NTH_DAY_OF_WEEK:
        /*
        ** Mm.n.d - nth "dth day" of month m.
        */

        /*
        ** Use Zeller's Congruence to get day-of-week of first day of
        ** month.
        */
        m1 = (rulep->r_mon + 9) % 12 + 1;
        yy0 = (rulep->r_mon <= 2) ? (year - 1) : year;
        yy1 = yy0 / 100;
        yy2 = yy0 % 100;
        dow = ((26 * m1 - 2) / 10 +
            1 + yy2 + yy2 / 4 + yy1 / 4 - 2 * yy1) % 7;
        if (dow < 0)
            dow += DAYSPERWEEK;

        /*
        ** "dow" is the day-of-week of the first day of the month. Get
        ** the day-of-month (zero-origin) of the first "dow" day of the
        ** month.
        */
        d = rulep->r_day - dow;
        if (d < 0)
            d += DAYSPERWEEK;
        for (i = 1; i < rulep->r_week; ++i) {
            if (d + DAYSPERWEEK >=
                mon_lengths[leapyear][rulep->r_mon - 1])
                    break;
            d += DAYSPERWEEK;
        }

        /*
        ** "d" is the day-of-month (zero-origin) of the day we want.
        */
        value = d * SECSPERDAY;
        for (i = 0; i < rulep->r_mon - 1; ++i)
            value += mon_lengths[leapyear][i] * SECSPERDAY;
        break;
    }

    /*
    ** "value" is the year-relative time of 00:00:00 UT on the day in
    ** question. To get the year-relative time of the specified local
    ** time on that day, add the transition time and the current offset
    ** from UT.
    */
    return value + rulep->r_time + offset;
}

/*
** Given a POSIX section 8-style TZ string, fill in the rule tables as
** appropriate.
*/

static bool
tzparse(const char *name, struct state *sp, bool lastditch)
{
	const char *			stdname;
	const char *			dstname;
	size_t				stdlen;
	size_t				dstlen;
	size_t				charcnt;
	int_fast32_t			stdoffset;
	int_fast32_t			dstoffset;
	register char *			cp;
	register bool			load_ok;

	stdname = name;
	if (lastditch) {
		stdlen = sizeof gmt - 1;
		name += stdlen;
		stdoffset = 0;
	} else {
		if (*name == '<') {
			name++;
			stdname = name;
			name = getqzname(name, '>');
			if (*name != '>')
			  return false;
			stdlen = name - stdname;
			name++;
		} else {
			name = getzname(name);
			stdlen = name - stdname;
		}
		if (!stdlen)
		  return false;
		name = getoffset(name, &stdoffset);
		if (name == NULL)
		  return false;
	}
	charcnt = stdlen + 1;
	if (sizeof sp->chars < charcnt)
	  return false;
	load_ok = tzload(TZDEFRULES, sp, false) == 0;
	if (!load_ok)
		sp->leapcnt = 0;		/* so, we're off a little */
	if (*name != '\0') {
		if (*name == '<') {
			dstname = ++name;
			name = getqzname(name, '>');
			if (*name != '>')
			  return false;
			dstlen = name - dstname;
			name++;
		} else {
			dstname = name;
			name = getzname(name);
			dstlen = name - dstname; /* length of DST zone name */
		}
		if (!dstlen)
		  return false;
		charcnt += dstlen + 1;
		if (sizeof sp->chars < charcnt)
		  return false;
		if (*name != '\0' && *name != ',' && *name != ';') {
			name = getoffset(name, &dstoffset);
			if (name == NULL)
			  return false;
		} else	dstoffset = stdoffset - SECSPERHOUR;
		if (*name == '\0' && !load_ok)
			name = TZDEFRULESTRING;
		if (*name == ',' || *name == ';') {
			struct rule	start;
			struct rule	end;
			register int	year;
			register int	yearlim;
			register int	timecnt;
			time_t		janfirst;

			++name;
			if ((name = getrule(name, &start)) == NULL)
			  return false;
			if (*name++ != ',')
			  return false;
			if ((name = getrule(name, &end)) == NULL)
			  return false;
			if (*name != '\0')
			  return false;
			sp->typecnt = 2;	/* standard time and DST */
			/*
			** Two transitions per year, from EPOCH_YEAR forward.
			*/
			init_ttinfo(&sp->ttis[0], -dstoffset, true, stdlen + 1);
			init_ttinfo(&sp->ttis[1], -stdoffset, false, 0);
			sp->defaulttype = 0;
			timecnt = 0;
			janfirst = 0;
			yearlim = EPOCH_YEAR + YEARSPERREPEAT;
			for (year = EPOCH_YEAR; year < yearlim; year++) {
				int_fast32_t
				  starttime = transtime(year, &start, stdoffset),
				  endtime = transtime(year, &end, dstoffset);
				int_fast32_t
				  yearsecs = (year_lengths[isleap(year)]
					      * SECSPERDAY);
				bool reversed = endtime < starttime;
				if (reversed) {
					int_fast32_t swap = starttime;
					starttime = endtime;
					endtime = swap;
				}
				if (reversed
				    || (starttime < endtime
					&& (endtime - starttime
					    < (yearsecs
					       + (stdoffset - dstoffset))))) {
					if (TZ_MAX_TIMES - 2 < timecnt)
						break;
					yearlim = year + YEARSPERREPEAT + 1;
					sp->ats[timecnt] = janfirst;
					if (increment_overflow_time
					    (&sp->ats[timecnt], starttime))
						break;
					sp->types[timecnt++] = reversed;
					sp->ats[timecnt] = janfirst;
					if (increment_overflow_time
					    (&sp->ats[timecnt], endtime))
						break;
					sp->types[timecnt++] = !reversed;
				}
				if (increment_overflow_time(&janfirst, yearsecs))
					break;
			}
			sp->timecnt = timecnt;
			if (!timecnt)
				sp->typecnt = 1;	/* Perpetual DST.  */
		} else {
			register int_fast32_t	theirstdoffset;
			register int_fast32_t	theirdstoffset;
			register int_fast32_t	theiroffset;
			register bool		isdst;
			register int		i;
			register int		j;

			if (*name != '\0')
			  return false;
			/*
			** Initial values of theirstdoffset and theirdstoffset.
			*/
			theirstdoffset = 0;
			for (i = 0; i < sp->timecnt; ++i) {
				j = sp->types[i];
				if (!sp->ttis[j].tt_isdst) {
					theirstdoffset =
						-sp->ttis[j].tt_gmtoff;
					break;
				}
			}
			theirdstoffset = 0;
			for (i = 0; i < sp->timecnt; ++i) {
				j = sp->types[i];
				if (sp->ttis[j].tt_isdst) {
					theirdstoffset =
						-sp->ttis[j].tt_gmtoff;
					break;
				}
			}
			/*
			** Initially we're assumed to be in standard time.
			*/
			isdst = false;
			theiroffset = theirstdoffset;
			/*
			** Now juggle transition times and types
			** tracking offsets as you do.
			*/
			for (i = 0; i < sp->timecnt; ++i) {
				j = sp->types[i];
				sp->types[i] = sp->ttis[j].tt_isdst;
				if (sp->ttis[j].tt_ttisgmt) {
					/* No adjustment to transition time */
				} else {
					/*
					** If summer time is in effect, and the
					** transition time was not specified as
					** standard time, add the summer time
					** offset to the transition time;
					** otherwise, add the standard time
					** offset to the transition time.
					*/
					/*
					** Transitions from DST to DDST
					** will effectively disappear since
					** POSIX provides for only one DST
					** offset.
					*/
					if (isdst && !sp->ttis[j].tt_ttisstd) {
						sp->ats[i] += dstoffset -
							theirdstoffset;
					} else {
						sp->ats[i] += stdoffset -
							theirstdoffset;
					}
				}
				theiroffset = -sp->ttis[j].tt_gmtoff;
				if (sp->ttis[j].tt_isdst)
					theirdstoffset = theiroffset;
				else	theirstdoffset = theiroffset;
			}
			/*
			** Finally, fill in ttis.
			*/
			init_ttinfo(&sp->ttis[0], -stdoffset, false, 0);
			init_ttinfo(&sp->ttis[1], -dstoffset, true, stdlen + 1);
			sp->typecnt = 2;
			sp->defaulttype = 0;
		}
	} else {
		dstlen = 0;
		sp->typecnt = 1;		/* only standard time */
		sp->timecnt = 0;
		init_ttinfo(&sp->ttis[0], -stdoffset, false, 0);
		sp->defaulttype = 0;
	}
	sp->charcnt = charcnt;
	cp = sp->chars;
	memcpy(cp, stdname, stdlen);
	cp += stdlen;
	*cp++ = '\0';
	if (dstlen != 0) {
		memcpy(cp, dstname, dstlen);
		*(cp + dstlen) = '\0';
	}
	return true;
}

static void
gmtload(struct state *const sp)
{
	if (tzload(gmt, sp, true) != 0)
		tzparse(gmt, sp, true);
}

/* Initialize *SP to a value appropriate for the TZ setting NAME.
   Return 0 on success, an errno value on failure.  */
static int
zoneinit(struct state *sp, char const *name)
{
  if (name && ! name[0]) {
    /*
    ** User wants it fast rather than right.
    */
    sp->leapcnt = 0;		/* so, we're off a little */
    sp->timecnt = 0;
    sp->typecnt = 0;
    sp->charcnt = 0;
    sp->goback = sp->goahead = false;
    init_ttinfo(&sp->ttis[0], 0, false, 0);
    strcpy(sp->chars, gmt);
    sp->defaulttype = 0;
    return 0;
  } else {
    int err = tzload(name, sp, true);
    if (err != 0 && name && name[0] != ':' && tzparse(name, sp, false))
      err = 0;
    if (err == 0)
      scrub_abbrs(sp);
    return err;
  }
}

static void
tzsetlcl(char const *name)
{
  struct state *sp = lclptr;
  int lcl = name ? strlen(name) < sizeof lcl_TZname : -1;
  if (lcl < 0
      ? lcl_is_set < 0
      : 0 < lcl_is_set && strcmp(lcl_TZname, name) == 0)
    return;
#ifdef ALL_STATE
  if (! sp)
    lclptr = sp = malloc(sizeof *lclptr);
#endif /* defined ALL_STATE */
  if (sp) {
    if (zoneinit(sp, name) != 0)
      zoneinit(sp, "");
    if (0 < lcl)
      strcpy(lcl_TZname, name);
  }
  settzname();
  lcl_is_set = lcl;
}

#ifdef STD_INSPIRED
void
tzsetwall(void)
{
  if (lock() != 0)
    return;
  tzsetlcl(NULL);
  unlock();
}
#endif

#if defined(__ANDROID__)
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h> // For __system_property_serial.
#endif

static void
tzset_unlocked(void)
{
#if defined(__ANDROID__)
  const char * name = getenv("TZ");

  // Try the "persist.sys.timezone" system property.
  if (name == NULL) {
    static const prop_info *pi;

    if (!pi) {
      pi = __system_property_find("persist.sys.timezone");
    }
    if (pi) {
      static char buf[PROP_VALUE_MAX];
      static uint32_t s = -1;
      static bool ok = false;
      uint32_t serial = __system_property_serial(pi);
      if (serial != s) {
        ok = __system_property_read(pi, 0, buf) > 0;
        s = serial;
      }
      if (ok) {
        name = buf;
      }
    }
  }

  tzsetlcl(name);
#else
  tzsetlcl(getenv("TZ"));
#endif
}

void
tzset(void)
{
  if (lock() != 0)
    return;
  tzset_unlocked();
  unlock();
}

static void
gmtcheck(void)
{
  static bool gmt_is_set;
  if (lock() != 0)
    return;
  if (! gmt_is_set) {
#ifdef ALL_STATE
    gmtptr = malloc(sizeof *gmtptr);
#endif
    if (gmtptr)
      gmtload(gmtptr);
    gmt_is_set = true;
  }
  unlock();
}

#if NETBSD_INSPIRED

timezone_t
tzalloc(char const *name)
{
  timezone_t sp = malloc(sizeof *sp);
  if (sp) {
    int err = zoneinit(sp, name);
    if (err != 0) {
      free(sp);
      errno = err;
      return NULL;
    }
  }
  return sp;
}

void
tzfree(timezone_t sp)
{
  free(sp);
}

/*
** NetBSD 6.1.4 has ctime_rz, but omit it because POSIX says ctime and
** ctime_r are obsolescent and have potential security problems that
** ctime_rz would share.  Callers can instead use localtime_rz + strftime.
**
** NetBSD 6.1.4 has tzgetname, but omit it because it doesn't work
** in zones with three or more time zone abbreviations.
** Callers can instead use localtime_rz + strftime.
*/

#endif

/*
** The easy way to behave "as if no library function calls" localtime
** is to not call it, so we drop its guts into "localsub", which can be
** freely called. (And no, the PANS doesn't require the above behavior,
** but it *is* desirable.)
**
** If successful and SETNAME is nonzero,
** set the applicable parts of tzname, timezone and altzone;
** however, it's OK to omit this step if the time zone is POSIX-compatible,
** since in that case tzset should have already done this step correctly.
** SETNAME's type is intfast32_t for compatibility with gmtsub,
** but it is actually a boolean and its value should be 0 or 1.
*/

/*ARGSUSED*/
static struct tm *
localsub(struct state const *sp, time_t const *timep, int_fast32_t setname,
	 struct tm *const tmp)
{
	register const struct ttinfo *	ttisp;
	register int			i;
	register struct tm *		result;
	const time_t			t = *timep;

	if (sp == NULL) {
	  /* Don't bother to set tzname etc.; tzset has already done it.  */
	  return gmtsub(gmtptr, timep, 0, tmp);
	}
	if ((sp->goback && t < sp->ats[0]) ||
		(sp->goahead && t > sp->ats[sp->timecnt - 1])) {
			time_t			newt = t;
			register time_t		seconds;
			register time_t		years;

			if (t < sp->ats[0])
				seconds = sp->ats[0] - t;
			else	seconds = t - sp->ats[sp->timecnt - 1];
			--seconds;
			years = (seconds / SECSPERREPEAT + 1) * YEARSPERREPEAT;
			seconds = years * AVGSECSPERYEAR;
			if (t < sp->ats[0])
				newt += seconds;
			else	newt -= seconds;
			if (newt < sp->ats[0] ||
				newt > sp->ats[sp->timecnt - 1])
					return NULL;	/* "cannot happen" */
			result = localsub(sp, &newt, setname, tmp);
			if (result) {
				register int_fast64_t newy;

				newy = result->tm_year;
				if (t < sp->ats[0])
					newy -= years;
				else	newy += years;
				if (! (INT_MIN <= newy && newy <= INT_MAX))
					return NULL;
				result->tm_year = newy;
			}
			return result;
	}
	if (sp->timecnt == 0 || t < sp->ats[0]) {
		i = sp->defaulttype;
	} else {
		register int	lo = 1;
		register int	hi = sp->timecnt;

		while (lo < hi) {
			register int	mid = (lo + hi) >> 1;

			if (t < sp->ats[mid])
				hi = mid;
			else	lo = mid + 1;
		}
		i = (int) sp->types[lo - 1];
	}
	ttisp = &sp->ttis[i];
	/*
	** To get (wrong) behavior that's compatible with System V Release 2.0
	** you'd replace the statement below with
	**	t += ttisp->tt_gmtoff;
	**	timesub(&t, 0L, sp, tmp);
	*/
	result = timesub(&t, ttisp->tt_gmtoff, sp, tmp);
	if (result) {
	  result->tm_isdst = ttisp->tt_isdst;
#ifdef TM_ZONE
	  result->TM_ZONE = (char *) &sp->chars[ttisp->tt_abbrind];
#endif /* defined TM_ZONE */
	  if (setname)
	    update_tzname_etc(sp, ttisp);
	}
	return result;
}

#if NETBSD_INSPIRED

struct tm *
localtime_rz(struct state *sp, time_t const *timep, struct tm *tmp)
{
  return localsub(sp, timep, 0, tmp);
}

#endif

static struct tm *
localtime_tzset(time_t const *timep, struct tm *tmp, bool setname)
{
  int err = lock();
  if (err) {
    errno = err;
    return NULL;
  }
  if (setname || !lcl_is_set)
    tzset_unlocked();
  tmp = localsub(lclptr, timep, setname, tmp);
  unlock();
  return tmp;
}

struct tm *
localtime(const time_t *timep)
{
  return localtime_tzset(timep, &tm, true);
}

struct tm *
localtime_r(const time_t *timep, struct tm *tmp)
{
  return localtime_tzset(timep, tmp, false);
}

/*
** gmtsub is to gmtime as localsub is to localtime.
*/

static struct tm *
gmtsub(struct state const *sp, time_t const *timep, int_fast32_t offset,
       struct tm *tmp)
{
	register struct tm *	result;

	result = timesub(timep, offset, gmtptr, tmp);
#ifdef TM_ZONE
	/*
	** Could get fancy here and deliver something such as
	** "UT+xxxx" or "UT-xxxx" if offset is non-zero,
	** but this is no time for a treasure hunt.
	*/
	tmp->TM_ZONE = ((char *)
			(offset ? wildabbr : gmtptr ? gmtptr->chars : gmt));
#endif /* defined TM_ZONE */
	return result;
}

/*
* Re-entrant version of gmtime.
*/

struct tm *
gmtime_r(const time_t *timep, struct tm *tmp)
{
  gmtcheck();
  return gmtsub(gmtptr, timep, 0, tmp);
}

struct tm *
gmtime(const time_t *timep)
{
  return gmtime_r(timep, &tm);
}

#ifdef STD_INSPIRED

struct tm *
offtime(const time_t *timep, long offset)
{
  gmtcheck();
  return gmtsub(gmtptr, timep, offset, &tm);
}

#endif /* defined STD_INSPIRED */

/*
** Return the number of leap years through the end of the given year
** where, to make the math easy, the answer for year zero is defined as zero.
*/

static int ATTRIBUTE_PURE
leaps_thru_end_of(register const int y)
{
	return (y >= 0) ? (y / 4 - y / 100 + y / 400) :
		-(leaps_thru_end_of(-(y + 1)) + 1);
}

static struct tm *
timesub(const time_t *timep, int_fast32_t offset,
	const struct state *sp, struct tm *tmp)
{
	register const struct lsinfo *	lp;
	register time_t			tdays;
	register int			idays;	/* unsigned would be so 2003 */
	register int_fast64_t		rem;
	int				y;
	register const int *		ip;
	register int_fast64_t		corr;
	register bool			hit;
	register int			i;

	corr = 0;
	hit = false;
	i = (sp == NULL) ? 0 : sp->leapcnt;
	while (--i >= 0) {
		lp = &sp->lsis[i];
		if (*timep >= lp->ls_trans) {
			if (*timep == lp->ls_trans) {
				hit = ((i == 0 && lp->ls_corr > 0) ||
					lp->ls_corr > sp->lsis[i - 1].ls_corr);
				if (hit)
					while (i > 0 &&
						sp->lsis[i].ls_trans ==
						sp->lsis[i - 1].ls_trans + 1 &&
						sp->lsis[i].ls_corr ==
						sp->lsis[i - 1].ls_corr + 1) {
							++hit;
							--i;
					}
			}
			corr = lp->ls_corr;
			break;
		}
	}
	y = EPOCH_YEAR;
	tdays = *timep / SECSPERDAY;
	rem = *timep % SECSPERDAY;
	while (tdays < 0 || tdays >= year_lengths[isleap(y)]) {
		int		newy;
		register time_t	tdelta;
		register int	idelta;
		register int	leapdays;

		tdelta = tdays / DAYSPERLYEAR;
		if (! ((! TYPE_SIGNED(time_t) || INT_MIN <= tdelta)
		       && tdelta <= INT_MAX))
		  goto out_of_range;
		idelta = tdelta;
		if (idelta == 0)
			idelta = (tdays < 0) ? -1 : 1;
		newy = y;
		if (increment_overflow(&newy, idelta))
		  goto out_of_range;
		leapdays = leaps_thru_end_of(newy - 1) -
			leaps_thru_end_of(y - 1);
		tdays -= ((time_t) newy - y) * DAYSPERNYEAR;
		tdays -= leapdays;
		y = newy;
	}
	/*
	** Given the range, we can now fearlessly cast...
	*/
	idays = tdays;
	rem += offset - corr;
	while (rem < 0) {
		rem += SECSPERDAY;
		--idays;
	}
	while (rem >= SECSPERDAY) {
		rem -= SECSPERDAY;
		++idays;
	}
	while (idays < 0) {
		if (increment_overflow(&y, -1))
		  goto out_of_range;
		idays += year_lengths[isleap(y)];
	}
	while (idays >= year_lengths[isleap(y)]) {
		idays -= year_lengths[isleap(y)];
		if (increment_overflow(&y, 1))
		  goto out_of_range;
	}
	tmp->tm_year = y;
	if (increment_overflow(&tmp->tm_year, -TM_YEAR_BASE))
	  goto out_of_range;
	tmp->tm_yday = idays;
	/*
	** The "extra" mods below avoid overflow problems.
	*/
	tmp->tm_wday = EPOCH_WDAY +
		((y - EPOCH_YEAR) % DAYSPERWEEK) *
		(DAYSPERNYEAR % DAYSPERWEEK) +
		leaps_thru_end_of(y - 1) -
		leaps_thru_end_of(EPOCH_YEAR - 1) +
		idays;
	tmp->tm_wday %= DAYSPERWEEK;
	if (tmp->tm_wday < 0)
		tmp->tm_wday += DAYSPERWEEK;
	tmp->tm_hour = (int) (rem / SECSPERHOUR);
	rem %= SECSPERHOUR;
	tmp->tm_min = (int) (rem / SECSPERMIN);
	/*
	** A positive leap second requires a special
	** representation. This uses "... ??:59:60" et seq.
	*/
	tmp->tm_sec = (int) (rem % SECSPERMIN) + hit;
	ip = mon_lengths[isleap(y)];
	for (tmp->tm_mon = 0; idays >= ip[tmp->tm_mon]; ++(tmp->tm_mon))
		idays -= ip[tmp->tm_mon];
	tmp->tm_mday = (int) (idays + 1);
	tmp->tm_isdst = 0;
#ifdef TM_GMTOFF
	tmp->TM_GMTOFF = offset;
#endif /* defined TM_GMTOFF */
	return tmp;

 out_of_range:
	errno = EOVERFLOW;
	return NULL;
}

char *
ctime(const time_t *timep)
{
/*
** Section 4.12.3.2 of X3.159-1989 requires that
**	The ctime function converts the calendar time pointed to by timer
**	to local time in the form of a string. It is equivalent to
**		asctime(localtime(timer))
*/
  struct tm *tmp = localtime(timep);
  return tmp ? asctime(tmp) : NULL;
}

char *
ctime_r(const time_t *timep, char *buf)
{
  struct tm mytm;
  struct tm *tmp = localtime_r(timep, &mytm);
  return tmp ? asctime_r(tmp, buf) : NULL;
}

/*
** Adapted from code provided by Robert Elz, who writes:
**	The "best" way to do mktime I think is based on an idea of Bob
**	Kridle's (so its said...) from a long time ago.
**	It does a binary search of the time_t space. Since time_t's are
**	just 32 bits, its a max of 32 iterations (even at 64 bits it
**	would still be very reasonable).
*/

#ifndef WRONG
#define WRONG	(-1)
#endif /* !defined WRONG */

/*
** Normalize logic courtesy Paul Eggert.
*/

static bool
increment_overflow(int *ip, int j)
{
	register int const	i = *ip;

	/*
	** If i >= 0 there can only be overflow if i + j > INT_MAX
	** or if j > INT_MAX - i; given i >= 0, INT_MAX - i cannot overflow.
	** If i < 0 there can only be overflow if i + j < INT_MIN
	** or if j < INT_MIN - i; given i < 0, INT_MIN - i cannot overflow.
	*/
	if ((i >= 0) ? (j > INT_MAX - i) : (j < INT_MIN - i))
		return true;
	*ip += j;
	return false;
}

static bool
increment_overflow32(int_fast32_t *const lp, int const m)
{
	register int_fast32_t const	l = *lp;

	if ((l >= 0) ? (m > INT_FAST32_MAX - l) : (m < INT_FAST32_MIN - l))
		return true;
	*lp += m;
	return false;
}

static bool
increment_overflow_time(time_t *tp, int_fast32_t j)
{
	/*
	** This is like
	** 'if (! (time_t_min <= *tp + j && *tp + j <= time_t_max)) ...',
	** except that it does the right thing even if *tp + j would overflow.
	*/
	if (! (j < 0
	       ? (TYPE_SIGNED(time_t) ? time_t_min - j <= *tp : -1 - j < *tp)
	       : *tp <= time_t_max - j))
		return true;
	*tp += j;
	return false;
}

static bool
normalize_overflow(int *const tensptr, int *const unitsptr, const int base)
{
	register int	tensdelta;

	tensdelta = (*unitsptr >= 0) ?
		(*unitsptr / base) :
		(-1 - (-1 - *unitsptr) / base);
	*unitsptr -= tensdelta * base;
	return increment_overflow(tensptr, tensdelta);
}

static bool
normalize_overflow32(int_fast32_t *tensptr, int *unitsptr, int base)
{
	register int	tensdelta;

	tensdelta = (*unitsptr >= 0) ?
		(*unitsptr / base) :
		(-1 - (-1 - *unitsptr) / base);
	*unitsptr -= tensdelta * base;
	return increment_overflow32(tensptr, tensdelta);
}

static int
tmcomp(register const struct tm *const atmp,
       register const struct tm *const btmp)
{
	register int	result;

	if (atmp->tm_year != btmp->tm_year)
		return atmp->tm_year < btmp->tm_year ? -1 : 1;
	if ((result = (atmp->tm_mon - btmp->tm_mon)) == 0 &&
		(result = (atmp->tm_mday - btmp->tm_mday)) == 0 &&
		(result = (atmp->tm_hour - btmp->tm_hour)) == 0 &&
		(result = (atmp->tm_min - btmp->tm_min)) == 0)
			result = atmp->tm_sec - btmp->tm_sec;
	return result;
}

static time_t
time2sub(struct tm *const tmp,
	 struct tm *(*funcp)(struct state const *, time_t const *,
			     int_fast32_t, struct tm *),
	 struct state const *sp,
	 const int_fast32_t offset,
	 bool *okayp,
	 bool do_norm_secs)
{
	register int			dir;
	register int			i, j;
	register int			saved_seconds;
	register int_fast32_t		li;
	register time_t			lo;
	register time_t			hi;
	int_fast32_t			y;
	time_t				newt;
	time_t				t;
	struct tm			yourtm, mytm;

	*okayp = false;
	yourtm = *tmp;
	if (do_norm_secs) {
		if (normalize_overflow(&yourtm.tm_min, &yourtm.tm_sec,
			SECSPERMIN))
				return WRONG;
	}
	if (normalize_overflow(&yourtm.tm_hour, &yourtm.tm_min, MINSPERHOUR))
		return WRONG;
	if (normalize_overflow(&yourtm.tm_mday, &yourtm.tm_hour, HOURSPERDAY))
		return WRONG;
	y = yourtm.tm_year;
	if (normalize_overflow32(&y, &yourtm.tm_mon, MONSPERYEAR))
		return WRONG;
	/*
	** Turn y into an actual year number for now.
	** It is converted back to an offset from TM_YEAR_BASE later.
	*/
	if (increment_overflow32(&y, TM_YEAR_BASE))
		return WRONG;
	while (yourtm.tm_mday <= 0) {
		if (increment_overflow32(&y, -1))
			return WRONG;
		li = y + (1 < yourtm.tm_mon);
		yourtm.tm_mday += year_lengths[isleap(li)];
	}
	while (yourtm.tm_mday > DAYSPERLYEAR) {
		li = y + (1 < yourtm.tm_mon);
		yourtm.tm_mday -= year_lengths[isleap(li)];
		if (increment_overflow32(&y, 1))
			return WRONG;
	}
	for ( ; ; ) {
		i = mon_lengths[isleap(y)][yourtm.tm_mon];
		if (yourtm.tm_mday <= i)
			break;
		yourtm.tm_mday -= i;
		if (++yourtm.tm_mon >= MONSPERYEAR) {
			yourtm.tm_mon = 0;
			if (increment_overflow32(&y, 1))
				return WRONG;
		}
	}
	if (increment_overflow32(&y, -TM_YEAR_BASE))
		return WRONG;
	if (! (INT_MIN <= y && y <= INT_MAX))
		return WRONG;
	yourtm.tm_year = y;
	if (yourtm.tm_sec >= 0 && yourtm.tm_sec < SECSPERMIN)
		saved_seconds = 0;
	else if (y + TM_YEAR_BASE < EPOCH_YEAR) {
		/*
		** We can't set tm_sec to 0, because that might push the
		** time below the minimum representable time.
		** Set tm_sec to 59 instead.
		** This assumes that the minimum representable time is
		** not in the same minute that a leap second was deleted from,
		** which is a safer assumption than using 58 would be.
		*/
		if (increment_overflow(&yourtm.tm_sec, 1 - SECSPERMIN))
			return WRONG;
		saved_seconds = yourtm.tm_sec;
		yourtm.tm_sec = SECSPERMIN - 1;
	} else {
		saved_seconds = yourtm.tm_sec;
		yourtm.tm_sec = 0;
	}
	/*
	** Do a binary search (this works whatever time_t's type is).
	*/
	lo = time_t_min;
	hi = time_t_max;
	for ( ; ; ) {
		t = lo / 2 + hi / 2;
		if (t < lo)
			t = lo;
		else if (t > hi)
			t = hi;
		if (! funcp(sp, &t, offset, &mytm)) {
			/*
			** Assume that t is too extreme to be represented in
			** a struct tm; arrange things so that it is less
			** extreme on the next pass.
			*/
			dir = (t > 0) ? 1 : -1;
		} else	dir = tmcomp(&mytm, &yourtm);
		if (dir != 0) {
			if (t == lo) {
				if (t == time_t_max)
					return WRONG;
				++t;
				++lo;
			} else if (t == hi) {
				if (t == time_t_min)
					return WRONG;
				--t;
				--hi;
			}
			if (lo > hi)
				return WRONG;
			if (dir > 0)
				hi = t;
			else	lo = t;
			continue;
		}
#if defined TM_GMTOFF && ! UNINIT_TRAP
		if (mytm.TM_GMTOFF != yourtm.TM_GMTOFF
		    && (yourtm.TM_GMTOFF < 0
			? (-SECSPERDAY <= yourtm.TM_GMTOFF
			   && (mytm.TM_GMTOFF <=
			       (SMALLEST (INT_FAST32_MAX, LONG_MAX)
				+ yourtm.TM_GMTOFF)))
			: (yourtm.TM_GMTOFF <= SECSPERDAY
			   && ((BIGGEST (INT_FAST32_MIN, LONG_MIN)
				+ yourtm.TM_GMTOFF)
			       <= mytm.TM_GMTOFF)))) {
		  /* MYTM matches YOURTM except with the wrong UTC offset.
		     YOURTM.TM_GMTOFF is plausible, so try it instead.
		     It's OK if YOURTM.TM_GMTOFF contains uninitialized data,
		     since the guess gets checked.  */
		  time_t altt = t;
		  int_fast32_t diff = mytm.TM_GMTOFF - yourtm.TM_GMTOFF;
		  if (!increment_overflow_time(&altt, diff)) {
		    struct tm alttm;
		    if (funcp(sp, &altt, offset, &alttm)
			&& alttm.tm_isdst == mytm.tm_isdst
			&& alttm.TM_GMTOFF == yourtm.TM_GMTOFF
			&& tmcomp(&alttm, &yourtm) == 0) {
		      t = altt;
		      mytm = alttm;
		    }
		  }
		}
#endif
		if (yourtm.tm_isdst < 0 || mytm.tm_isdst == yourtm.tm_isdst)
			break;
		/*
		** Right time, wrong type.
		** Hunt for right time, right type.
		** It's okay to guess wrong since the guess
		** gets checked.
		*/
		if (sp == NULL)
			return WRONG;
		for (i = sp->typecnt - 1; i >= 0; --i) {
			if (sp->ttis[i].tt_isdst != yourtm.tm_isdst)
				continue;
			for (j = sp->typecnt - 1; j >= 0; --j) {
				if (sp->ttis[j].tt_isdst == yourtm.tm_isdst)
					continue;
				newt = t + sp->ttis[j].tt_gmtoff -
					sp->ttis[i].tt_gmtoff;
				if (! funcp(sp, &newt, offset, &mytm))
					continue;
				if (tmcomp(&mytm, &yourtm) != 0)
					continue;
				if (mytm.tm_isdst != yourtm.tm_isdst)
					continue;
				/*
				** We have a match.
				*/
				t = newt;
				goto label;
			}
		}
		return WRONG;
	}
label:
	newt = t + saved_seconds;
	if ((newt < t) != (saved_seconds < 0))
		return WRONG;
	t = newt;
	if (funcp(sp, &t, offset, tmp))
		*okayp = true;
	return t;
}

static time_t
time2(struct tm * const	tmp,
      struct tm *(*funcp)(struct state const *, time_t const *,
			  int_fast32_t, struct tm *),
      struct state const *sp,
      const int_fast32_t offset,
      bool *okayp)
{
	time_t	t;

	/*
	** First try without normalization of seconds
	** (in case tm_sec contains a value associated with a leap second).
	** If that fails, try with normalization of seconds.
	*/
	t = time2sub(tmp, funcp, sp, offset, okayp, false);
	return *okayp ? t : time2sub(tmp, funcp, sp, offset, okayp, true);
}

static time_t
time1(struct tm *const tmp,
      struct tm *(*funcp) (struct state const *, time_t const *,
			   int_fast32_t, struct tm *),
      struct state const *sp,
      const int_fast32_t offset)
{
	register time_t			t;
	register int			samei, otheri;
	register int			sameind, otherind;
	register int			i;
	register int			nseen;
	char				seen[TZ_MAX_TYPES];
	unsigned char			types[TZ_MAX_TYPES];
	bool				okay;

	if (tmp == NULL) {
		errno = EINVAL;
		return WRONG;
	}
	if (tmp->tm_isdst > 1)
		tmp->tm_isdst = 1;
	t = time2(tmp, funcp, sp, offset, &okay);
	if (okay)
		return t;
	if (tmp->tm_isdst < 0)
#ifdef PCTS
		/*
		** POSIX Conformance Test Suite code courtesy Grant Sullivan.
		*/
		tmp->tm_isdst = 0;	/* reset to std and try again */
#else
		return t;
#endif /* !defined PCTS */
	/*
	** We're supposed to assume that somebody took a time of one type
	** and did some math on it that yielded a "struct tm" that's bad.
	** We try to divine the type they started from and adjust to the
	** type they need.
	*/
	if (sp == NULL)
		return WRONG;
	for (i = 0; i < sp->typecnt; ++i)
		seen[i] = false;
	nseen = 0;
	for (i = sp->timecnt - 1; i >= 0; --i)
		if (!seen[sp->types[i]]) {
			seen[sp->types[i]] = true;
			types[nseen++] = sp->types[i];
		}
	for (sameind = 0; sameind < nseen; ++sameind) {
		samei = types[sameind];
		if (sp->ttis[samei].tt_isdst != tmp->tm_isdst)
			continue;
		for (otherind = 0; otherind < nseen; ++otherind) {
			otheri = types[otherind];
			if (sp->ttis[otheri].tt_isdst == tmp->tm_isdst)
				continue;
			tmp->tm_sec += sp->ttis[otheri].tt_gmtoff -
					sp->ttis[samei].tt_gmtoff;
			tmp->tm_isdst = !tmp->tm_isdst;
			t = time2(tmp, funcp, sp, offset, &okay);
			if (okay)
				return t;
			tmp->tm_sec -= sp->ttis[otheri].tt_gmtoff -
					sp->ttis[samei].tt_gmtoff;
			tmp->tm_isdst = !tmp->tm_isdst;
		}
	}
	return WRONG;
}

static time_t
mktime_tzname(struct state *sp, struct tm *tmp, bool setname)
{
  if (sp)
    return time1(tmp, localsub, sp, setname);
  else {
    gmtcheck();
    return time1(tmp, gmtsub, gmtptr, 0);
  }
}

#if NETBSD_INSPIRED

time_t
mktime_z(struct state *sp, struct tm *tmp)
{
  return mktime_tzname(sp, tmp, false);
}

#endif

time_t
mktime(struct tm *tmp)
{
  time_t t;
  int err = lock();
  if (err) {
    errno = err;
    return -1;
  }
  tzset_unlocked();
  t = mktime_tzname(lclptr, tmp, true);
  unlock();
  return t;
}

#ifdef STD_INSPIRED

time_t
timelocal(struct tm *tmp)
{
	if (tmp != NULL)
		tmp->tm_isdst = -1;	/* in case it wasn't initialized */
	return mktime(tmp);
}

time_t
timegm(struct tm *tmp)
{
  return timeoff(tmp, 0);
}

time_t
timeoff(struct tm *tmp, long offset)
{
  if (tmp)
    tmp->tm_isdst = 0;
  gmtcheck();
  return time1(tmp, gmtsub, gmtptr, offset);
}

#endif /* defined STD_INSPIRED */

/*
** XXX--is the below the right way to conditionalize??
*/

#ifdef STD_INSPIRED

/*
** IEEE Std 1003.1-1988 (POSIX) legislates that 536457599
** shall correspond to "Wed Dec 31 23:59:59 UTC 1986", which
** is not the case if we are accounting for leap seconds.
** So, we provide the following conversion routines for use
** when exchanging timestamps with POSIX conforming systems.
*/

static int_fast64_t
leapcorr(struct state const *sp, time_t t)
{
	register struct lsinfo const *	lp;
	register int			i;

	i = sp->leapcnt;
	while (--i >= 0) {
		lp = &sp->lsis[i];
		if (t >= lp->ls_trans)
			return lp->ls_corr;
	}
	return 0;
}

NETBSD_INSPIRED_EXTERN time_t ATTRIBUTE_PURE
time2posix_z(struct state *sp, time_t t)
{
  return t - leapcorr(sp, t);
}

time_t
time2posix(time_t t)
{
  int err = lock();
  if (err) {
    errno = err;
    return -1;
  }
  if (!lcl_is_set)
    tzset_unlocked();
  if (lclptr)
    t = time2posix_z(lclptr, t);
  unlock();
  return t;
}

NETBSD_INSPIRED_EXTERN time_t ATTRIBUTE_PURE
posix2time_z(struct state *sp, time_t t)
{
	time_t	x;
	time_t	y;
	/*
	** For a positive leap second hit, the result
	** is not unique. For a negative leap second
	** hit, the corresponding time doesn't exist,
	** so we return an adjacent second.
	*/
	x = t + leapcorr(sp, t);
	y = x - leapcorr(sp, x);
	if (y < t) {
		do {
			x++;
			y = x - leapcorr(sp, x);
		} while (y < t);
		x -= y != t;
	} else if (y > t) {
		do {
			--x;
			y = x - leapcorr(sp, x);
		} while (y > t);
		x += y != t;
	}
	return x;
}

time_t
posix2time(time_t t)
{
  int err = lock();
  if (err) {
    errno = err;
    return -1;
  }
  if (!lcl_is_set)
    tzset_unlocked();
  if (lclptr)
    t = posix2time_z(lclptr, t);
  unlock();
  return t;
}

#endif /* defined STD_INSPIRED */

#ifdef time_tz

/* Convert from the underlying system's time_t to the ersatz time_tz,
   which is called 'time_t' in this file.  */

time_t
time(time_t *p)
{
  time_t r = sys_time(0);
  if (p)
    *p = r;
  return r;
}

#endif

// BEGIN android-added

#include <assert.h>
#include <stdint.h>
#include <arpa/inet.h> // For ntohl(3).

static int __bionic_open_tzdata_path(const char* path_prefix_variable, const char* path_suffix,
                                     const char* olson_id) {
  const char* path_prefix = getenv(path_prefix_variable);
  if (path_prefix == NULL) {
    fprintf(stderr, "%s: %s not set!\n", __FUNCTION__, path_prefix_variable);
    return -1;
  }
  size_t path_length = strlen(path_prefix) + 1 + strlen(path_suffix) + 1;
  char* path = malloc(path_length);
  if (path == NULL) {
    fprintf(stderr, "%s: couldn't allocate %zu-byte path\n", __FUNCTION__, path_length);
    return -1;
  }
  snprintf(path, path_length, "%s/%s", path_prefix, path_suffix);
  int fd = TEMP_FAILURE_RETRY(open(path, OPEN_MODE));
  if (fd == -1) {
    // fprintf(stderr, "%s: could not open \"%s\": %s\n", __FUNCTION__, path, strerror(errno));
    free(path);
    return -2; // Distinguish failure to find any data from failure to find a specific id.
  }

  // byte[12] tzdata_version  -- "tzdata2012f\0"
  // int index_offset
  // int data_offset
  // int zonetab_offset
  struct bionic_tzdata_header {
    char tzdata_version[12];
    int32_t index_offset;
    int32_t data_offset;
    int32_t zonetab_offset;
  } header;
  memset(&header, 0, sizeof(header));
  ssize_t bytes_read = TEMP_FAILURE_RETRY(read(fd, &header, sizeof(header)));
  if (bytes_read != sizeof(header)) {
    fprintf(stderr, "%s: could not read header of \"%s\": %s\n",
            __FUNCTION__, path, (bytes_read == -1) ? strerror(errno) : "short read");
    free(path);
    close(fd);
    return -1;
  }

  if (strncmp(header.tzdata_version, "tzdata", 6) != 0 || header.tzdata_version[11] != 0) {
    fprintf(stderr, "%s: bad magic in \"%s\": \"%.6s\"\n",
            __FUNCTION__, path, header.tzdata_version);
    free(path);
    close(fd);
    return -1;
  }

#if 0
  fprintf(stderr, "version: %s\n", header.tzdata_version);
  fprintf(stderr, "index_offset = %d\n", ntohl(header.index_offset));
  fprintf(stderr, "data_offset = %d\n", ntohl(header.data_offset));
  fprintf(stderr, "zonetab_offset = %d\n", ntohl(header.zonetab_offset));
#endif

  if (TEMP_FAILURE_RETRY(lseek(fd, ntohl(header.index_offset), SEEK_SET)) == -1) {
    fprintf(stderr, "%s: couldn't seek to index in \"%s\": %s\n",
            __FUNCTION__, path, strerror(errno));
    free(path);
    close(fd);
    return -1;
  }

  off_t specific_zone_offset = -1;
  ssize_t index_size = ntohl(header.data_offset) - ntohl(header.index_offset);
  char* index = malloc(index_size);
  if (index == NULL) {
    fprintf(stderr, "%s: couldn't allocate %zd-byte index for \"%s\"\n",
            __FUNCTION__, index_size, path);
    free(path);
    close(fd);
    return -1;
  }
  if (TEMP_FAILURE_RETRY(read(fd, index, index_size)) != index_size) {
    fprintf(stderr, "%s: could not read index of \"%s\": %s\n",
            __FUNCTION__, path, (bytes_read == -1) ? strerror(errno) : "short read");
    free(path);
    free(index);
    close(fd);
    return -1;
  }

  static const size_t NAME_LENGTH = 40;
  struct index_entry_t {
    char buf[NAME_LENGTH];
    int32_t start;
    int32_t length;
    int32_t unused; // Was raw GMT offset; always 0 since tzdata2014f (L).
  };

  size_t id_count = (ntohl(header.data_offset) - ntohl(header.index_offset)) / sizeof(struct index_entry_t);
  struct index_entry_t* entry = (struct index_entry_t*) index;
  for (size_t i = 0; i < id_count; ++i) {
    char this_id[NAME_LENGTH + 1];
    memcpy(this_id, entry->buf, NAME_LENGTH);
    this_id[NAME_LENGTH] = '\0';

    if (strcmp(this_id, olson_id) == 0) {
      specific_zone_offset = ntohl(entry->start) + ntohl(header.data_offset);
      break;
    }

    ++entry;
  }
  free(index);

  if (specific_zone_offset == -1) {
    // fprintf(stderr, "%s: couldn't find zone \"%s\"\n", __FUNCTION__, olson_id);
    free(path);
    close(fd);
    return -1;
  }

  if (TEMP_FAILURE_RETRY(lseek(fd, specific_zone_offset, SEEK_SET)) == -1) {
    fprintf(stderr, "%s: could not seek to %ld in \"%s\": %s\n",
            __FUNCTION__, specific_zone_offset, path, strerror(errno));
    free(path);
    close(fd);
    return -1;
  }

  // TODO: check that there's TZ_MAGIC at this offset, so we can fall back to the other file if not.

  free(path);
  return fd;
}

static int __bionic_open_tzdata(const char* olson_id) {
  int fd = __bionic_open_tzdata_path("ANDROID_DATA", "/misc/zoneinfo/current/tzdata", olson_id);
  if (fd < 0) {
    fd = __bionic_open_tzdata_path("ANDROID_ROOT", "/usr/share/zoneinfo/tzdata", olson_id);
    if (fd == -2) {
      // The first thing that 'recovery' does is try to format the current time. It doesn't have
      // any tzdata available, so we must not abort here --- doing so breaks the recovery image!
      fprintf(stderr, "%s: couldn't find any tzdata when looking for %s!\n", __FUNCTION__, olson_id);
    }
  }
  return fd;
}

// Caches the most recent timezone (http://b/8270865).
static int __bionic_tzload_cached(const char* name, struct state* const sp, const int doextend) {
  lock();

  // Our single-item cache.
  static char* g_cached_time_zone_name;
  static struct state g_cached_time_zone;

  // Do we already have this timezone cached?
  if (g_cached_time_zone_name != NULL && strcmp(name, g_cached_time_zone_name) == 0) {
    *sp = g_cached_time_zone;
    unlock();
    return 0;
  }

  // Can we load it?
  int rc = tzload(name, sp, doextend);
  if (rc == 0) {
    // Update the cache.
    free(g_cached_time_zone_name);
    g_cached_time_zone_name = strdup(name);
    g_cached_time_zone = *sp;
  }

  unlock();
  return rc;
}

// Non-standard API: mktime(3) but with an explicit timezone parameter.
// This can't actually be hidden/removed until we fix MtpUtils.cpp
__attribute__((visibility("default"))) time_t mktime_tz(struct tm* const tmp, const char* tz) {
  struct state* st = malloc(sizeof(*st));
  time_t return_value;

  if (st == NULL)
    return 0;
  if (__bionic_tzload_cached(tz, st, true) != 0) {
    // TODO: not sure what's best here, but for now, we fall back to gmt.
    gmtload(st);
  }

  return_value = time1(tmp, localsub, st, 0L);
  free(st);
  return return_value;
}

// END android-added
