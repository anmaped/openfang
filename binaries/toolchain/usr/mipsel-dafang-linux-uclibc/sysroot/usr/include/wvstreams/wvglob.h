/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2004 Net Integration Technologies, Inc.
 * 
 * Globbing support (ie. filename wildcards) through WvRegex
 */ 
#ifndef __WVGLOB_H
#define __WVGLOB_H

#include "wvregex.h"

/*!
@brief WvGlob -- Unified support for filename globbing.
!*/
class WvGlob : public WvRegex
{

private:

    static const bool normal_quit_chars[256];
    static const bool brace_quit_chars[256];

    static WvString glob_to_regex(const char *src, size_t &src_used,
            char *dst, size_t &dst_used, const bool quit_chars[256]);

public:
    
    //!
    //! Construct an empty glob object.  Matches will always fail until set()
    //! is called with a valid glob pattern.
    //!
    WvGlob();
    //!
    //! Construct an glob object for the given pattern.
    //!
    WvGlob(WvStringParm glob);
    
    //!
    //! Replace the current regex to match with a new one.
    //!
    //! \param regex The new regular expression to match
    //! \param cflags CFlags used to compile the regular expression;
    //!               the defaults are case sensitive, extended RE.
    //!
    bool set(WvStringParm glob);

    //!
    //! Convert a glob string to its regex equvilent.  All wildcards
    //! (*, ?, {x,y,z}) are wrapped in parens for capturing into registers
    //!
    static WvString glob_to_regex(WvStringParm glob, WvString *errstr);
};

#endif // __WVGLOB_H
