/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A period event stream.
 */

#ifndef __WVDAILYEVENT_H
#define __WVDAILYEVENT_H

#include "wvstream.h"

/**
 * A simple class that can trigger an event on a timed basis.
 * 
 * The period may be specified the following ways:
 * 
 *  - Given an hour, triggers once per day, on that hour.
 *  - Given a number of times per day, triggers that many times
 *     per day, evenly, starting at the specified hour.
 *  - WvDailyEvents can happen up to 1440 times a day, that is once per
 *     minute, no more.
 * 
 * 
 * Presently has a one-hour granularity in the first case, but that
 * can be fixed someday when someone cares.
 * 
 */
class WvDailyEvent : public WvStream
/**********************************/
{
public:
    /**
     * Constructs WvDailyEvent.
     * \param _first_hour the first hour of the day in which the event should 
     * occur.
     * \param _num_per_day the number of times in a day that the event should
     * occur.
     * \param _skip_first indicates whether the first event should be skipped
     * or not, starting on the second is default.
     * If _num_per_day is not specified, it defaults to 0 (which is equivalent
     * to running the event once a day). 
     */
    WvDailyEvent( int _first_hour, int _num_per_day=0, bool _skip_first=true );

    /** Munges SelectInfo such that the stream will select when the
     *  time is right for the event to occur.
     *  "The time is right" means that it is the first hour in some arbitrary 
     *  day that the event should occur or it is the first hour + 
     *  (number-of-minutes-in-a-day mod number of occurrences in a day) minutes
     *  in some arbitrary day that the event should occur.
     */
    virtual void pre_select( SelectInfo& si );
    virtual bool post_select( SelectInfo& si );

    /**
     * Modifies the first hour in which the event should occur and the number of
     * times the event should occur in a day.
     * \param _first_hour the first hour of the day in which the event should 
     * occur.
     * \param _num_per_day the number of times in a day that the event should
     * occur.
     * If _num_per_day is not specified, it defaults to 0 (which is equivalent
     * to running the event once a day). 
     */
    void configure( int _first_hour, int _num_per_day=0, bool _skip_first=true );

    /// Set number of times per day the event should occur - ONLY FOR TESTING!
    void set_num_per_day(int _num_per_day);
    
    /** 
     * Modifies the first hour in which the event should occur and leaves the 
     * number of times per day unmodified.
     */
    void set_hour( int h )
        { configure( h, num_per_day ); }

    /// return the time when the next event will occur
    time_t next_event() const;

private:
    int     first_hour;
    int     num_per_day;
    bool    need_reset;
    bool    skip_first;
    time_t  prev;
    
    time_t  not_until;

public:
    const char *wstype() const { return "WvDailyEvent"; }
};

#endif
