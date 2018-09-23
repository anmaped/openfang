/*
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Manages a UniConf daemon session.
 */
#ifndef __UNICONFDAEMONCONN_H
#define __UNICONFDAEMONCONN_H

#include "uniconf.h"
#include "uniclientconn.h"
#include "unipermgen.h"
#include "wvlog.h"
#include "wvhashtable.h"

#define NUM_WATCHES 113
#define CONTINUE_SELECT_AT 100

class UniConfDaemon;

/**
 * Retains all state and behavior related to a single UniConf daemon
 * connection.
 */
class UniConfDaemonConn : public UniClientConn 
{
public:
    UniConfDaemonConn(WvStream *s, const UniConf &root);
    virtual ~UniConfDaemonConn();

    virtual void close();

    virtual void execute();

protected:
    UniConf root;

    virtual void do_invalid(WvStringParm c);
    virtual void do_malformed(UniClientConn::Command);
    virtual void do_noop();
    virtual void do_reply(WvStringParm reply);
    virtual void do_get(const UniConfKey &key);
    virtual void do_set(const UniConfKey &key, WvStringParm value);
    virtual void do_remove(const UniConfKey &key);
    virtual void do_subtree(const UniConfKey &key, bool recursive);
    virtual void do_haschildren(const UniConfKey &key);
    virtual void do_commit();
    virtual void do_refresh();
    virtual void do_quit();
    virtual void do_help();

    virtual void addcallback();
    virtual void delcallback();

    void deltacallback(const UniConf &cfg, const UniConfKey &key);
};

#endif // __UNICONFDAEMONCONN_H
