/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 */
#ifndef __WVSUBPROCQUEUESTREAM_H
#define __WVSUBPROCQUEUESTREAM_H

#include "wvsubprocqueue.h"
#include "wvlog.h"

/**
 * A variant of WvSubProcQueue that can be added to a WvStreamList so that
 * WvSubProcQueue::go() gets called automatically at a reasonable interval.
 */
class WvSubProcQueueStream : public WvStream, public WvSubProcQueue
{
public:
    WvSubProcQueueStream(int _maxrunning);
    virtual ~WvSubProcQueueStream();
    
    virtual void execute();
    
private:
    WvLog log;
    
public:
    const char *wstype() const { return "WvSubProcQueueStream"; }
};

#endif // __WVSUBPROCQUEUESTREAM_H
