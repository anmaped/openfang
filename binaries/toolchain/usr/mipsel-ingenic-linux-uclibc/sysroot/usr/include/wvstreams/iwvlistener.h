/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * An interface for "listeners", streams that spawn other streams from
 * (presumably) incoming connections.
 */ 
#ifndef __IWVLISTENER_H
#define __IWVLISTENER_H

#include "iwvstream.h"

typedef wv::function<void(IWvStream*)> IWvListenerCallback;
typedef wv::function<IWvStream*(IWvStream*)> IWvListenerWrapper;

class IWvListener : public IWvStream
{
public:
    static IWvListener *create(WvString moniker, IObject *obj = NULL);
    
    /**
     * Accept a connection from this stream.  If none are available right now,
     * might return NULL or block.  (NULL is preferable.)
     */
    virtual IWvStream *accept() = 0;
    
    /**
     * Set a user-defined function to be called when a new connection is
     * available.  The new connection IWvStream is given, and it's the
     * callback's responsibility to make sure it gets freed properly.
     */
    virtual IWvListenerCallback onaccept(IWvListenerCallback _cb) = 0;
    
    /**
     * Add a wrapper function for this stream: something that accept() will
     * call to possibly wrap the stream from accept() before returning it.
     * You can use this more than once; the wrappers will be called in
     * order (so the "innermost" stream is the original, the first wrapper
     * is next, and so on).
     */
    virtual void addwrap(IWvListenerWrapper _wrapper) = 0;
};

DEFINE_IID(IWvListener, {0xe7c2433a, 0x6d5c, 0x4345, {0x83,
	0xee, 0xc0, 0x0f, 0xa7, 0xe3, 0x08, 0xeb}});

#endif // __IWVLISTENER_H
