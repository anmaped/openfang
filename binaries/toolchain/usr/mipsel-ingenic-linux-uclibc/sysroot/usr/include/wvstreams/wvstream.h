/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * Provides basic streaming I/O support.
 */ 
#ifndef __WVSTREAM_H
#define __WVSTREAM_H

#include "iwvstream.h"
#include "wvtimeutils.h"
#include "wvstreamsdebugger.h"
#include <errno.h>
#include <limits.h>
#include "wvattrs.h"

/**
 * Unified support for streams, that is, sequences of bytes that may or
 * may not be ready for read/write at any given time.
 * 
 * We provide typical read and write routines, as well as a select() function
 * for each stream.
 */
class WvStream: public IWvStream
{
    IMPLEMENT_IOBJECT(WvStream);

    WvString my_wsname;
    WSID my_wsid;
    WvAttrs attrs;
public:
    /**
     * If this is set, select() doesn't return true for read unless the
     * given stream also returns true for write.
     */
    WvStream *read_requires_writable;

    /**
     * If this is set, select() doesn't return true for write unless the
     * given stream also returns true for read.
     */
    WvStream *write_requires_readable;
    
    /** If this is set, enables the use of continue_select(). */
    bool uses_continue_select;

    /** Specifies the stack size to reserve for continue_select(). */
    size_t personal_stack_size;

    /**
     * This will be true during callback execution if the
     * callback was triggered by the alarm going off.
     */
    bool alarm_was_ticking;
    
    /** True if noread()/nowrite()/close() have been called, respectively. */
    bool stop_read, stop_write, closed;
    
    /** Basic constructor for just a do-nothing WvStream */
    WvStream();
    virtual ~WvStream();

    /**
     * Close the stream if it is open; isok() becomes false from now on.
     * Note!!  If you override this function in a derived class, you must
     *   call it yourself from your destructor.  WvStream::~WvStream()
     *   can only call WvStream::close() because of the way virtual
     *   functions work in C++.
     */ 
    virtual void close();

    /** Override seterr() from WvError so that it auto-closes the stream. */
    virtual void seterr(int _errnum);
    void seterr(WvStringParm specialerr)
        { WvErrorBase::seterr(specialerr); }
    void seterr(WVSTRING_FORMAT_DECL)
        { seterr(WvString(WVSTRING_FORMAT_CALL)); }
    
    /** return true if the stream is actually usable right now */
    virtual bool isok() const;
    
    /** read a data block on the stream.  Returns the actual amount read. */
    virtual size_t read(void *buf, size_t count);

    /**
     * Reads up to 'count' bytes of data from the stream into the buffer.
     * Returns the actual amount read.
     *
     * If 'count' is greater than the amount of free space available
     * in the buffer, only reads at most that amount.  You should
     * specify a reasonable upper bound on how much data should
     * be read at once.
     */
    virtual size_t read(WvBuf &outbuf, size_t count);

    /** 
     * Puts data back into the stream's internal buffer.  We cheat so that
     * there's no restriction on how much (or what) data can be unread().
     * This is different from WvBuf::unget() (which is rather restrictive).
     */
    virtual void unread(WvBuf &outbuf, size_t count);

    /**
     * Write data to the stream.  Returns the actual amount written.
     * Since WvStream has an output buffer, it *always* successfully "writes"
     * the full amount (but you might have to flush the buffers later so it
     * actually gets sent).
     */
    virtual size_t write(const void *buf, size_t count);

    /**
     * Writes data to the stream from the given buffer.
     * Returns the actual amount written.
     *
     * If count is greater than the amount of data available in
     * the buffer, only writes at most that amount.
     */
    virtual size_t write(WvBuf &inbuf, size_t count = INT_MAX);

