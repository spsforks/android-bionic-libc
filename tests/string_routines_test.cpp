#include <gtest/gtest.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "time.h"

#define RANDOMIZE srandom( time(NULL) )

#include <string.h>
#include <wchar.h>

#define POS_ITER  10
#define MIN(a,b) (((a)<(b))?(a):(b)) 

#define MAX_AL  64
#define NUM_AL 10
#define WNUM_AL 4
int alignments[NUM_AL] = { 24, 32, 16, 48, 1, 2 , 3, 0, 5, 11 };

#define ITER    500

int len[ITER + 1];

int set_len (int max_len) {
  len[0] = 0;
  int n = 1, l, i;

  for (i = 1; i < ITER; i++) {
    l = (int)exp (log ((double) max_len) * i/ITER);
    if (l != len[n - 1]) {
      len[n] = l;
      n++;
    }
  }
  len[n] = max_len;
  n++;
  return n;
}


#define MAX_LEN     1024

int test_strcat (int n) {
  char *glob_ptr, *glob_ptr1, *glob_ptr2, *ptr, *ptr1, *ptr2;
  int i, j, align1, align2;
  int error = 0;

  glob_ptr = (char *) valloc (2 * MAX_LEN + MAX_AL);
  glob_ptr1 = (char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (char *) valloc (2 * MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];

      ptr = glob_ptr + align1;
      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      memset (ptr2, '\2', MAX_LEN);
      ptr2[MAX_LEN - 1] = '\0';
      memcpy (ptr, ptr2, 2 * MAX_LEN);

      memset (ptr1, random() & 255, len[i]);
      ptr1[random() % len[i]] = '\0';
      ptr1[len[i] - 1] = '\0';

      strcpy (ptr + MAX_LEN - 1, ptr1);

      if (strcat (ptr2, ptr1) != ptr2) {
        printf ("FAIL: return value\n");
        error++;
      }

      if (memcmp (ptr, ptr2, 2 * MAX_LEN) != 0) {
        printf ("ITER: %d\tlen: %d\talign1: %d\talign2: %d\n", i, len[i], align1, align2);
        error++;
      }
    }
  }
  free (glob_ptr);
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_strchr (int n) {
  unsigned char *glob_ptr1, *ptr1; char *ref;
  int i, j, align1, pos;
  int error = 0;
  int seek_char = random() & 255;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;

      if (~seek_char > 0)
        memset (ptr1, ~seek_char, len[i]);
      else
        memset (ptr1, '\1', len[i]);
      ptr1[len[i] - 1] = '\0';

      pos = random() % MAX_LEN;
      if (pos >= len[i] - 1) {
        if (seek_char == 0)
          ref = (char *)ptr1 + len[i] - 1;
        else
          ref = NULL;
      }
      else {
        ptr1[pos] = seek_char;
        ref = (char *) (ptr1 + pos);
      }

      if (strchr ((char *)ptr1, seek_char) != ref) {
        printf ("ITER: %d\tlen: %d\talign1: %d\tseek_char: %d\tpos: %u\n",
                    i, len[i], align1, seek_char, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  return error;
}


int test_strcmp (int n) {
  unsigned char *glob_ptr1, *glob_ptr2, *ptr1, *ptr2;
  int i, j, r, ref, align1, align2, pos;
  int error = 0;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];

      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      memset (ptr1, 'v', MAX_LEN);
      memset (ptr2, 'n', MAX_LEN);
      ptr1[len[i] - 1] = '\0';
      ptr2[len[i] - 1] = '\0';

      pos = random() % MAX_LEN;
      if (!pos) pos = 1;
      if (pos >= len[i] - 1) {
        memcpy (ptr1, ptr2, len[i]);
        ref = 0;
        r = strcmp ((char *)ptr1, (char *)ptr2);
      }
      else {
        memcpy (ptr1, ptr2, pos);
        if (ptr1[pos] > ptr2[pos]) {
          ref = 1;
        }
        else if (ptr1[pos] == ptr2[pos]) {
          ptr1[pos + 1] = '\0';
          ptr2[pos + 1] = '\0';
          ref = 0;
        }
        else {
          ref = -1;
        }
        r = strcmp ((char *)ptr1, (char *)ptr2);
      }

      if ((r == 0 && ref)
          || (r < 0 && ref >= 0)
          || (r > 0 && ref <= 0)) {
        printf ("ITER: %d\tlen: %u\talign1: %d\talign2: %d\tpos: %u\n",
                    i, len[i], align1, align2, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_strcpy (int n) {
  char *glob_ptr, *glob_ptr1, *glob_ptr2, *ptr, *ptr1, *ptr2;
  int j, align1, align2;
  size_t pos;
  int error = 0;

  glob_ptr = (char *) valloc (2 * MAX_LEN + MAX_AL);
  glob_ptr1 = (char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (j = 0; j < POS_ITER; j++) {
    align1 = alignments[random() % (NUM_AL - 1)];
    align2 = alignments[random() % (NUM_AL - 1)];

    ptr = glob_ptr + align1;
    ptr1 = glob_ptr1 + align1;
    ptr2 = glob_ptr2 + align2;

    pos = random() % MAX_LEN;

    memset (ptr1, '\2', pos);
    ptr1[pos] = '\0';
    ptr1[MAX_LEN - 1] = '\0';

    memcpy (ptr, ptr1, MAX_LEN);

    memset (ptr2, '\1', MAX_LEN);
    ptr2[MAX_LEN - 1] = '\0';

    memset (ptr + MAX_LEN, '\1', MAX_LEN);
    memcpy (ptr + MAX_LEN, ptr1, pos + 1);
    ptr[2 * MAX_LEN - 1] = '\0';

    if (strcpy (ptr2, ptr1) != ptr2) {
      printf ("FAIL: return value\n");
      error++;
    }

    if ( (memcmp (ptr1, ptr, MAX_LEN)) != 0
        || (memcmp (ptr2, ptr + MAX_LEN, MAX_LEN) != 0) ) {
      printf ("ITER: %d\talign1: %d\talign2: %d\tpos: %u\n",
                  j, align1, align2, pos);
      error++;
    }
  }
  free (glob_ptr);
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


#if __BIONIC__
int test_strlcat (int n) {
  char *glob_ptr, *glob_ptr1, *glob_ptr2, *ptr, *ptr1, *ptr2;
  int i, j, pos;
  int align1, align2;
  int error = 0;

  glob_ptr = (char *) valloc (2 * MAX_LEN + MAX_AL);
  glob_ptr1 = (char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (char *) valloc (2 * MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 0; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];
      ptr = glob_ptr + align1;
      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      memset (ptr2, '\2', MAX_LEN + len[i]);
      ptr2[MAX_LEN - 1] = '\0';
      memcpy (ptr, ptr2, MAX_LEN + len[i]);

      pos = random() % MAX_LEN;
      memset (ptr1, '\3', pos);
      ptr1[pos] = '\0';
      if (pos < len[i]) {
        memcpy (ptr + MAX_LEN - 1, ptr1, pos + 1);
      }
      else {
        memcpy (ptr + MAX_LEN - 1, ptr1, len[i]);
        ptr[MAX_LEN + len[i] - 1] = '\0';
      }

      strlcat (ptr2, ptr1, MAX_LEN + len[i]);

      if (memcmp (ptr, ptr2, MAX_LEN + len[i]) != 0) {
        printf ("ITER: %d\tlen: %d\t, align1: %d\talign2: %d\tpos: %d\n",
                    i, len[i], align1, align2, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  free (glob_ptr2);
  free (glob_ptr);
  return error;
}


int test_strlcpy (int n) {
  char *glob_ptr, *glob_ptr1, *glob_ptr2, *ptr, *ptr1, *ptr2;
  int j, pos, r, ref;
  int align1, align2;
  int rand;
  int error = 0;

  glob_ptr = (char *) valloc (2 * MAX_LEN + MAX_AL);
  glob_ptr1 = (char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];

      ptr = glob_ptr + align1;
      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      rand = random() & 255;
      if (rand < 1)
        rand = 1;
      memset (ptr1, rand, MAX_LEN);

      pos = random() % MAX_LEN;
      if (pos < MAX_LEN)
        ptr1[pos] = '\0';
      memcpy (ptr, ptr1, MAX_LEN);

      memset (ptr2, random() & 255, MAX_LEN);
      memcpy (ptr + MAX_LEN, ptr2, MAX_LEN);

      if (pos > MAX_LEN - 1) {
        memcpy (ptr + MAX_LEN, ptr1, MAX_LEN);
        ptr[2 * MAX_LEN - 1] = '\0';
      }
      else
        memcpy (ptr + MAX_LEN, ptr1, pos + 1);
        
      r = strlcpy (ptr2, ptr1, MAX_LEN);
      ref = strlen (ptr2);
      if (r != ref) {
        printf ("FAIL: return value\tr: %d\tref: %d\n", r , ref);
        error++;
      }

      if ( (memcmp (ptr1, ptr, MAX_LEN) != 0)
          || (memcmp (ptr2, ptr + MAX_LEN, MAX_LEN) != 0) ) {
        printf ("ITER: %d\talign1: %d\talign2: %d\tpos: %d\n",
                    j, align1, align2, pos);
        error++;
      }
    }
    free (glob_ptr1);
    free (glob_ptr2);
    free (glob_ptr);
    return error;
}
#endif

int test_strncat (int n) {
  char *glob_ptr, *glob_ptr1, *glob_ptr2, *ptr, *ptr1, *ptr2;
  int i, j, align1, align2;
  size_t r, pos;
  int error = 0;

  glob_ptr = (char *) valloc (2 * MAX_LEN + MAX_AL);
  glob_ptr1 = (char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (char *) valloc (2 * MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];

      ptr = glob_ptr + align1;
      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      memset (ptr2, '\2', MAX_LEN);
      ptr2[MAX_LEN - 1] = '\0';
      memcpy (ptr, ptr2, 2 * MAX_LEN);

      memset (ptr1, random() & 255, len[i]);
      ptr1[random() % len[i]] = '\0';
      ptr1[len[i] - 1] = '\0';

      pos = strlen (ptr1);

      r = random () % len[i];
      strncpy (ptr + MAX_LEN - 1, ptr1, MIN (r, pos));
      ptr[MAX_LEN + MIN (r, pos) - 1] = '\0';

      if (strncat (ptr2, ptr1, r) != ptr2) {
        printf ("FAIL: return value\n");
        error++;
      }

      if (memcmp (ptr, ptr2, 2 * MAX_LEN) != 0) {
        printf ("ITER: %d\tlen: %d\talign1: %d\talign2: %d\tpos: %d\tr: %d\n",
                    i, len[i], align1, align2, pos, r);
        error++;
      }
    }
  }
  free (glob_ptr);
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_strncmp (int n) {
  unsigned char *glob_ptr1, *glob_ptr2, *ptr1, *ptr2;
  int i, j, r, ref, align1, align2, pos;
  int error = 0;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];

      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      memset (ptr1, 'v', MAX_LEN);
      memset (ptr2, 'c', MAX_LEN);
      ptr1[len[i] - 1] = '\0';
      ptr2[len[i] - 1] = '\0';

      pos = random() % MAX_LEN;
      if (!pos) pos = 1;
      if (pos >= len[i] - 1) {
        memcpy (ptr1, ptr2, len[i]);
        ref = 0;
        r = strncmp ((char *)ptr1, (char *)ptr2, len[i]);
      }
      else {
        memcpy (ptr1, ptr2, pos);
        if (ptr1[pos] > ptr2[pos]) {
          ref = 1;
        }
        else if (ptr1[pos] == ptr2[pos]) {
          ptr1[pos + 1] = '\0';
          ptr2[pos + 1] = '\0';
          ref = 0;
        }
        else {
          ref = -1;
        }
        r = strncmp ((char *)ptr1, (char *)ptr2, len[i]);
      }

      if ((r == 0 && ref)
          || (r < 0 && ref >= 0)
          || (r > 0 && ref <= 0)) {
        printf ("ITER: %d\tlen: %d\talign1: %d\talign2: %d\tpos: %d\n",
                    i, len[i], align1, align2, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_strncpy (int n) {
  char *glob_ptr, *glob_ptr1, *glob_ptr2, *ptr, *ptr1, *ptr2;
  int j, align1, align2;
  size_t pos;
  int error = 0;

  glob_ptr = (char *) valloc (2 * MAX_LEN + MAX_AL);
  glob_ptr1 = (char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (j = 0; j < ITER; j++) {
    align1 = alignments[random() % (NUM_AL - 1)];
    align2 = alignments[random() % (NUM_AL - 1)];

    ptr = glob_ptr + align1;
    ptr1 = glob_ptr1 + align1;
    ptr2 = glob_ptr2 + align2;

    memset (ptr1, random() & 255, MAX_LEN);
    ptr1[random () % MAX_LEN] = '\0';
    memcpy (ptr, ptr1, MAX_LEN);

    memset (ptr2, '\1', MAX_LEN);

    if (memchr (ptr1, 0, MAX_LEN))
      pos = strlen (ptr1);
    else
      pos = MAX_LEN - 1;

    memset (ptr + MAX_LEN, '\0', MAX_LEN);
    memcpy (ptr + MAX_LEN, ptr1, pos + 1);
      
    if (strncpy (ptr2, ptr1, MAX_LEN) != ptr2) {
      printf ("FAIL: return value\n");
      error++;
    }

    if (memcmp (ptr1, ptr, MAX_LEN) != 0
        || memcmp (ptr2, ptr + MAX_LEN, MAX_LEN) != 0 ) {
      printf ("ITER: %d\talign1: %d\talign2: %d\tpos: %d\n",
                  j, align1, align2, pos);
      error++;
    }
  }
  free (glob_ptr);
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_strrchr (int n) {
  unsigned char *glob_ptr1, *ptr1; char *ref;
  int i, j, align1, pos;
  int error = 0;
  int seek_char = random() & 255;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;

      if (~seek_char > 0)
        memset (ptr1, ~seek_char, len[i]);
      else
        memset (ptr1, '\1', len[i]);
      ptr1[len[i] - 1] = '\0';

      pos = random() % MAX_LEN;
      if (pos >= len[i] - 1) {
        if (seek_char == 0)
          ref = (char *)ptr1 + len[i] - 1;
        else
          ref = NULL;
      }
      else {
        ptr1[pos] = seek_char;
        ref = (char *) (ptr1 + pos);
      }

      if (strrchr ((char *)ptr1, seek_char) != ref) {
        printf ("ITER: %d\tlen: %d\talign1: %d\tseek_char: %d\tpos: %u\n",
                    i, len[i], align1, seek_char, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  return error;
}


int test_memchr (int n) {
  unsigned char *glob_ptr1, *ptr1, *ref;
  int i, j, align1, pos;
  int error = 0;
  int seek_char = random() & 255;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 0; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;

      memset (ptr1, ~seek_char, len[i]);

      pos = random() % MAX_LEN;
      if (pos >= len[i]) {
        ref = NULL;
      }
      else {
        ptr1[pos] = seek_char;
        ref = (unsigned char *) (ptr1 + pos);
      }

      if (memchr (ptr1, seek_char, len[i]) != ref) {
        printf ("ITER: %d\tlen: %d\talign1: %d\tpos: %d\tseek_char: %u\n",
                    i, len[i], align1, pos, seek_char);
        error++;
      }
    }
  }
  free (glob_ptr1);
  return error;
}


int test_memrchr (int n) {
  unsigned char *glob_ptr1, *ptr1, *ref;
  int i, j, align1, pos;
  int error = 0;
  int seek_char = random() & 255;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 0; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;

      memset (ptr1, ~seek_char, len[i]);

      pos = random() % MAX_LEN;
      if (pos >= len[i]) {
        ref = NULL;
      }
      else {
        ptr1[pos] = seek_char;
        ref = (unsigned char *) (ptr1 + pos);
      }

      if (memrchr (ptr1, seek_char, len[i]) != ref) {
        printf ("ITER: %d\tlen: %d\talign1: %d\tpos: %d\tseek_char: %d\n",
                    i, len[i], align1, pos, seek_char);
        error++;
      }
    }
  }
  free (glob_ptr1);
  return error;
}


int test_memcmp (int n) {
  unsigned char *glob_ptr1, *glob_ptr2, *ptr1, *ptr2;
  int i, j, r, ref, align1, align2, pos;
  int error = 0;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 0; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];

      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      pos = random() % MAX_LEN;
      memset (ptr1, random() & 255, pos);
      memset (ptr2, random() & 255, pos);

      if (pos > len[i]) {
        memcpy (ptr1, ptr2, len[i]);
        ref = 0;
        r = memcmp (ptr1, ptr2, len[i]);
      }
      else {
        memcpy (ptr1, ptr2, pos);
        if (ptr1[pos] > ptr2[pos]) {
          ref = 1;
        }
        else if (ptr1[pos] == ptr2[pos]) {
          ref = 0;
        }
        else {
          ref = -1;
        }
        r = memcmp (ptr1, ptr2, pos + 1);
      }

      if ((r == 0 && ref)
          || (r < 0 && ref >= 0)
          || (r > 0 && ref <= 0)) {
          printf ("ITER: %d\tlen:%d\talign1: %d\talign2: %d\treturned: %d\texpeted: %d\n",
                    i, len[i], align1, align2, r, ref);
          error++;
      }
    }
  }
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


#undef MAX_LEN
#define MAX_LEN     1024 / sizeof (wchar_t)

int test_wcscat (int n) {
  wchar_t *glob_ptr, *glob_ptr1, *glob_ptr2, *ptr, *ptr1, *ptr2;
  int i, j, align1, align2;
  int error = 0;

  glob_ptr = (wchar_t *) valloc ((2 * MAX_LEN + MAX_AL) * sizeof (wchar_t));
  glob_ptr1 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));
  glob_ptr2 = (wchar_t *) valloc ((2 * MAX_LEN + MAX_AL) * sizeof (wchar_t));

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (WNUM_AL - 1)];
      align2 = alignments[random() % (WNUM_AL - 1)];

      ptr = glob_ptr + align1;
      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      wmemset (ptr2, L'\2', MAX_LEN);
      ptr2[MAX_LEN - 1] = L'\0';
      wmemcpy (ptr, ptr2, 2 * MAX_LEN);

      wmemset (ptr1, random(), len[i]);
      ptr1[random() % len[i]] = L'\0';
      ptr1[len[i] - 1] = L'\0';

      wcscpy (ptr + MAX_LEN - 1, ptr1);

      if (wcscat (ptr2, ptr1) != ptr2) {
        printf ("FAIL: return value\n");
        error++;
      }

      if (wmemcmp (ptr, ptr2, 2 * MAX_LEN) != 0) {
        printf ("ITER: %d\tlen: %d\talign1: %d\talign2: %d\n", i, len[i], align1, align2);
        error++;
      }
    }
  }
  free (glob_ptr);
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_wcschr (int n) {
  wchar_t *glob_ptr1, *ptr1, *ref;
  int i, j, align1, pos;
  int error = 0;
  int seek_char = random();

  glob_ptr1 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (WNUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;

      if (~seek_char > 0)
        wmemset (ptr1, ~seek_char, MAX_LEN);
      else
        wmemset (ptr1, L'\1', MAX_LEN);
      ptr1[len[i] - 1] = L'\0';

      pos = random() % MAX_LEN;
      if (pos >= len[i] - 1) {
        if (seek_char == 0)
          ref = (wchar_t *)ptr1 + len[i] - 1;
        else
          ref = NULL;
      }
      else {
        ptr1[pos] = seek_char;
        ref = (wchar_t *) (ptr1 + pos);
      }

      if (wcschr (ptr1, seek_char) != ref) {
        printf ("ITER: %d\tlen: %d\talign1: %d\tseek_char: %d\tpos: %u\n",
                    i, len[i], align1, seek_char, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  return error;
}


int test_wcscmp (int n) {
  wchar_t *glob_ptr1, *glob_ptr2, *ptr1, *ptr2;
  int i, j, r, ref, align1, align2, pos;
  int error = 0;

  glob_ptr1 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));
  glob_ptr2 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (WNUM_AL - 1)];
      align2 = alignments[random() % (WNUM_AL - 1)];

      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      wmemset (ptr1, L'\3', MAX_LEN);
      wmemset (ptr2, L'\2', MAX_LEN);
      ptr1[len[i] - 1] = L'\0';
      ptr2[len[i] - 1] = L'\0';

      pos = random() % MAX_LEN;
      if (!pos) pos = 1;
      if (pos >= len[i] - 1) {
        wmemcpy (ptr1, ptr2, len[i]);
        ref = 0;
        r = wcscmp (ptr1, ptr2);
      }
      else {
        wmemcpy (ptr1, ptr2, pos);
        if (ptr1[pos] > ptr2[pos]) {
          ref = 1;
        }
        else if (ptr1[pos] == ptr2[pos]) {
          ptr1[pos + 1] = L'\0';
          ptr2[pos + 1] = L'\0';
          ref = 0;
        }
        else {
          ref = -1;
        }
        r = wcscmp (ptr1, ptr2);
      }

      if ((r == 0 && ref)
          || (r < 0 && ref >= 0)
          || (r > 0 && ref <= 0)) {
        printf ("ITER: %d\tlen: %u\talign1: %d\talign2: %d\tpos: %u\n",
                    i, len[i], align1, align2, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_wcscpy (int n) {
  wchar_t *glob_ptr, *glob_ptr1, *glob_ptr2, *ptr, *ptr1, *ptr2;
  int j, align1, align2;
  size_t pos;
  int error = 0;

  glob_ptr = (wchar_t *) valloc ((2 * MAX_LEN + MAX_AL) * sizeof (wchar_t));
  glob_ptr1 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));
  glob_ptr2 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));

  RANDOMIZE;
  for (j = 0; j < POS_ITER; j++) {
    align1 = alignments[random() % (WNUM_AL - 1)];
    align2 = alignments[random() % (WNUM_AL - 1)];

    ptr = glob_ptr + align1;
    ptr1 = glob_ptr1 + align1;
    ptr2 = glob_ptr2 + align2;

    pos = random() % MAX_LEN;

    wmemset (ptr1, L'\2', MAX_LEN);
    ptr1[pos] = L'\0';
    ptr1[MAX_LEN - 1] = L'\0';

    wmemcpy (ptr, ptr1, MAX_LEN);

    wmemset (ptr2, L'\1', MAX_LEN);
    ptr2[MAX_LEN - 1] = L'\0';

    wmemset (ptr + MAX_LEN, L'\1', MAX_LEN);
    wmemcpy (ptr + MAX_LEN, ptr1, pos + 1);
    ptr[2 * MAX_LEN - 1] = L'\0';

    if (wcscpy (ptr2, ptr1) != ptr2) {
      printf ("FAIL: return value\n");
      error++;
    }

    if ( (wmemcmp (ptr1, ptr, MAX_LEN)) != 0
        || (wmemcmp (ptr2, ptr + MAX_LEN, MAX_LEN) != 0) ) {
      printf ("ITER: %d\talign1: %d\talign2: %d\tpos: %u\n",
                  j, align1, align2, pos);
      error++;
    }
  }
  free (glob_ptr);
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_wcslen (int n) {
  wchar_t *glob_ptr1, *ptr1;
  int i, j, align1, pos;
  int error = 0;

  glob_ptr1 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (WNUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;

      pos = random() % MAX_LEN;
      if (pos > len[i] - 1) {
        pos = len[i] - 1;
      }

      wmemset (ptr1, L'\5', MAX_LEN);
      ptr1[pos] = L'\0';

      if (wcslen (ptr1) != pos) {
        printf ("ITER: %d\tlen: %d\talign1: %d\trand: %d\tpos: %d\n",
                    i, len[i], align1, rand, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  return error;
}


int test_wcsrchr (int n) {
  wchar_t *glob_ptr1, *ptr1, *ref;
  int i, j, align1, pos;
  int error = 0;
  int seek_char = random();

  glob_ptr1 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));

  RANDOMIZE;
  for (i = 1; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (WNUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;

      if (~seek_char > 0)
        wmemset (ptr1, ~seek_char, MAX_LEN);
      else
        wmemset (ptr1, L'\1', MAX_LEN);
      ptr1[len[i] - 1] = L'\0';

      pos = random() % MAX_LEN;
      if (pos >= len[i] - 1) {
        if (seek_char == 0)
          ref = (wchar_t *)ptr1 + len[i] - 1;
        else
          ref = NULL;
      }
      else {
        ptr1[pos] = seek_char;
        ref = (wchar_t *) (ptr1 + pos);
      }

      if (wcsrchr (ptr1, seek_char) != ref) {
        printf ("ITER: %d\tlen: %d\talign1: %d\tseek_char: %d\tpos: %u\n",
                    i, len[i], align1, seek_char, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  return error;
}


int test_wmemcmp (int n) {
  wchar_t *glob_ptr1, *glob_ptr2, *ptr1, *ptr2;
  int i, j, r, ref, align1, align2, pos;
  int error = 0;

  glob_ptr1 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));
  glob_ptr2 = (wchar_t *) valloc ((MAX_LEN + MAX_AL) * sizeof (wchar_t));

  RANDOMIZE;
  for (i = 0; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (WNUM_AL - 1)];
      align2 = alignments[random() % (WNUM_AL - 1)];

      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      pos = random() % MAX_LEN;
      wmemset (ptr1, random(), pos);
      wmemset (ptr2, random(), pos);

      if (pos > len[i]) {
        wmemcpy (ptr1, ptr2, len[i]);
        ref = 0;
        r = wmemcmp (ptr1, ptr2, len[i]);
      }
      else {
        wmemcpy (ptr1, ptr2, pos);
        if (ptr1[pos] > ptr2[pos]) {
          ref = 1;
        }
        else if (ptr1[pos] == ptr2[pos]) {
          ref = 0;
        }
        else {
          ref = -1;
        }
        r = wmemcmp (ptr1, ptr2, pos + 1);
      }

      if ((r == 0 && ref)
          || (r < 0 && ref >= 0)
          || (r > 0 && ref <= 0)) {
          printf ("ITER: %d\tlen:%d\talign1: %d\talign2: %d\treturned: %d\texpeted: %d\n",
                    i, len[i], align1, align2, r, ref);
          error++;
      }
    }
  }
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}

#undef MAX_LEN
#define MAX_LEN     2 * 1024 * 1024

int test_memcpy (int n) {
  unsigned char *glob_ptr1, *glob_ptr2, *ptr1, *ptr2;
  int i, j, align1, align2;
  size_t pos;
  int error = 0;
  int rand = random() & 255;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 0; i < n - 1; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];

      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      pos = random() % (MAX_LEN - len[i]);

      memset (ptr1, rand, len[i]);
      memset (ptr1 + len[i], ~rand, MAX_LEN - len[i]);

      memset (ptr2, rand, len[i]);
      memset (ptr2 + len[i], ~rand, MAX_LEN - len[i]);
      memset (ptr2 + pos, '\0', len[i]);

      if (memcpy (ptr2 + pos, ptr1 + pos, len[i]) != ptr2 + pos) {
        printf ("FAIL: return value\n");
        error++;
      }

      if (memcmp (ptr1, ptr2, MAX_LEN) != 0) {
        printf ("ITER: %d\tlen: %d\talign1: %d\t align2: %d\tpos: %d\n",
                    i, len[i], align1, align1, pos);
        error++;
      }
    }
  }
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_memset (int n) {
  unsigned char *glob_ptr1, *glob_ptr2, *ptr1, *ptr2;
  int i, j, align1, align2;
  size_t pos, k;
  char ch = random() & 255;
  int error = 0;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 0; i < n - 1; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      memset (ptr1, ~ch, MAX_LEN);
      memcpy (ptr2, ptr1, MAX_LEN);

      pos = random() % (MAX_LEN - len[i]);
      for (k = pos; k < pos + len[i]; k++)
        ptr1[k] = ch;

      if (memset (ptr2 + pos, ch, len[i]) != ptr2 + pos) {
        printf ("FAIL: return value\n");
        error++;
      }

      if ( memcmp (ptr1, ptr2, MAX_LEN) != 0 ) {
        printf ("ITER: %d\tlen: %d\talign1: %d\talign2: %d\tchar: %d\n",
                    i, len[i], align1, align2, ch);
        error++;
      }
    }
  }
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_memmove (int n) {
  unsigned char *glob_ptr, *glob_ptr1, *glob_ptr2, *ptr, *ptr1, *ptr2;
  int i, j, align1, align2;
  size_t pos;
  int error = 0;

  glob_ptr1 = (unsigned char *) valloc (2 * MAX_LEN + MAX_AL);
  glob_ptr2 = (unsigned char *) valloc (2 * MAX_LEN + MAX_AL);
  glob_ptr = (unsigned char *) valloc (MAX_LEN + MAX_AL); // buffer

  RANDOMIZE;
  for (i = 0; i < n - 1; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;
      ptr = glob_ptr + align1; // buffer

      memset (ptr1, random() & 255, 2 * MAX_LEN);

      pos = random() % (MAX_LEN - len[i]);

      memset (ptr1, random() & 255, len[i]);
      memcpy (ptr2, ptr1, 2 * MAX_LEN);
      memcpy (ptr, ptr1, len[i]);
      memcpy (ptr1 + pos, ptr, len[i]);

      if (memmove (ptr2 + pos, ptr2, len[i]) != ptr2 + pos) {
        printf ("FAIL: return value\n");
        error++;
      }

      if (memcmp (ptr2, ptr1, 2 * MAX_LEN) != 0) {
        printf ("ITER: %d\tlen: %d\talign1: %d\talign2: %d\tpos: %d\n",
                    i, len[i], align1, align2, pos);
        error++;
      }
    }
  }
  free (glob_ptr);
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_bcopy (int n) {
  unsigned char *glob_ptr1, *glob_ptr2, *ptr1, *ptr2;
  int i, j, align1, align2;
  size_t start;
  int error = 0;

  glob_ptr1 = (unsigned char *) valloc (2 * MAX_LEN + MAX_AL);
  glob_ptr2 = (unsigned char *) valloc (2 * MAX_LEN + MAX_AL);

  RANDOMIZE;
  for (i = 0; i < n; i++) {

    for (j = 0; j < POS_ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      memset (ptr1, random() & 255, MAX_LEN);
      memset (ptr1 + MAX_LEN, random() & 255, MAX_LEN);
      memcpy (ptr2, ptr1, 2 * MAX_LEN);

      start = random() % (2 * MAX_LEN - len[i]);
      memcpy (ptr2 + start, ptr1, len[i]);
      
      bcopy (ptr1, ptr1 + start, len[i]);

      if (memcmp (ptr1, ptr2, 2 * MAX_LEN) != 0 ) {
        printf ("ITER: %d\tlen: %d\talign1: %d\tstart: %d\n",
                    i, len[i], align1, start);
        error++;
      }
    }
  }
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}


int test_bzero (int n) {
  unsigned char *glob_ptr1, *glob_ptr2, *ptr1, *ptr2;
  int j, align1, align2;
  size_t start, end;
  int error = 0;

  glob_ptr1 = (unsigned char *) valloc (MAX_LEN + MAX_AL);
  glob_ptr2 = (unsigned char *) valloc (MAX_LEN + MAX_AL);

  RANDOMIZE;

    for (j = 0; j < ITER; j++) {
      align1 = alignments[random() % (NUM_AL - 1)];
      align2 = alignments[random() % (NUM_AL - 1)];
      ptr1 = glob_ptr1 + align1;
      ptr2 = glob_ptr2 + align2;

      memset (ptr1, random() & 255, MAX_LEN);

      start = random() % MAX_LEN;
      end = start + random() % (MAX_LEN - start);

      memcpy (ptr2, ptr1, start);
      memset (ptr2 + start, '\0', end - start);
      memcpy (ptr2 + end, ptr1 + end, MAX_LEN - end);

      bzero (ptr1 + start, end - start);

      if ( memcmp (ptr1, ptr2, MAX_LEN) != 0 ) {
        printf ("ITER: %d\talign1: %d\talign2: %d\tstart: %d\n",
                    j, align1, align2, start);
        error++;
      }
    }
  free (glob_ptr1);
  free (glob_ptr2);
  return error;
}

#undef MAX_LEN

#define MAKE_STRING_TEST(FUNC)  \
TEST(string, FUNC) {            \
  int n = set_len (MAX_LEN);    \
  int err = test_##FUNC (n);    \
  ASSERT_EQ(err, 0);            \
}


#define MAX_LEN     1024
MAKE_STRING_TEST (strcat)
MAKE_STRING_TEST (strchr)
MAKE_STRING_TEST (strcmp)
MAKE_STRING_TEST (strcpy)

#if __BIONIC__
MAKE_STRING_TEST (strlcat)
MAKE_STRING_TEST (strlcpy)
#endif

MAKE_STRING_TEST (strncat)
MAKE_STRING_TEST (strncmp)
MAKE_STRING_TEST (strncpy)
MAKE_STRING_TEST (strrchr)

MAKE_STRING_TEST (memchr)
MAKE_STRING_TEST (memrchr)
MAKE_STRING_TEST (memcmp)
#undef MAX_LEN

#define MAX_LEN     1024 / sizeof (wchar_t)
MAKE_STRING_TEST (wcscat)
MAKE_STRING_TEST (wcschr)
MAKE_STRING_TEST (wcscmp)
MAKE_STRING_TEST (wcscpy)
MAKE_STRING_TEST (wcslen)
MAKE_STRING_TEST (wmemcmp)
MAKE_STRING_TEST (wcsrchr)
#undef MAX_LEN

#define MAX_LEN     2 * 1024 * 1024
MAKE_STRING_TEST (memcpy)
MAKE_STRING_TEST (memset)
MAKE_STRING_TEST (memmove)

MAKE_STRING_TEST (bcopy)
MAKE_STRING_TEST (bzero)
#undef MAX_LEN
