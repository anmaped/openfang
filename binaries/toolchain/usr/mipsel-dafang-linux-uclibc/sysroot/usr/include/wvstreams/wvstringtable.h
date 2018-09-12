/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * WvStrings are used a lot more often than WvStringTables, so the Table need
 * not be defined most of the time.  Include this file if you need it.
 *
 */
#ifndef __WVSTRINGTABLE_H
#define __WVSTRINGTABLE_H

#include "wvstring.h"
#include "wvscatterhash.h"

DeclareWvScatterTable2(WvStringTableBase, WvString);

class WvStringTable : public WvStringTableBase
{
    // copy constructor: not defined anywhere!
    WvStringTable(const WvStringTable &t);
public:
    WvStringTable(unsigned _numslots = 0) : WvStringTableBase(_numslots) {};
    WvString join(const char *joinchars = " \t") const;
    void split(WvStringParm s, const char *splitchars = " \t\r\n",
	       int limit = 0);
    void splitstrict(WvStringParm s, const char *splitchars = " \t\r\n",
	       int limit = 0);
};

#endif // __WVSTRINGTABLE_H