    /**
     * set the maximum size of outbuf, beyond which a call to write() will
     * return 0.  I need to do this for tape backups, since all I can do
     * is write to the loopback as fast as I can, which causes us to run 
     * out of memory and get SIGABRT'd.  (dcoombs: 12/15/2000)
     * 
     * FIXME: there must be a better way.  This confuses the semantics of
     * write(); can you trust it to always write all the bytes, or not?
     */
    void outbuf_limit(size_t size)
        { max_outbuf_size = size; }

    virtual void noread();
    virtual void nowrite();
    virtual void maybe_autoclose();
    
    virtual bool isreadable();
    virtual bool iswritable();
    
    /**
     * unbuffered I/O functions; these ignore the buffer, which is
     * handled by read().  Don't call these functions explicitly unless
     * you have a _really_ good reason.
     * 
     * This is what you would override in a derived class.
     */ 
    virtual size_t uread(void *buf, size_t count)
        { return 0; /* basic WvStream doesn't actually do anything! */ }

    /**
     * unbuffered I/O functions; these ignore the buffer, which is
     * handled by write().  Don't call these functions explicitly unless
     * you have a _really_ good reason.
     * 
     * This is what you would override in a derived class.
     */ 
    virtual size_t uwrite(const void *buf, size_t count)
        { return count; /* basic WvStream doesn't actually do anything! */ }

    /**
     * Read up to one line of data from the stream and return a
     * pointer to the internal buffer containing this line.  If the
     * end-of-line 'separator' is encountered, it is removed from the
     * string.  If there is not a full line available, returns
     * NULL. You can read what we have so far by calling read().
     *
     * Readahead specifies the maximum amount of data that the stream
     * is allowed to read in one shot.
     *
     * It is expected that there will be no NULL characters on the
     * line.
     *
     * wait_msec is provided so that legacy code does not break.  But
     * it really should be 0.
     */
    char *getline(time_t wait_msec = 0,
		  char separator = '\n', int readahead = 1024)
    {
	return blocking_getline(wait_msec, separator, readahead);
    }

    /** Auto-convert int to time_t. */
    char *getline(int wait_msec,
		  char separator = '\n', int readahead = 1024)
    {
	return getline(time_t(wait_msec), separator, readahead);
    }

    /** Auto-convert double to time_t. */
    char *getline(double wait_msec,
		  char separator = '\n', int readahead = 1024)
    {
	return getline(time_t(wait_msec), separator, readahead);
    }

private:
    /** We will prohibit someone from calling getline with a char or
     * bool as the first parameter.  This will attempt to detect dumb
     * mistakes.
     */
    char *getline(char, int i = 0);
    char *getline(bool, int i = 0);
public:

    /**
     * This is a version of getline() that allows you to block for
     * more data to arrive.
     *
     * This should be used carefully, as blocking is generally
     * unexpected in WvStreams programs.
     *
     * If wait_msec < 0, it will wait forever for the 'separator'
     * (often a bad idea!).  If wait_msed == 0, this is the equivalent
     * of getline().
     */
    char *blocking_getline(time_t wait_msec, int separator = '\n',
			   int readahead = 1024);

    /**
     * This is a version of blocking_getline() that uses
     * continue_select to avoid blocking other streams.
     */
    char *continue_getline(time_t wait_msec, int separator = '\n',
			   int readahead = 1024);

    /**
     * force read() to not return any bytes unless 'count' bytes can be
     * read at once.  (Useful for processing Content-Length headers, etc.)
     * Use count==0 to disable this feature.
     * 
     * WARNING: getline() sets queuemin to 0 automatically!
     */ 
    void queuemin(size_t count)
        { queue_min = count; }

    /**
     * drain the input buffer (read and discard data until select(0)
     * returns false)
     */
    void drain();
    
    /**
     * force write() to always buffer output.  This can be more efficient
     * if you write a lot of small segments and want to "coagulate" them
     * automatically.  To flush the output buffer, use flush() or select().
     */ 
    void delay_output(bool is_delayed)
    {
        outbuf_delayed_flush = is_delayed;
        want_to_flush = !is_delayed;
    }

