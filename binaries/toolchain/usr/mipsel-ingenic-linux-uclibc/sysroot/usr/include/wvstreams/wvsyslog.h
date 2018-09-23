/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */ 
#ifndef __WVSYSLOG_H
#define __WVSYSLOG_H

#include "wvlogrcv.h"

/**
 * WvSyslog is a descendant of WvLogRcv that sends messages to the syslogd
 * daemon.
 */
class WvSyslog : public WvLogRcv
{
public:
    WvSyslog(WvStringParm _prefix, bool _include_appname,
	     WvLog::LogLevel _first_debug = WvLog::Debug,
	     WvLog::LogLevel _max_level = WvLog::NUM_LOGLEVELS);
    virtual ~WvSyslog();

protected:
    WvLog::LogLevel first_debug;
    WvDynBuf current;
    WvString syslog_prefix;
    bool include_appname;
    
    virtual void _begin_line();
    virtual void _mid_line(const char *str, size_t len);
    virtual void _end_line();
};

#endif // __WVLOGBUFFER_H
