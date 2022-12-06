#pragma once

#define DEF_WEAK(sym) /* ignore */

struct _TimeLocale {
    const char *abday[7];
    const char *day[7];
    const char *abmon[12];
    const char *mon[12];
    const char *am_pm[2];
    const char *d_t_fmt;
    const char *d_fmt;
    const char *t_fmt;
    const char *t_fmt_ampm;
};

static const struct _TimeLocale _DefaultTimeLocale = {
    {
        "Sun","Mon","Tue","Wed","Thu","Fri","Sat",
    },
    {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
        "Friday", "Saturday"
    },
    {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    },
    {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    },
    {
        "AM", "PM"
    },
    "%a %b %d %H:%M:%S %Y",
    "%m/%d/%y",
    "%H:%M:%S",
    "%I:%M:%S %p"
};

static const struct _TimeLocale* _CurrentTimeLocale = &_DefaultTimeLocale;
