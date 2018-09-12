/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 * 
 * A UniConfGen that reconnects to an inner generator whenever the inner
 * generator is no longer OK.
 */
#ifndef __UNIRETRYGEN_H
#define __UNIRETRYGEN_H

#include "unifiltergen.h"
#include "wvtimeutils.h"
#include "wvlog.h"

/**
 * A UniConfGen that reconnects to an inner generator specified by
 * a moniker whenever the inner generator is no longer OK.  It will
 * try to periodically recreate the inner generator until the resulting
 * object returns isok() true.
 *
 * The UniRetryGen's moniker takes either of the forms retry:<inner moniker>
 * or retry:{<inner moniker> <retry timeout>}; in the second form the retry
 * timeout is specified in milliseconds.  If not specified, the retry timout
 * is 5000ms.  Specifically this means that whenever the retry generator is
 * does not have a good inner generator and at least 5000ms have passed 
 * since the disconnect or last reconnect attempt, reconnection will be 
 * attempted again.
 *
 * The connection is created through the underlying
 * backend's moniker and destroyed by delete.
 *
 * If UniRetryGen's constructor is used directly, a callback of type
 * UniRetryGen::ReconnectCallback can be specified, which will be called
 * whenever the UniRetryGen reconnects to the underlying moniker.  This allows
 * for any necessary resynchronisation, such as a call to refresh().
 *
 * UniRetryGen can be used in combination with UniReplicateGen to create
 * a connection to a UniConf daemon that is robust against network
 * failures through a moniker such as replicate:{retry:tcp:192.168.0.1 tmp:}
 */
class UniRetryGen : public UniFilterGen
{
public:

    typedef wv::function<void(UniRetryGen&)> ReconnectCallback;
    
private:

    WvLog log;

    WvString moniker;

    ReconnectCallback reconnect_callback; 

    time_t retry_interval_ms;
    WvTime next_reconnect_attempt;
    
    void maybe_disconnect();
    void maybe_reconnect();

public:

    UniRetryGen(WvStringParm _moniker,
            ReconnectCallback _reconect_callback = ReconnectCallback(),
            time_t _retry_internal_ms = 5000);

    /***** Overridden methods *****/

    virtual void commit();
    virtual bool refresh();
    virtual void prefetch(const UniConfKey &key, bool recursive);
    virtual void flush_buffers() { }
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual bool exists(const UniConfKey &key);
    virtual bool haschildren(const UniConfKey &key);
    virtual bool isok();
    virtual Iter *iterator(const UniConfKey &key);
    virtual Iter *recursiveiterator(const UniConfKey &key);
};

#endif //__UNIRETRYGEN_H
