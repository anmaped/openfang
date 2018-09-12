/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * DNS name resolver with support for background lookups.
 */
#ifndef __WVRESOLVER_H
#define __WVRESOLVER_H

#include "wvaddr.h"
#include "wvstream.h"
#include "wvlinklist.h"
#include "wvhashtable.h"

class WvResolverHost;
class WvResolverAddr;

DeclareWvDict(WvResolverHost, WvString, name);
DeclareWvDict(WvResolverAddr, WvIPAddr, addr[0]);

DeclareWvList(WvIPAddr);

/** ASynchronous DNS resolver functions, so that we can do non-blocking lookups */
class WvResolver
{
    static int numresolvers;
    static WvResolverHostDict *hostmap;
    static WvResolverAddrDict *addrmap;
public:
    WvResolver();
    ~WvResolver();
    
    /**
     * Return -1 on timeout, or the number of addresses found, which may
     * be 0 if the address does not exist.
     * addrlist, if present, has to be an initialized list.
     * addr points to an internal WvIPAddr and will be deleted automatically.
     */
    int findaddr(int msec_timeout, WvStringParm name,
		 WvIPAddr const **addr, WvIPAddrList *addrlist = NULL);
    int findname(int msec_timeout, WvIPAddr *ipaddr, char **name);

    void clearhost(WvStringParm hostname);
    
    /** add all of our waiting fds to an fd_set for use with select(). */
    void pre_select(WvStringParm hostname, WvStream::SelectInfo &si);

    /** determines whether the resolving process is complete. */
    bool post_select(WvStringParm hostname, WvStream::SelectInfo &si);
};

#endif // __WVRESOLVER_H