    /**
     * if true, force write() to call flush() each time, the default behavour.
     * otherwise, flush() is granted special meaning when explicitly invoked
     * by the client and write() may empty the output buffer, but will not
     * explicitly flush().
     */
    void auto_flush(bool is_automatic)
        { is_auto_flush = is_automatic; }

    /**
     * flush the output buffer, if we can do it without delaying more than
     * msec_timeout milliseconds at a time.  (-1 means wait forever)
     * 
     * Returns true if the flushing finished (the output buffer is empty).
     */
    virtual bool flush(time_t msec_timeout);

    virtual bool should_flush();

    /**
     * flush the output buffer automatically as select() is called.  If
     * the buffer empties, close the stream.  If msec_timeout seconds pass,
     * close the stream.  After the stream closes, it will become !isok()
     * (and a WvStreamList can delete it automatically)
     */ 
    void flush_then_close(int msec_timeout);
    
    /**
     * pre_select() sets up for eventually calling ::select().
     * It adds the right fds to the read, write, and except lists in the
     * SelectInfo struct.
     * 
     * Returns true if we already know this stream is ready, and there's no
     * need to actually do a real ::select().  Some streams, such as timers,
     * can be implemented by _only_ either returning true or false here after
     * doing a calculation, and never actually adding anything to the
     * SelectInfo.
     * 
     * You can add your stream to any of the lists even if readable,
     * writable, or isexception isn't set.  This is what force_select()
     * does.  You can also choose not to add yourself to the list if you know
     * it would be useless right now.
     * 
     * pre_select() is only called if isok() is true.
     * 
     * pre_select() is allowed to reduce msec_timeout (or change it if it's
     * -1).  However, it's not allowed to _increase_ msec_timeout.
     */ 
    virtual void pre_select(SelectInfo &si);
    
    /**
     * A more convenient version of pre_select() usable for overriding the
     * 'want' value temporarily.
     */
    void pre_select(SelectInfo &si, const SelectRequest &r)
    {
	SelectRequest oldwant = si.wants;
	si.wants = r;
	pre_select(si);
	si.wants = oldwant;
    }
    
    /**
     * Like pre_select(), but still exists even if you override the other
     * pre_select() in a subclass.  Sigh.
     */
    void xpre_select(SelectInfo &si, const SelectRequest &r)
        { pre_select(si, r); }
    
    /**
     * post_select() is called after ::select(), and returns true if this
     * object is now ready.  Usually this is done by checking for this object
     * in the read, write, and except lists in the SelectInfo structure.  If
     * you want to do it in some other way, you should usually do it in
     * pre_select() instead.
     * 
     * You may also want to do extra maintenance functions here; for example,
     * the standard WvStream::post_select tries to flush outbuf if it's
     * nonempty.  WvTCPConn might retry connect() if it's waiting for a
     * connection to be established.
     */
    virtual bool post_select(SelectInfo &si);

    /**
     * Like post_select(), but still exists even if you override the other
     * post_select() in a subclass.  Sigh.
     */
    bool xpost_select(SelectInfo &si, const SelectRequest &r)
        { return post_select(si, r); }
    
    /**
     * A more convenient version of post_select() usable for overriding the
     * 'want' value temporarily.
     */
    bool post_select(SelectInfo &si, const SelectRequest &r)
    {
	SelectRequest oldwant = si.wants;
	si.wants = r;
	bool val = post_select(si);
	si.wants = oldwant;
	return val;
    }
    
