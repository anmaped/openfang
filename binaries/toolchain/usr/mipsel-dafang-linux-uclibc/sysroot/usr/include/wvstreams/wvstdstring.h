/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Some helper functions for WvString so it's more easily interchangeable
 * with std::string.  These functions are in a separate include file so we
 * don't need a separate library *and* we don't need to #include <string> in
 * wvstring.h.
 */
#ifndef __WVSTDSTRING_H
#define __WVSTDSTRING_H

#include "wvstring.h"

inline WvFastString::WvFastString(const std::string &s)
{
    construct(s.c_str());
}


inline WvString::WvString(const std::string &s)
{
    construct(s.c_str());
}

#if 0
inline WvFastString::operator std::string() const
{
    return cstr();
}
#endif

#endif // __WVSTDSTRING_H
