/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */ 
#ifndef __WVUNIXSOCKET_H
#define __WVUNIXSOCKET_H

#include "wvfdstream.h"
#include "wvaddr.h"

class WvIStreamList;
class WvUnixListener;
class WvUnixConn;

#ifndef _WIN32

/**
 * WvStream-based Unix domain socket connection class.
 * 
 * Unlike WvTCPConn, WvUnixConn makes connections synchronously because either
 * the remote server is there, or it isn't.  For convenience, we'll just
 * ignore situations where it's a local server but still slow.
 * 
 * FIXME: support SOCK_DGRAM mode somehow.  This is a bit tricky since the
 * listener/connection separation doesn't make as much sense then.  I guess we
 * could just ignore the listener or something...
 * 
 * FIXME: use the weird credential-passing stuff to exchange pid, uid, and gid
 * with the remote end of the socket.  See the unix(7) man page.  This would
 * be very cool for authentication purposes.
 */
class WvUnixConn : public WvFdStream
{
    friend class WvUnixListener;
protected:
    WvUnixAddr addr;
    
    /** connect an already-open socket (used by WvUnixListener) */
    WvUnixConn(int _fd, const WvUnixAddr &_addr);
    
public:
    /** connect a new socket */
    WvUnixConn(const WvUnixAddr &_addr);

    virtual ~WvUnixConn();
    
    /**
     * the local address of this socket (ie. from getsockname())
     * really useful only for transparent proxies, but always available.
     * may be 0.0.0.0 if we did not bind explicitly!
     */
    const WvUnixAddr &localaddr() { return addr; }
    
    /**
     * return the remote address (source of all incoming packets),
     * which is a constant for any given connection.
     * This doesn't make much sense in Unix domain sockets, so we just
     * return localaddr() instead.
     */
    virtual const WvUnixAddr *src() const;
    
public:
    const char *wstype() const { return "WvUnixConn"; }
};

#endif // _WIN32

#endif // __WVUNIXSOCKET_H