    /**
     * Return true if any of the requested features are true on the stream.
     * If msec_timeout < 0, waits forever (bad idea!).  ==0, does not wait.
     * Otherwise, waits for up to msec_timeout milliseconds.
     * 
     * **NOTE**
     *   select() is _not_ virtual!  To change the select() behaviour
     *   of a stream, override the pre_select() and/or post_select()
     *   functions.
     * 
     * This version of select() sets forceable==true, so force_select
     * options are taken into account.
     * 
     * You almost always use this version of select() with callbacks, like
     * this:  if (stream.select(1000)) stream.callback();
     * 
     * If you want to read/write the stream in question, try using the other
     * variant of select().
     * 
     * DEPRECATED.  Call runonce() instead.
     */
    bool select(time_t msec_timeout)
        { return _select(msec_timeout, false, false, false, true); }
    
    /**
     * Exactly the same as:
     *     if (select(timeout)) callback();
     * 
     * ...except that the above is deprecated, because it assumes callbacks
     * aren't called automatically and that the return value of one-parameter
     * select() is actually meaningful.
     * 
     * Update your main loop to call runonce() instead of the above.
     * 
     * Almost all modern programs should use msec_timeout = -1.
     */
    void runonce(time_t msec_timeout = -1)
        { if (select(msec_timeout)) callback(); }
    
     /**
      * This version of select() sets forceable==false, so we use the exact
      * readable/writable/isexception options provided.
      * 
      * You normally use this variant of select() when deciding whether you
      * should read/write a particular stream.  For example:
      * 
      *     if (stream.select(1000, true, false))
      *             len = stream.read(buf, sizeof(buf));
      * 
      * This variant of select() is probably not what you want with
      * most WvStreamLists, unless you know exactly what you're doing.
      * 
      * WARNING: the difference between the one-parameter and multi-parameter
      * versions of select() is *incredibly* confusing.  Make sure you use the
      * right one!
      * 
      * DEPRECATED.  Call isreadable() or iswritable() instead, if
      * msec_timeout was going to be zero.  Other values of msec_timeout are
      * not really recommended anyway.
      */
    bool select(time_t msec_timeout,
		bool readable, bool writable, bool isex = false)
        { return _select(msec_timeout, readable, writable, isex, false); }

    /**
     * Use get_select_request() to save the current state of the
     * selection state of this stream.  That way, you can call
     * force_select() and undo_force_select() to restore this properly.
     */
    IWvStream::SelectRequest get_select_request();

    /**
     * Use force_select() to force one or more particular modes (readable,
     * writable, or isexception) to true when selecting on this stream.
     * 
     * If an option is set 'true', we will select on that option when someone
     * does a select().  If it's set 'false', we don't change its force
     * status.  (To de-force something, use undo_force_select().)
     */
    void force_select(bool readable, bool writable, bool isexception = false);
    
    /**
     * Undo a previous force_select() - ie. un-forces the options which
     * are 'true', and leaves the false ones alone.
     */
    void undo_force_select(bool readable, bool writable,
			   bool isexception = false);
    
    /**
     * return to the caller from execute(), but don't really return exactly;
     * this uses WvCont::yield() to return to the caller of callback()
     * without losing our place in execute() itself.  So, next time someone
     * calls callback(), it will be as if continue_select() returned.
     * 
     * NOTE: execute() will won't be called recursively this way, but any
     * other member function might get called, or member variables changed,
     * or the state of the world updated while continue_select() runs.  Don't
     * assume that nothing has changed after a call to continue_select().
     * 
     * NOTE 2: if you're going to call continue_select(), you should set
     * uses_continue_select=true before the first call to callback().
     * Otherwise your WvCont won't get created.
     * 
     * NOTE 3: if msec_timeout >= 0, this uses WvStream::alarm().
     */
    bool continue_select(time_t msec_timeout);
    
    /**
     * you MUST run this from your destructor if you use continue_select(), or
     * very weird things will happen if someone deletes your object while in
     * continue_select().
     */
    void terminate_continue_select();

    /**
     * get the remote address from which the last data block was received.
     * May be NULL.  The pointer becomes invalid upon the next call to read().
     */
    virtual const WvAddr *src() const;
    
