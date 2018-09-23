/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */ 
#ifndef __WVLOGBUFFER_H
#define __WVLOGBUFFER_H

#include "wvlogrcv.h"
#include "wvhashtable.h"

/**
 * WvLogBuffer is a descendant of WvLogRcv that buffers log messages for
 * later use.  It only keeps up to max_lines log entries for every 
 * source/debug level, s.t. debug level <= max_level 
 */
class WvLogBuffer : public WvLogRcv
{
public:
    // An actual message
    class Msg
    {
        public:
            time_t timestamp;
            WvLog::LogLevel level;
            WvString source, message; 

            Msg(WvLog::LogLevel _level, WvStringParm _source, WvString _message);
    };

    DeclareWvList(Msg); 
    
    /*
     * Maps a string describing msg type of the form "source:level"
     * to a list of pointers to all messages of this type
     */
    class MsgCounter
    {
        public:
            MsgCounter(WvString _src_lvl) : src_lvl(_src_lvl) {};
            Msg* add(Msg* msg, int max);
            WvString src_lvl;
        private:
            MsgList list;
    };
    
    DeclareWvDict(MsgCounter, WvString, src_lvl);
    
protected:
    /*
     * "Owns" all the msges
     */
    MsgList msgs;
    /*
     * Used for keeping track of how many messages from a given source/level
     *      there are. Just stores the pointers.
     */
    MsgCounterDict counters;
    WvDynBuf current;
    int max_lines;

    void handle_msg(Msg *lastmsg);
    
    virtual void _begin_line() {};
    virtual void _mid_line(const char *str, size_t len);
    virtual void _end_line();

public:
    WvLogBuffer(int _max_lines, 
		WvLog::LogLevel _max_level = WvLog::NUM_LOGLEVELS);
    virtual ~WvLogBuffer();

    MsgList &messages()
    	{ end_line(); return msgs; }

    void feed_receiver(WvLogRcv& receiver);

    void dump(WvStream &s);
};

#endif // __WVLOGBUFFER_H
