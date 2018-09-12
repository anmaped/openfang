/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * The basic streaming I/O interface.
 */ 
#ifndef __IWVSTREAM_H
#define __IWVSTREAM_H

#include "wvbuf.h"
#include "wverror.h"
#include "wvtr1.h"
#include "wvxplc.h"


class WvAddr;
class WvStream;


/* The stream gets passed back as a parameter. */
typedef wv::function<void()> IWvStreamCallback;
typedef unsigned int WSID;

class IWvStream : public WvErrorBase, public IObject
{
public:
    static IWvStream *create(WvStringParm moniker, IObject *obj = NULL);
    
    /**
     * A SelectRequest is a convenient way to remember what we want to do
     * to a particular stream: read from it, write to it, or check for
     * exceptions.
     */
    struct SelectRequest {
	bool readable, writable, isexception;
	
	SelectRequest() { }
	SelectRequest(bool r, bool w, bool x = false)
		{ readable = r; writable = w; isexception = x; }
	
	SelectRequest &operator |= (const SelectRequest &r)
		{ readable |= r.readable; writable |= r.writable;
		    isexception |= r.isexception; return *this; }
    };
    
    /**
     * the data structure used by pre_select()/post_select() and internally
     * by select().
     */
    struct SelectInfo {
	fd_set read, write, except; // set by pre_select, read by post_select
	SelectRequest wants;        // what is the user looking for?
	int max_fd;                 // largest fd in read, write, or except
	time_t msec_timeout;        // max time to wait, or -1 for forever
	bool inherit_request;       // 'wants' values passed to child streams
	bool global_sure;           // should we run the globalstream callback
    };
    
    IWvStream();
    virtual ~IWvStream();
    virtual void close() = 0;
    virtual bool isok() const = 0;
    virtual void callback() = 0;
    
    // FIXME: these really have no place in the interface...
    virtual int getrfd() const = 0;
    virtual int getwfd() const = 0;

    // FIXME: evil, should go away (or be changed to localaddr/remoteaddr)
    virtual const WvAddr *src() const = 0;
    
    // needed for select().
    // Some say that pre_select() should go away.
    virtual void pre_select(SelectInfo &si) = 0;
    virtual bool post_select(SelectInfo &si) = 0;
    
    // these are now the official way to get/put data to your stream.
    // The old uread() and uwrite() are just implementation details!
    virtual size_t read(void *buf, size_t count) = 0;
    virtual size_t write(const void *buf, size_t count) = 0;

    // FIXME: these are the new fancy versions, but WvBuf needs to have
    // a safely abstracted interface class (IWvBuf) before IWvStream will
    // really be safe, if we include these.
    virtual size_t read(WvBuf &outbuf, size_t count) = 0;
    virtual size_t write(WvBuf &inbuf, size_t count = INT_MAX) = 0;

    /**
     * Shuts down the reading side of the stream.  This is the opposite
     * of nowrite(), but the name is actually slightly misleading; subsequent
     * calls to read() *might not* fail; rather, if the other end of the
     * connection tries to write to us, they should fail.
     *
     * After noread(), if the read buffer (if any) is empty once, we promise
     * that it will never refill.
     * 
     * If you call both noread() and nowrite(), then the stream does close()
     * automatically once all buffers are empty.
     */
    virtual void noread() = 0;

    /**
     * Shuts down the writing side of the stream.
     * Subsequent calls to write() will fail.  But if there's data in the
     * output buffer, it will still be flushed.
     * 
     * If you call both noread() and nowrite(), then the stream does close()
     * automatically once all buffers are empty.
     */
    virtual void nowrite() = 0;
    
    /**
     * Auto-close the stream if the time is right.  If noread() and nowrite()
     * and all buffers are empty, then we can probably close.
     */
    virtual void maybe_autoclose() = 0;
    
    /** Returns true if the stream is readable. */
    virtual bool isreadable() = 0;
    
    /** Returns true if the stream is writable (without using the outbuf). */
    virtual bool iswritable() = 0;
    
    /**
     * flush the output buffer, if we can do it without delaying more than
     * msec_timeout milliseconds at a time.  (-1 means wait forever)
     * 
     * Returns true if it finished flushing (ie. the outbuf is empty).
     * 
     * FIXME: Something like this probably belongs in IWvStream, but
     * probably not exactly this.
     */
    virtual bool flush(time_t msec_timeout) = 0;

    /**
     * Returns true if we want to flush the output buffer right now.  This
     * allows us to implement delayed_flush(), flush_then_close(), etc, but
     * it's still super-ugly and probably needs to go away.  (In fact, all
     * our buffer flushing is super-ugly right now.)
     */
    virtual bool should_flush() = 0;

    /*
     * WARNING: these don't work as expected!
     */
    /** Sets a callback to be invoked when the stream is readable. */
    virtual IWvStreamCallback setreadcallback(IWvStreamCallback _callfunc) = 0;

    /** Sets a callback to be invoked when the stream is writable. */
    virtual IWvStreamCallback setwritecallback(IWvStreamCallback _callfunc) = 0;

    /** Sets a callback to be invoked when the stream is in exception
     * state. */
    virtual IWvStreamCallback setexceptcallback(IWvStreamCallback _callfunc) = 0;
    /* 
     * END WARNING
     */

    /** Sets a callback to be invoked on close().  */
    virtual IWvStreamCallback setclosecallback(IWvStreamCallback _callfunc) = 0;

    /* Stream identification/debugging */
    virtual const char *wsname() const = 0;
    virtual void set_wsname(WvStringParm name) = 0;
    virtual const char *wstype() const = 0; // This is not static due to, eg, WvStreamClone
    virtual WSID wsid() const = 0;

    /**
     ** set the maximum size of outbuf, beyond which a call to write() will
     ** return 0.  
     **/

     virtual void outbuf_limit(size_t size) = 0;
     virtual WvString getattr(WvStringParm name) const = 0;
};

DEFINE_IID(IWvStream, {0x7ca76e98, 0xb653, 0x43d7,
    {0xb0, 0x56, 0x8b, 0x9d, 0xde, 0x9a, 0xbe, 0x9d}});


#endif /* __IWVSTREAM_H */