    /**
     * define the callback function for this stream, called whenever
     * the callback() member is run, and passed the 'userdata' pointer.
     */
    void setcallback(IWvStreamCallback _callfunc);
        
    /** Sets a callback to be invoked when the stream is readable. */
    IWvStreamCallback setreadcallback(IWvStreamCallback _callback);

    /** Sets a callback to be invoked when the stream is writable. */
    IWvStreamCallback setwritecallback(IWvStreamCallback _callback);

    /** Sets a callback to be invoked when the stream is in exception
     * state. */
    IWvStreamCallback setexceptcallback(IWvStreamCallback _callback);

    /** Sets a callback to be invoked on close().  */
    IWvStreamCallback setclosecallback(IWvStreamCallback _callback);

    /**
     * set the callback function for this stream to an internal routine
     * that auto-forwards all incoming stream data to the given output
     * stream.
     */
    void autoforward(WvStream &s);

    /** Stops autoforwarding. */
    void noautoforward();
    static void autoforward_callback(WvStream &input, WvStream &output);
    
    /**
     * A wrapper that's compatible with WvCont, but calls the "real" callback.
     */
    void *_callwrap(void *);
    
    /**
     * Actually call the registered callfunc and execute().
     */
    void _callback();
    
    /**
     * if the stream has a callback function defined, call it now.
     * otherwise call execute().
     */
    virtual void callback();
    
    /**
     * set an alarm, ie. select() will return true after this many ms.
     * The alarm is cleared when callback() is called.
     */
    void alarm(time_t msec_timeout);

    /**
     * return the number of milliseconds remaining before the alarm will go
     * off; -1 means no alarm is set (infinity), 0 means the alarm has
     * been hit and will be cleared by the next callback().
     */
    time_t alarm_remaining();

    /**
     * print a preformatted WvString to the stream.
     * see the simple version of write() way up above.
     */
    size_t write(WvStringParm s)
        { return write(s.cstr(), s.len()); }
    size_t print(WvStringParm s)
        { return write(s); }
    size_t operator() (WvStringParm s)
        { return write(s); }

    /** preformat and write() a string. */
    size_t print(WVSTRING_FORMAT_DECL)
	{ return write(WvString(WVSTRING_FORMAT_CALL)); }
    size_t operator() (WVSTRING_FORMAT_DECL)
        { return write(WvString(WVSTRING_FORMAT_CALL)); }

    const char *wsname() const
        { return my_wsname; }
    void set_wsname(WvStringParm wsname)
        { my_wsname = wsname; }
    void set_wsname(WVSTRING_FORMAT_DECL)
        { set_wsname(WvString(WVSTRING_FORMAT_CALL)); }
        
    const char *wstype() const { return "WvStream"; }
    
    WSID wsid() const { return my_wsid; }
    static IWvStream *find_by_wsid(WSID wsid);

    virtual WvString getattr(WvStringParm name) const
	{ return attrs.get(name); }

    // ridiculous hackery for now so that the wvstream unit test can poke
    // around in the insides of WvStream.  Eventually, inbuf will go away
    // from the base WvStream class, so nothing like this will be needed.
#ifdef __WVSTREAM_UNIT_TEST
public:
    size_t outbuf_used() 
        { return outbuf.used(); }
    size_t inbuf_used()
        { return inbuf.used(); }
    void inbuf_putstr(WvStringParm t)
        { inbuf.putstr(t); }
#endif

protected:
    void setattr(WvStringParm name, WvStringParm value)
	{  attrs.set(name, value); }
    // builds the SelectInfo data structure (runs pre_select)
    // returns true if there are callbacks to be dispatched
    //
    // all of the fields are filled in with new values
    // si.msec_timeout contains the time until the next alarm expires
    void _build_selectinfo(SelectInfo &si, time_t msec_timeout,
        bool readable, bool writable, bool isexcept,
        bool forceable);

