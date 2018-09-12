/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * Portable standins for getuid() and friends.
 */ 
#ifndef __WVUID_H
#define __WVUID_H

#include "wvstring.h"

#if WIN32
  typedef WvString wvuid_t;
  #define WVUID_INVALID (WvString::null)
#else // not WIN32
  #include <pwd.h>
  typedef uid_t wvuid_t;
  #define WVUID_INVALID ((wvuid_t)(-1))
#endif

/**
 * Convert a uid object into a human-readable username.
 * Returns WvString::null if the uid isn't findable or doesn't exist.
 */
WvString wv_username_from_uid(wvuid_t uid);

/**
 * Convert a human-readable username into a machine-usable uid.
 * Returns WVUID_INVALID if the username isn't findable or doesn't exist.
 */
wvuid_t wv_uid_from_username(WvString username);

/**
 * Return the machine-usable uid corresponding to the current session user.
 * 
 * Corresponds to getuid() in Unix; parallels for functions
 * like getgid() and geteuid() are not currently available.
 * 
 * In Unix, this should never fail.  In Windows, there is no guarantee that
 * it will work.  It returns WVUID_INVALID on failure.
 */
wvuid_t wvgetuid();


#endif // __WVUID_H
