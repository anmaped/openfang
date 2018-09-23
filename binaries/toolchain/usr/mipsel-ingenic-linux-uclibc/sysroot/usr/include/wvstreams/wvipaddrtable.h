/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * WvIPAddrs are used a lot more often than WvIPAddrTables, so the Table need
 * not be defined most of the time.  Include this file if you need it.
 *
 */
#ifndef __WVIPADDRTABLE_H
#define __WVIPADDRTABLE_H

#include "wvaddr.h"
#include "wvhashtable.h"

DeclareWvTable(WvIPAddr);

#endif // __WVIPADDRTABLE_H