    // runs the actual select() function over the given
    // SelectInfo data structure, returns the number of descriptors
    // in the set, and sets the error code if a problem occurs
    int _do_select(SelectInfo &si);

    // processes the SelectInfo data structure (runs post_select)
    // returns true if there are callbacks to be dispatched
    bool _process_selectinfo(SelectInfo &si, bool forceable);

    // tries to empty the output buffer if the stream is writable
    // not quite the same as flush() since it merely empties the output
    // buffer asynchronously whereas flush() might have other semantics
    // also handles autoclose (eg. after flush)
    bool flush_outbuf(time_t msec_timeout);

    // called once flush() has emptied outbuf to ensure that any other
    // internal stream buffers actually do get flushed before it returns
    virtual bool flush_internal(time_t msec_timeout);
    
    // the real implementations for these are actually in WvFDStream, which
    // is where they belong.  By IWvStream needs them to exist for now, so
    // it's a hack.  In standard WvStream they return -1.
    virtual int getrfd() const;
    virtual int getwfd() const;
    
    // FIXME: this one is so bad, I'm not touching it. Quick hack to
    // make it work anyway.
    friend class WvHTTPClientProxyStream;

    WvDynBuf inbuf, outbuf;

    IWvStreamCallback callfunc;
    wv::function<void*(void*)> call_ctx;

    IWvStreamCallback readcb, writecb, exceptcb, closecb;

    size_t max_outbuf_size;
    bool outbuf_delayed_flush;
    bool is_auto_flush;

    // Used to guard against excessive flushing when using delay_flush
    bool want_to_flush;

    // Used to ensure we don't flush recursively.
    bool is_flushing;

    size_t queue_min;		// minimum bytes to read()
    time_t autoclose_time;	// close eventually, even if output is queued
    WvTime alarm_time;          // select() returns true at this time
    WvTime last_alarm_check;    // last time we checked the alarm_remaining
    
    /**
     * The callback() function calls execute(), and then calls the user-
     * specified callback if one is defined.  Do not call execute() directly;
     * call callback() instead.
     * 
     * The default execute() function does nothing.
     * 
     * Note: If you override this function in a derived class, you must
     * call the parent execute() yourself from the derived class.
     */
    virtual void execute()
        { }
    
    // every call to select() selects on the globalstream.
    static WvStream *globalstream;

    static void debugger_streams_display_header(WvStringParm cmd,
            WvStreamsDebugger::ResultCallback result_cb);
    static void debugger_streams_display_one_stream(WvStream *s,
            WvStringParm cmd,
            WvStreamsDebugger::ResultCallback result_cb);
    static void debugger_streams_maybe_display_one_stream(WvStream *s,
            WvStringParm cmd,
            const WvStringList &args,
            WvStreamsDebugger::ResultCallback result_cb);
 
private:
    /** The function that does the actual work of select(). */
    bool _select(time_t msec_timeout,
		 bool readable, bool writable, bool isexcept,
		 bool forceable);

    void legacy_callback();

    /** Prevent accidental copying of WvStream.  These don't actually exist. */
    WvStream(const WvStream &s);
    WvStream& operator= (const WvStream &s);
    static void add_debugger_commands();

    static WvString debugger_streams_run_cb(WvStringParm cmd,
        WvStringList &args,
        WvStreamsDebugger::ResultCallback result_cb, void *);
    static WvString debugger_close_run_cb(WvStringParm cmd,
        WvStringList &args,
        WvStreamsDebugger::ResultCallback result_cb, void *);
};

/**
 * Console streams...
 *
 * This can be reassigned while the program is running, if desired,
 * but MUST NOT be NULL.
 */
extern WvStream *wvcon; // tied stdin and stdout stream
extern WvStream *wvin;  // stdin stream
extern WvStream *wvout; // stdout stream
extern WvStream *wverr; // stderr stream

#endif // __WVSTREAM_H
