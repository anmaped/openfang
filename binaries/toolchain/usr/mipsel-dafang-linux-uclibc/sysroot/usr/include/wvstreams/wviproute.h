/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * The WvIPRoute and WvIPRouteList classes, which can manipulate the kernel
 * routing table in useful ways.
 */
#ifndef __WVIPROUTE_H
#define __WVIPROUTE_H

#include "wvaddr.h"
#include "wvlinklist.h"
#include "wvlog.h"

/** Manipulate the kernel routing table in strange and interesting ways ;) */
class WvIPRoute
{
public:
    WvIPRoute(WvStringParm _ifc, const WvIPNet &_net, const WvIPAddr &_gate,
	      int _metric, WvStringParm _table);
    operator WvString() const;
    bool operator== (const WvIPRoute &r2) const;
    
    WvString ifc;
    WvIPNet ip;
    WvIPAddr gateway;
    int metric;
    WvString table; // "advanced ip routing" table name
    WvIPAddr src;
};


DeclareWvList2(WvIPRouteListBase, WvIPRoute);

/** List of IP Routes currently in effect */
class WvIPRouteList : public WvIPRouteListBase
{
public:
    WvLog log;
    
    WvIPRouteList();
    
    /** automatically fill the list with appropriate data from the kernel */
    void get_kernel();
    
    /** automatically set the kernel to the values in the RouteList */
    void set_kernel();
	       
    /** find the routing entry that matches 'addr' */
    WvIPRoute *find(const WvIPAddr &addr);
};


#endif // __WVIPROUTE_H
