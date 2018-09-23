/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2005 Net Integration Technologies, Inc.
 *
 * Implementation of an efficient lookup for a set characters.
 *
 * It is, however, a little space intensive, but you should statically
 * create them in your functions, and then they won't be so bad.
 */
#ifndef __WVSTRINGMASK_H
#define __WVSTRINGMASK_H

#include "wvstring.h"

/**
 * A class used to provide a masked lookup for characters in a string.
 */
class WvStringMask
{
public:
    /**
     * Create a WvStringMask out of a WvString.  When looked up,
     * characters in 's' will return true.
     */
    WvStringMask(WvStringParm s = WvString::null);
    WvStringMask(char c);

    /**
     * Look up a character.  This will return true if 'c' is in 'set'.
     */
    bool operator[](const char c) const;

    /**
     * Get the first character set into the mask
     */
    const char first() const;

    /**
     * Clear the WvStringMask, so that all lookups return false.
     */
    void zap();

    /**
     * Set a character 'c' to a particular truth value.
     */
    void set(const char c, bool value);

    /**
     * Set all characters in string 's' to a particular truth value.
     */
    void set(WvStringParm s, bool value);

private:
    bool _set[256];
    char _first;
};

#endif // __WVSTRINGMASK_H
