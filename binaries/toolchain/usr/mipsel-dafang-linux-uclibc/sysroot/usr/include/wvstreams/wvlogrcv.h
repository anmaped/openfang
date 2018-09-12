/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Enhanced "Log Receiver" classes for WvLog.
 * 
 * WvLogRcv-derived classes support automatic formatting of log lines with
 * a prefix, removing control characters, and so on.
 * 
 * WvLogRcv supports partial- and multiple-line log messages.  For example,
 *        log.print("test ");
 *        log.print("string\nfoo");
 * will print:
 *        appname(lvl): test string
 *        appname(lvl): foo
 */
#ifndef __WVLOGRCV_H
#define __WVLOGRCV_H

#include "wvlog.h"
#include "wvfdstream.h"
#include "wvscatterhash.h"

/**
 * WvLogRcv adds some intelligence to WvLogRcvBase, to keep
 * track of line-prefix-printing and other formatting information.
 */
class WvLogRcv : public WvLogRcvBase
{
protected:  
    WvString last_source;
    WvLog::LogLevel max_level, last_level;
    time_t last_time;
    bool at_newline;
    WvString prefix;
    size_t prelen;
    
    class Src_Lvl
    {
    public:
	WvString src;
	WvLog::LogLevel lvl;
	Src_Lvl(WvString _src, int _lvl) : src(_src),
	lvl((WvLog::LogLevel)_lvl) {};
    };
    
    DeclareWvScatterDict(Src_Lvl, WvString, src);
    
    Src_LvlDict custom_levels;
    
    /** Set the Prefix and Prefix Length (size_t prelen) */
    virtual void _make_prefix(time_t now);
    
    /** Start a new log line (print prefix) */
    virtual void _begin_line();
    
    /** End this (Guaranteed NonEmpty) log line */
    virtual void _end_line();
    
    /**
     * add text to the current log line.  'str' may contain only
     * one '\n' optional character at str[len-1] (the end); if it does,
     * end_line will be called immediately after this function.
     */
    virtual void _mid_line(const char *str, size_t len) = 0;
    
private:
    void begin_line()
        { if (!at_newline) return; _begin_line(); at_newline = false; }
    void mid_line(const char *str, size_t len)
        { _mid_line(str, len); 
	    if (len>0 && str[len-1] == '\n') at_newline = true; }
    
public:
    virtual void log(WvStringParm source, int loglevel,
		     const char *_buf, size_t len);
    
    static const char *loglevels[WvLog::NUM_LOGLEVELS];
    
    WvLogRcv(WvLog::LogLevel _max_level = WvLog::NUM_LOGLEVELS);
    virtual ~WvLogRcv();
    
    void end_line()
        { if (at_newline) return;
	    _mid_line("\n", 1); _end_line(); at_newline = true; };
    
    WvLog::LogLevel level() const
        { return max_level; }
    void level(WvLog::LogLevel lvl)
        { max_level = lvl; }
    
    /*
     * Allows you to override debug levels for specific sources
     *  example: mysql=9,Service Manager=9
     *  will get all the debug output from all the sources that
     *  contain (case insensitively) "mysql" or "Service Manager"
     *  in the source name regardless of the current debug level.
     */
    bool set_custom_levels(WvString descr);
};


/**
 * Captures formatted log messages and outputs them to the
 * specified file descriptor.
 */
class WvLogConsole : public WvFDStream, public WvLogRcv
{
    public:
        WvLogConsole(int _fd,
                WvLog::LogLevel _max_level = WvLog::NUM_LOGLEVELS);
        virtual ~WvLogConsole();
    protected:
        virtual void _mid_line(const char *str, size_t len);
};

#endif // __WVLOGRCV_H
