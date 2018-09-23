/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */ 
#ifndef __WVTIMEOUTSTREAM_H
#define __WVTIMEOUTSTREAM_H

#include "wvstream.h"

/**
 * WvTimeoutStream is a stream that becomes !isok() after a
 * configurable number of milliseconds. It will wake up a select(). It
 * will return true if select()ed and that the timeout has
 * expired. But using it in a WvStreamList will not have it call the
 * callback/execute because the WvStreamList checks whether isok() is
 * true before doing the select().
 */

class WvTimeoutStream: public WvStream
{
    bool ok;

public:
    WvTimeoutStream(time_t msec);
    virtual bool isok() const { return ok; }

    virtual void execute();
    
private:
    const char *wstype() const { return "WvTimeoutStream"; }
};

#endif // __WVTIMEOUTSTREAM_H
