/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * UniClientGen is a UniConfGen for retrieving data from the
 * UniConfDaemon.
 */
#ifndef __UNICONFCLIENT_H
#define __UNICONFCLIENT_H

#include "uniconfgen.h"
#include "wvlog.h"
#include "wvstringlist.h"
#include "uniclientconn.h"
#include "uniconfkey.h"

/**
 * Communicates with a UniConfDaemon to fetch and store keys and
 * values.
 * 
 * To mount, use the moniker prefix "unix:" followed by the
 * path of the Unix domain socket used by the UniConfDaemon.
 * Alternately, use the moniker prefix "tcp:" followed by the
 * hostname, a colon, and the port of a machine that serves
 * UniConfDaemon requests over TCP.
 * 
 */
class UniClientGen : public UniConfGen
{
    UniClientConn *conn;

    WvLog log;

    WvString result_key;        /*!< the key that the current result is from */
    WvString result;            /*!< the result from the current key */
    
    UniListIter *result_list;   /*!< result list for iterations */

    bool cmdinprogress;     /*!< true while a command is in progress */
    bool cmdsuccess;        /*!< true when a command completed successfully */

    time_t timeout; // command timeout in ms

    int version; /*!< version number of the protocol */

public:
    /**
     * Creates a generator which can communicate with a daemon using
     * the specified stream.
     * "stream" is the raw connection
     */
    UniClientGen(IWvStream *stream, WvStringParm dst = WvString::null);

    virtual ~UniClientGen();

    time_t set_timeout(time_t _timeout);

    /***** Overridden members *****/

    virtual bool isok();

    virtual bool refresh();
    virtual void flush_buffers();
    virtual void commit(); 
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual bool haschildren(const UniConfKey &key);
    virtual Iter *iterator(const UniConfKey &key);
    virtual Iter *recursiveiterator(const UniConfKey &key);

protected:
    virtual Iter *do_iterator(const UniConfKey &key, bool recursive);
    void conncallback();
    bool do_select();
};


#endif // __UNICONFCLIENT_H
