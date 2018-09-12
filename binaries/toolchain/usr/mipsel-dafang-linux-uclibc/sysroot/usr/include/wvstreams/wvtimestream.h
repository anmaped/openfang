/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */ 
#ifndef __WVTIMESTREAM_H
#define __WVTIMESTREAM_H

#include "wvtimeutils.h"
#include "wvstream.h"

/**
 * WvTimeStream causes select() to be true after a configurable number
 * of milliseconds.  Because programs using WvStream make no guarantees
 * about how often select() will be called, WvTimeStream tries to adjust
 * its timing to a correct _average_ number of milliseconds per tick.
 * 
 * For example, if ms_per_tick=100, WvTimeStream will tick 10 times in one
 * second.  However, there may be a few milliseconds of difference
 * ("jitter") for each individual tick, due to random system delays.
 */
class WvTimeStream : public WvStream
{
    WvTime last;
    WvTime next;
    time_t ms_per_tick;

public:
    WvTimeStream();
    
    /**
     * Every 'msec' milliseconds, select() will return true on this
     * stream.  if 'msec' is 0 (or less), the timer is disabled.
     *
     * Be careful when mixing alarm() with this. You can know whether
     * it was the alarm or if you had a timer event by looking at
     * alarm_was_ticking. But the alarm() has priority, so if there's
     * always an alarm, the timer event never gets to run. Calling
     * alarm(0) in the callback unconditionally would thus be a bad
     * idea, or even with an unsuitably small number (say, less than
     * the time it takes to go back into select()). So don't do it.
     */
    void set_timer(time_t msec);

    virtual bool isok() const;
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
    virtual void execute();

public:
    const char *wstype() const { return "WvTimeStream"; }
};


#endif // __WVTIMESTREAM_H
