/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */ 
#ifndef __WVUNIXLISTENER_H
#define __WVUNIXLISTENER_H

#include "wvlistener.h"
#include "wvaddr.h"

#ifndef _WIN32

/** Server end of a Unix Sockets stream */
class WvUnixListener : public WvListener
{
public:
    WvUnixListener(const WvUnixAddr &_addr, int create_mode);
    virtual ~WvUnixListener();
    virtual void close();
    
    /**
     * return a new WvUnixConn socket corresponding to a newly-accepted
     * connection.  If no connection is ready immediately, we wait for
     * one indefinitely.  You can use select(read=true) to check for a
     * waiting connection.
     */
    IWvStream *accept();
    
    /** src() is a bit of a misnomer, but it returns the socket address. */
    virtual const WvUnixAddr *src() const;
    
protected:
    WvUnixAddr addr;
    bool bound_okay;

    void accept_callback(WvIStreamList *list,
			 wv::function<void(IWvStream*)> cb,
			 IWvStream *_connection);

public:
    const char *wstype() const { return "WvUnixListener"; }
};

#endif // _WIN32

#endif // __WVUNIXLISTENER_H
