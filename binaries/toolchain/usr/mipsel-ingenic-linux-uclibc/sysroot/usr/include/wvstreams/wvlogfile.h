/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A "Log Receiver" that logs messages to a file 
 */

#ifndef __WVLOGFILE_H
#define __WVLOGFILE_H

#include "wvfile.h"
#include "wvlogrcv.h"

/// Basic WvLogRcv that logs to a file. Always logs to the same file.
/// No auto-rotation of log files.
class WvLogFileBase : public WvLogRcv, public WvFile
{
public:
    WvLogFileBase(WvStringParm _filename,
		  WvLog::LogLevel _max_level = WvLog::NUM_LOGLEVELS);
    
    // run fsync() every so many log messages.  0 never fsyncs.
    int fsync_every;

protected:
    WvLogFileBase(WvLog::LogLevel _max_level);
    virtual void _make_prefix(time_t now_sec); 
    virtual void _mid_line(const char *str, size_t len);
    virtual void _end_line();

    int fsync_count;
};


/// A more advanced WvLogFileBase.  Logs to a file named <filename>.<date>.
/// Deletes old log files after 'keep_for' days.
class WvLogFile : public WvLogFileBase
{
public:
    WvLogFile(WvStringParm _filename,
	      WvLog::LogLevel _max_level = WvLog::NUM_LOGLEVELS,
	      int _keep_for = 7, bool _force_new_line = false,
              bool _allow_append = true);
    
    WvString start_log();

private:
    virtual void _make_prefix(time_t now_sec); 
    int keep_for, last_day;
    WvString filename;
    bool allow_append;

public:
    const char *wstype() const { return "WvLogFileBase"; }
};

#endif // __WVLOGFILE_H
