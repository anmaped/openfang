/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * WvStream-based TCP connection and server classes.
 */ 
#ifndef __WVTCPLISTENER_H
#define __WVTCPLISTENER_H

#include "wvlistener.h"
#include "wvaddr.h"

class WvIStreamList;

/** Class to easily create the Server side of a WvTCPConn. */
class WvTCPListener : public WvListener
{
public:
    /**
     * Create a WvStream that listens on _listenport of the current machine
     * This is how you set up a TCP Server.
     */
    WvTCPListener(const WvIPPortAddr &_listenport);

    virtual ~WvTCPListener();
    
    /**
     * return a new WvTCPConn socket corresponding to a newly-accepted
     * connection.  If no connection is ready immediately, we may end up
     * blocking, or we may return NULL.  You should use select(read=true)
     * to check for a waiting connection.
     */
    virtual IWvStream *accept();
    
    /** src() is a bit of a misnomer, but it returns the listener port. */
    virtual const WvIPPortAddr *src() const;
    
protected:
    WvIPPortAddr listenport;
    void accept_callback(WvIStreamList *list,
			 wv::function<void(IWvStream*)> cb,
			 IWvStream *_connection);

public:
    const char *wstype() const { return "WvTCPListener"; }
};


#endif // __WVTCP_H
