/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Common hash functions for use with wvscatterhash.h and wvhashtable.h.
 */
#ifndef __WVHASH_H
#define __WVHASH_H

#include "wvstring.h"

// predefined hashing functions (note: string hashes are case-insensitive)
unsigned WvHash(WvStringParm s);
unsigned WvHash(const char *s);
unsigned WvHash(const int &i);
unsigned WvHash(const void *p);


// Default comparison function used by WvHashTable
template <class K>
struct OpEqComp
{
    static bool compare(const K *key1, const K *key2)
        { return *key1 == *key2; }
};


// Case-insensitive comparison function for WvHashTable
template <class K>
struct StrCaseComp
{
    static bool compare(const K *key1, const K *key2)
        { return strcasecmp(*key1, *key2) == 0; }
};

#endif // __WVHASH_H
