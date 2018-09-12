/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2004-2006 Net Integration Technologies, Inc.
 *
 * Pathfinder Software:
 *   Copyright (C) 2007, Carillon Information Security Inc.
 *
 * This library is licensed under the LGPL, please read LICENSE for details.
 * 
 * This class represents a dbus server, which may have multiple connections
 * at the same time. It is intended purely for unit testing and debugging
 * purposes and by no means should be used in production code (use the
 * dbus daemon for that).
 * 
 */ 
#ifndef __WVDBUSSERVER_H
#define __WVDBUSSERVER_H

#include "wvlistener.h"
#include "wvhashtable.h"
#include "wvlog.h"
#include "wvistreamlist.h"
#include <stdint.h>

class WvDBusMsg;
class WvDBusConn;
DeclareWvList(WvDBusConn);


class WvDBusServer : public WvIStreamList
{
    WvIStreamList listeners;
public:
    /* 
     * Constructs a new DBus server.  You must then call listen() to
     * actually listen for new connections.
     */
    WvDBusServer();
    
    /**
     * Not actually defined.  Just prevents accidental copying.
     */
    WvDBusServer(WvDBusServer &c);
    
    /**
     * Shut down this server.
     */
    virtual ~WvDBusServer();
    
    /**
     * Listen using a given WvListener moniker.  Until you do this at least
     * once, this stream is !isok().  It is okay to call listen() more
     * than once if you want to listen on more than one port.
     * 
     * For example:
     *    WvDBusServer s;
     *    s.listen("unix:/tmp/foo");
     */
    void listen(WvStringParm moniker);

    virtual bool isok() const;
    virtual int geterr() const;
    
    /**
     * Register a given dbus service name as belonging to a particular
     * connection.
     */
    void register_name(WvStringParm name, WvDBusConn *conn);
    
    /**
     * Undo a register_name().
     */
    void unregister_name(WvStringParm name, WvDBusConn *conn);
    
    /**
     * Forget all name registrations for a particular connection.  Also
     * forget all serial numbers attached to that connection.  Mostly useful
     * when a connection closes.
     */
    void unregister_conn(WvDBusConn *conn);
    
    /**
     * get the full, final address (identification guid and all) of the server
     * if there's more than one listener, returns one of them.
     */
    WvString get_addr();

private:
    WvLog log;
    WvDBusConnList all_conns;
    std::map<WvString,WvDBusConn*> name_to_conn;
    
    void new_connection_cb(IWvStream *s);
    void conn_closed(WvStream &s);
	
    bool do_server_msg(WvDBusConn &conn, WvDBusMsg &msg);
    bool do_bridge_msg(WvDBusConn &conn, WvDBusMsg &msg);
    bool do_broadcast_msg(WvDBusConn &conn, WvDBusMsg &msg);
    bool do_gaveup_msg(WvDBusConn &conn, WvDBusMsg &msg);
};

#endif // __WVDBUSSERVER_H
