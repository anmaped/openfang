/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A buffered loopback stream.
 */ 
#ifndef __WVBUFSTREAM_H
#define __WVBUFSTREAM_H

#include "wvstream.h"

/**
 * WvBufStream stores data written by write(), and returns it
 * later on in read().
 * 
 * Handy for making virtual streams, like WvHttpPool.
 * 
 * After seteof(), the stream closes itself (without error) once the inbuf
 * is emptied out by read().
 * 
 * FIXME: WvStream itself should probably deal better with streams that go
 * !isok() with stuff still in inbuf.  Then we could skip seteof() altogether,
 * and just do close() when we're done write()ing data.  For now, it's just
 * too unreliable to do that, since streams do things like drop out of
 * WvStreamLists automatically when they're !isok(), even if they still have
 * data in inbuf.
 * 
 * 2003/12/09: This is cleaned up a lot by WvStream's new noread()/nowrite()
 * functions, but we still need to actually *use* them in WvBufStream...
 */
class WvBufStream : public WvStream
{
    bool dead, /*!< true if the stream has been closed */
	 eof;  /*!< true if the sender has no more data to write(). */
    
public:
    WvBufStream();
    virtual ~WvBufStream();
	
    virtual void close();
    
    virtual size_t uread(void *buf, size_t size);
    virtual size_t uwrite(const void *buf, size_t size);
    virtual bool isok() const;
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
    
    void seteof() { eof = true; }
};


#endif // __WVBUFSTREAM_H
