/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 */
#ifndef __WVTIMEUTILS_H
#define __WVTIMEUTILS_H

#ifdef _WIN32
#include "winsock2.h"
#include <time.h>
int gettimeofday(struct timeval *tv, struct timezone *tz);
#else
#include <sys/time.h>
#endif

/** Based on (and interchangeable with) struct timeval. */
class WvTime : public timeval
{
public:
    WvTime()
        { } // WARNING: leaves members uninitialized, like timeval would do!
    WvTime(long long t)
        { tv_sec = long(t/1000000L); tv_usec = long(t%1000000L); }
    WvTime(time_t sec, time_t usec)
        { tv_sec = long(sec); tv_usec = long(usec); }
    WvTime(const struct timeval &tv)
        { tv_sec = tv.tv_sec; tv_usec = tv.tv_usec; }
    WvTime(const WvTime &tv)
        { tv_sec = tv.tv_sec; tv_usec = tv.tv_usec; }
    
    operator long long() const
        { return ((long long)tv_sec)*1000000LL + tv_usec; }
};

static const WvTime wvtime_zero(0, 0);

/** Returns the number of milliseconds between times a and b (ie a-b).
 * Value returned is not rounded, it is chopped via integer division
 * For example: if the result of the subtraction is 0.9, the returned
 * value is 0 if the result is -0.9, the result is also 0.
*/
time_t msecdiff(const WvTime &a, const WvTime &b);

/** Returns the current time of day. */
WvTime wvtime();

/** Adds the specified number of milliseconds to a time value. */
WvTime msecadd(const WvTime &a, time_t msec);

/** Returns the timeval difference between two timevals (ie a-b). */
WvTime tvdiff(const WvTime &a, const WvTime &b);

/** Normalizes the time value. 
 * This means that any two 'equally valued' timevals will be converted
 * so that the seconds and the microseconds are the same in each timeval.
 */
inline void normalize(WvTime &tv)
{
    tv.tv_sec += tv.tv_usec < 0 ? (tv.tv_usec/1000000)-1 : tv.tv_usec/1000000;
    tv.tv_usec %= 1000000;
    tv.tv_usec += tv.tv_usec < 0 ? 1000000 : 0;
}

// Stepped time functions.  Used to synchronize wvstreams.
const WvTime &wvstime(); 
void wvstime_sync();

// This function is just like wvstime_sync(), but will never make the
// time go backward.
void wvstime_sync_forward();

// This sets the time returned by wvstime() to the specified value. To
// be used for unit testing.
void wvstime_set(const WvTime &);

/**
 * Delay for a requested number of milliseconds.
 */
void wvdelay(int msec_delay);

#endif // __WVTIMEUTILS_H
