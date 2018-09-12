/* -*- Mode: C++ -*-
 * Worldvisions Tunnel Vision Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * High-level abstraction for creating daemon processes that do
 * nothing but listen on a list of WvStreams and add connections
 * to the global list.
 */
#ifndef __WVSTREAMSDAEMON_H
#define __WVSTREAMSDAEMON_H

#include "wvdaemon.h"
#include "iwvstream.h"
#include "wvistreamlist.h"

/*!
@brief WvStreamsDaemon - High-level abstraction for a daemon process that
does nothing but add streams to the global list and execute it.

This is generally what a modern WvStreams-based daemon should look like.

The WvDaemonCallback function passed in the constructor is used to
populate the globallist with streams that are necessary when the
daemon starts, such as listening sockets.  These streams are added
using the WvStreamsDaemon::add_stream, WvStreamsDaemon::add_die_stream
and WvStreamsDaemon::add_restart_stream members, the last two
governing what happens to the daemon when the stream is !isok().

*/
class WvStreamsDaemon : public WvDaemon
{
private:

    WvDaemonCallback callback;

    bool do_full_close;
    WvIStreamList streams;

    void init(WvDaemonCallback cb);

protected:

    virtual void do_start();
    virtual void do_run();
    virtual void do_stop();

private:

    void restart_close_cb(IWvStream *s, const char *id);
    void die_close_cb(IWvStream *s, const char *id);

public:

    //! Construct a new WvStreamsDaemon with given name and version, and
    //! use the cb function to populate the daemon with its initial streams
    WvStreamsDaemon(WvStringParm name,
            WvStringParm version,
            WvDaemonCallback cb) :
        WvDaemon(name, version, WvDaemonCallback(),
                WvDaemonCallback(), WvDaemonCallback())
    {
        init(cb);
    }

    //! Add a stream to the daemon; don't do anything if it goes !isok().
    //! This should be called from the WvDaemonCallback function
    //! passed to the constructor.
    void add_stream(IWvStream *istream,
		    bool auto_free, const char *id);
    //! Add a stream to the daemon; the daemon will restart, re-populating
    //! the initial streams using the callback passed to the constructor,
    //! if the stream goes !isok().
    //! This should be called from the WvDaemonCallback function
    //! passed to the constructor.
    void add_restart_stream(IWvStream *istream,
			    bool auto_free, const char *id);
    //! Add a stream to the daemon; if the stream goes !isok() the daemon
    //! will exit.
    //! This should be called from the WvDaemonCallback function
    //! passed to the constructor.
    void add_die_stream(IWvStream *istream,
			bool auto_free, const char *id);

    //! If this member is called then any existing streams on the globallist 
    //! added *after* the WvDaemonCallback was executed will be closed
    //! if the daemon restarts; otherwise, they will persist after the restart.
    void close_existing_connections_on_restart()
    {
        do_full_close = true;
    }

    //! Change the callback function and userdata
    void setcallback(WvDaemonCallback cb);

private:
    //! Create some undefined overrides to prevent accidentally using a
    //! WvString as an id; these functions will keep a long-term reference to
    //! the string, so you should probably use a string constant.
    void add_stream(IWvStream *istream, bool auto_free, WvString id);
    void add_restart_stream(IWvStream *istream, bool auto_free, WvString id);
    void add_die_stream(IWvStream *istream, bool auto_free, WvString id);
public:
    const char *wstype() const { return "WvStreamsDaemon"; }
};
#endif // __WVSTREAMSDAEMON_H
