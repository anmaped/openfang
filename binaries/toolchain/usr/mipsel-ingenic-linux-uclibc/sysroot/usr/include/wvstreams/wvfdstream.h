/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Base class for streams built on Unix file descriptors.
 */ 
#ifndef __WVFDSTREAM_H
#define __WVFDSTREAM_H

#include "wvstream.h"

/**
 * Base class for streams built on Unix file descriptors.
 * 
 * WvFDStream distinguishes between read and write file descriptors
 * at creation time.  Additionally, the reading and writing halves
 * may be independently shut down by calling noread() or nowrite().
 * 
 */
class WvFdStream : public WvStream
{
protected:
    /** The file descriptor for reading. */
    int rfd;

    /** The file descriptor for writing. */
    int wfd;
    
    /** Have we actually shut down the read/write sides? */
    bool shutdown_read, shutdown_write;

    /**
     * Sets the file descriptor for both reading and writing.
     * Convenience method.
     */
    void setfd(int fd)
        { rfd = wfd = fd; }

public:
    /**
     * Creates a WvStream from an existing file descriptor.
     * "rwfd" is the file descriptor for reading and writing
     */
    WvFdStream(int rwfd = -1);
    
    /**
     * Creates a WvStream from two existing file descriptors.
     * 
     * The file decriptors may be the same.
     * 
     * "rfd" is the file descriptor for reading
     * "wfd" is the file descriptor for writing
     */
    WvFdStream(int rfd, int wfd);

    /** Destroys the stream and invokes close(). */
    virtual ~WvFdStream();

    /**
     * Returns the Unix file descriptor for reading from this stream.
     * Returns: the file descriptor, or -1 if none
     */
    int getrfd() const
        { return rfd; }
    
    /**
     * Returns the Unix file descriptor for writing to this stream.
     * Returns: the file descriptor, or -1 if none
     */
    int getwfd() const
        { return wfd; }

    /**
     * Returns the Unix file descriptor for reading and writing.
     * 
     * Asserts that the file descriptors for reading and writing
     * are the same before returning.
     * 
     * Returns: the file descriptor, or -1 if none
     */
    int getfd() const
    {
        assert(rfd == wfd);
        return rfd;
    }
    
    /** Make the fds on this stream blocking or non-blocking. */
    void set_nonblock(bool nonblock);
    
    /** Make the fds on this stream close-on-exec or not. */
    void set_close_on_exec(bool close_on_exec);

    /***** Overridden members *****/
    
    /**
     * Closes the file descriptors.
     * 
     * If it is undesirable for the file descriptors to be closed by
     * this stream, duplicate the file descriptors using dup() before
     * creating the stream.
     * 
     */
    virtual void close();
    virtual bool isok() const;
    virtual size_t uread(void *buf, size_t count);
    virtual size_t uwrite(const void *buf, size_t count);
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
    virtual void maybe_autoclose();

public:
    const char *wstype() const { return "WvFdStream"; }
};

typedef WvFdStream WvFDStream;

#endif // __WVFDSTREAM_H
