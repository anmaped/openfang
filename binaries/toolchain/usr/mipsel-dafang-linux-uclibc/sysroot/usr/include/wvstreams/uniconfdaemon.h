/*
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2004 Net Integration Technologies, Inc.
 *
 * Manages a UniConf daemon.
 */
#ifndef __UNICONFDAEMON_H
#define __UNICONFDAEMON_H

#include "wvlog.h"
#include "wvistreamlist.h"
#include "uniconf.h"
#include "wvaddr.h"

class UniConfDaemon : public WvIStreamList
{
    UniConf cfg;
    WvLog log, debug;
    bool authenticate;
    IUniConfGen *permgen;

public:
    /**
     * Create a UniConfDaemon to serve the Uniconf tree cfg.  If auth is
     * true, require authentication through PAM before accepting connections.
     */
    UniConfDaemon(const UniConf &cfg, bool auth, IUniConfGen *permgen);
    virtual ~UniConfDaemon();

    virtual void close();

    void accept(WvStream *stream);
    
    /**
     * Start listening on a socket described by the given WvListener
     * moniker.
     */
    void listen(WvStringParm lmoniker);

private:
    void listencallback(IWvStream *s);
};

#endif // __UNICONFDAEMON_H
