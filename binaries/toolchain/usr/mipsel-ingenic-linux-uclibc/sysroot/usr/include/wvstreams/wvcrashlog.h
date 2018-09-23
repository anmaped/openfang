/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A "Log Receiver" that logs messages to a file 
 */

#ifndef __WVCRASHLOG_H
#define __WVCRASHLOG_H

#include "wvlogrcv.h"

/// WvLogRcv that sticks log messages in the wvcrash_ring_buffer
class WvCrashLog : public WvLogRcv
{
public:
    WvCrashLog(WvLog::LogLevel _max_level = WvLog::NUM_LOGLEVELS);
    
protected:
    virtual void _make_prefix(time_t now_sec); 
    virtual void _mid_line(const char *str, size_t len);
};

#endif
