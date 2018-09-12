/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2005 Net Integration Technologies, Inc.
 *
 * Helper classes and functions to add more information to WvCrashes.
 */
#ifndef __WVASSERT_H
#define __WVASSERT_H

#include <assert.h>

#include "wvcrash.h"
#include "wvstring.h"

// WvCrash allows you to print a programme's last will and testament.
// That is, a little note about what it was hoping to do before it
// died.
//
// This helper class lets you write a will, and when it gets
// destroyed, it will restore the old will from before.  This lets you
// safely nest them.
class WvCrashWill
{
public:
    // Leave a will behind.
    WvCrashWill(const char *will);
    WvCrashWill(WVSTRING_FORMAT_DECL);

    // Restore the will that was there before you created this object.
    ~WvCrashWill();

    // Rewrite the will you're leaving behind.
    void rewrite(const char *will);
    void rewrite(WVSTRING_FORMAT_DECL);
private:
    WvString old_will;
};

#if !defined(__GLIBC__)

# define wvassert(expr, args...)            assert(expr)
# define wvassert_perror(errnum)            perror(errnum)

#elif defined(NDEBUG)

# define wvassert(expr, args...)	(__ASSERT_VOID_CAST (0))
# define wvassert_perror(errnum)	(__ASSERT_VOID_CAST (0))

#else // Not NDEBUG

static inline void __wvcrash_leave_will()
{
}

static inline void __wvcrash_leave_will(const char *will)
{
    wvcrash_leave_will(will);
}

static inline void __wvcrash_leave_will(WVSTRING_FORMAT_DECL)
{
    wvcrash_leave_will(WvFastString(WVSTRING_FORMAT_CALL));
}

// Use this function instead of assert().  You may also leave parameters
// at the end, which allow you to log messages.  For instance:
//
// wvassert(a == b, "a: '%s'\n b: '%s'", a, b);
# define wvassert(expr, args...) \
  (__ASSERT_VOID_CAST ((expr) ? 0 :					      \
		       (__wvcrash_leave_will (args),			      \
			(__assert_fail (__STRING(expr), __FILE__, __LINE__,   \
					__ASSERT_FUNCTION), 0))))

// Use this function instead of assert_perror().  You may also leave
// parameters at the end, which allow you to log messages.  For instance:
//
// wvassert(errno, "Error trying to read file: '%s'", filename);
#  define wvassert_perror(errnum, args...) \
  (__ASSERT_VOID_CAST (!(errnum) ? 0 :					      \
		       (__wvcrash_leave_will (args),			      \
			(__assert_perror_fail ((errnum), __FILE__, __LINE__,  \
					       __ASSERT_FUNCTION), 0))))

#endif // NDEBUG

#endif // WVASSERT_H
