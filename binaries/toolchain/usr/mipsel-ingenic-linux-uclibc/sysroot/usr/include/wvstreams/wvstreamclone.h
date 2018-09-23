/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * Provides support for constructing streams that wrap other streams.
 * This can turn a (relatively useless) IWvStream into a useful WvStream.
 */ 
#ifndef __WVSTREAMCLONE_H
#define __WVSTREAMCLONE_H

#include "wvstream.h"

/**
 * WvStreamClone simply forwards all requests to the "cloned" stream.
 * 
 * A class derived from WvStreamClone can contain a WvStream as a
 * dynamically allocated data member, but act like the stream itself.
 * 
 * This is useful for classes that need to create/destroy WvPipes
 * while they run, for example, yet do not want users to know about
 * the member variable.
 */
class WvStreamClone : public WvStream
{
public:
    /**
     * Constructs the stream, then calls setclone(_cloned).  See setclone()
     * for important information.
     */
    WvStreamClone(IWvStream *_cloned = NULL);
    
    /**
     * The WvStreamClone destructor.
     * 
     * Note that this calls setclone(NULL), which calls WVRELEASE(cloned).
     * 
     * However, we do *not* explicitly cloned->close().  If the clone is
     * ready to delete itself after the release, then it will close itself
     * anyway; if not, WvStreamClone leaves it alone.  Note that this
     * behaviour was quite different before WvStreams 5.
     */
    virtual ~WvStreamClone();

    IWvStream *cloned;
    
    /**
     * WvStreamClone takes ownership of the given stream; it will WVRELEASE()
     * the stream when you setclone() it to something else or destroy this
     * object.  If this is undesirable, you should addRef() the stream before
     * passing it in here.
     * 
     * Note: setclone() changes the cloned stream's various callbacks.  It
     * removes the callbacks from the old stream when you setclone() to
     * something else.  This means it is not safe to clone the same stream
     * more than once at a time, but it is safe to un-clone and then re-clone
     * a stream (as long as you watch its reference count).
     * 
     * Note: calling setclone() more than once on the same stream can cause
     * weird interactions with buffering.  Be careful.  It is often best to
     * flush your output buffer before calling setclone().
     */
    virtual void setclone(IWvStream *clone);
    
    /**
     * Close this stream.  Note that this *does* close the inner stream.
     * However, see ~WvStreamClone() for a note about destroying vs. closing
     * the stream.
     */
    virtual void close();
    
    /**
     * WvStream overrides.  These generally just call the cloned stream's
     * version.
     */
    
    virtual bool flush_internal(time_t msec_timeout);
    virtual size_t uread(void *buf, size_t size);
    virtual size_t uwrite(const void *buf, size_t size);
    virtual bool isok() const;
    virtual int geterr() const;
    virtual WvString errstr() const;
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
    virtual const WvAddr *src() const;
    virtual void execute();
    virtual void noread();
    virtual void nowrite();
    virtual WvString getattr(WvStringParm name) const;

private:
    void close_callback();

protected:
    WvString my_type;
public:
    const char *wstype() const { return my_type; }
};

#endif // __WVSTREAMCLONE_H
