/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A generic data-logger class with support for multiple receivers.  If
 * no WvLogRcv objects have been created (see wvlogrcv.h) the default is
 * to log to stderr.
 * 
 * WvLog supports partial- and multiple-line log messages.  For example,
 *        log.print("test ");
 *        log.print("string\nfoo");
 * will print:
 *        appname(lvl): test string
 *        appname(lvl): foo
 */
#ifndef __WVLOG_H
#define __WVLOG_H

#include "wvstream.h"
#include <errno.h>
#ifdef _WIN32
typedef int pid_t;
#endif

class WvLog;

// a WvLogRcv registers itself with WvLog and prints, captures,
// or transmits log messages.
class WvLogRcvBase
{
    friend class WvLog;
protected:
    const char *appname(WvStringParm log) const;
    virtual void log(WvStringParm source, int loglevel,
		     const char *_buf, size_t len) = 0;

private:
    static void cleanup_on_fork(pid_t p);
    static void static_init();

public:
    bool force_new_line;
    WvLogRcvBase();
    virtual ~WvLogRcvBase();
};


DeclareWvList(WvLogRcvBase);

typedef wv::function<WvString(WvStringParm)> WvLogFilter;

/**
 * A WvLog stream accepts log messages from applications and forwards them
 * to all registered WvLogRcv's.
 */
class WvLog : public WvStream
{
    friend class WvLogRcvBase;
public:
    enum LogLevel {
	Critical = 0,
	Error,
	Warning,
	Notice,
	Info,
	Debug, Debug1=Debug,
	Debug2,
	Debug3,
	Debug4,
	Debug5,
	
	NUM_LOGLEVELS
    };
    WvString app;

protected:
    LogLevel loglevel;
    static WvLogRcvBaseList *receivers;
    static int num_receivers, num_logs;
    static WvLogRcvBase *default_receiver;
    WvLogFilter* filter;

public:
    WvLog(WvStringParm _app, LogLevel _loglevel = Info,  
            WvLogFilter* filter = 0);
    WvLog(const WvLog &l);
    virtual ~WvLog();
    
    /** fd==-1, but this stream is always ok */
    virtual bool isok() const;
    
    /* always writable */
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);

    /**
     * change the loglevel.  This returns the object again, so you can
     * make convenient statements like log.lvl(WvLog::Warning).print(...)
     */
    WvLog &lvl(LogLevel _loglevel)
        { loglevel = _loglevel; return *this; }
    
    /** change the loglevel and then print a message. */
    size_t operator() (LogLevel _loglevel, WvStringParm s)
    { 
	LogLevel l = loglevel; 
	size_t x = lvl(_loglevel).write(filter ? (*filter)(s) : s);
	lvl(l);
	return x;
    }
    
    /** change the loglevel and then print a formatted message */
    size_t operator() (LogLevel _loglevel, WVSTRING_FORMAT_DECL)
    { 
	LogLevel l = loglevel;
        size_t x;
        if (filter)
            x = lvl(_loglevel).print((*filter)(WvString(WVSTRING_FORMAT_CALL)));
        else
            x = lvl(_loglevel).print(WVSTRING_FORMAT_CALL);
	lvl(l);
	return x;
    }
    
    /**
     * although these appear in WvStream, they need to be re-listed here
     * since the above operator()s caused them to be hidden
     */
    size_t operator() (WvStringParm s)
        { return WvStream::operator()(filter ? (*filter)(s) : s); }
    size_t operator() (WVSTRING_FORMAT_DECL)
        { return (filter ? 
            WvStream::operator()((*filter)(WvString(WVSTRING_FORMAT_CALL))) :
            WvStream::operator()(WVSTRING_FORMAT_CALL) );
        }
    
    /**
     * split off a new WvLog object with the requested loglevel.  This way
     * you can have log at two or more levels without having to retype
     * log.lvl(WvLog::blahblah) all the time.
     */
    WvLog split(LogLevel _loglevel) const
        { return WvLog(app, _loglevel, filter); }
    
    /**
     * we override the unbuffered write function, so lines also include the
     * application and log level.
     */
    virtual size_t uwrite(const void *buf, size_t len);
    
    /** a useful substitute for the normal C perror() function */
    void perror(WvStringParm s)
        { print("%s: %s\n", s, strerror(errno)); }

public:
    const char *wstype() const { return "WvLog"; }
};


#endif // __WVLOG_H
