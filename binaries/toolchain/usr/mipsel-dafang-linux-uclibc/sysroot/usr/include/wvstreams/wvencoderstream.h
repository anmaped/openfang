/* -*- Mode: C++ -*-
 * Worldvisions Tunnel Vision Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * An stream wrapper for encoders.
 */
#ifndef __WVENCODERSTREAM_H
#define __WVENCODERSTREAM_H

#include "wvstream.h"
#include "wvstreamclone.h"
#include "wvencoder.h"

/**
 * WvEncoderStream chains a series of encoders on the input and
 * output ports of the underlying stream to effect on-the-fly data
 * transformations.
 * 
 * Notice that the value of WvEncoderStream's auto_flush flag becomes
 * important when working with encoders that treat explicit buffer
 * flushing in a special manner.  For instance, on flush() the Gzip
 * encoder synchronizes its output.  Were auto_flush to remain true,
 * each incremental write to the stream would cause the Gzip encoder
 * to flush its dictionary thereby resulting in poor compression.
 * 
 * See WvStream::auto_flush(bool).
 * 
 * WARNING: it is possible to add to the readchain and/or writechain in the
 * middle of a session, for example to add gzip compression after first
 * negotiating it with the remote end.  If you plan to do this, be very
 * careful with buffering.  Never read() or getline() more bytes than
 * you're sure will exist with the old encoding.  Even then, only ever
 * append things to the readchain, not prepend.  (Therefore only prepend
 * things to the writechain.) Always flush() or flush_write() before
 * adding to the writechain.  See also min_readsize.
 */
class WvEncoderStream : public WvStreamClone
{
    bool is_closing;
    WvDynBuf readinbuf;
    WvDynBuf readoutbuf;
    WvDynBuf writeinbuf;
    //WvDynBuf writeoutbuf;

public:
    /** Encoder chain through which input data is passed. */
    WvEncoderChain readchain;

    /** Encoder chain through which output data is passed. */
    WvEncoderChain writechain;

    /**
     * Controls the minimum number of unencoded bytes the encoder
     * should try to read at once from the underlying stream,
     * to optimize performance of block-oriented protocols.
     * This is not the same as queuemin() which guarantees how much
     * encoded input must be generated before select() returns true.
     * if 0, the encoder will only read whatever is specified in uread().
     * 
     * NOTE: if you plan to append a new encoder to the readchain after
     * doing read(), you should always use read() with only 1 byte at a
     * time, or some extra data will end up in inbuf and not pass through
     * the newly- added encoder.  Remember that getline() also calls
     * read().  The purpose of min_readsize is to make these one-byte
     * read() calls more efficient, because the *underlying* stream can
     * still read lots of bytes, as long as we only pass single bytes down
     * to the WvEncoderStream.
     * 
     * If you don't plan to add new encoders to readchain from now on, you
     * can give larger sizes to read() or getline(), and thus min_readsize
     * is unnecessary.
     */
    size_t min_readsize;

    /**
     * Creates an encoder stream.
     *
     * "cloned" is the stream to wrap
     */
    WvEncoderStream(WvStream *cloned);
    virtual ~WvEncoderStream();

    /**
     * Safely shuts down the stream.
     * 
     * Does the following in sequence:
     * 
     *  - Flushes and finishes the read chain
     *  - Flushes and finishes the write chain
     *  - Flushes the stream output buffers
     *  - Closes the underlying stream
     */
    virtual void close();

    /**
     * Flushes the read chain through to the stream's input buffer.
     * 
     * The regular stream flush() only operates on the write chain.
     * 
     * Returns: true if the encoder chain returned true
     */
    bool flush_read();

    /**
     * Flushes the write chain through to the stream's output buffer.
     * 
     * The regular stream flush() invokes this, then attempts to
     * synchronously push the buffered data to the stream, which
     * may not always be desirable since it can be quite costly.
     * 
     * To simply cause the write chain to be flushed but allow
     * WvStreams to drain the encoded output buffer at its leisure,
     * use this function.
     * 
     * Returns: true if the encoder chain returned true
     */
    bool flush_write();

    /**
     * Calls flush() then finish() on the read chain of encoders.
     *
     * Returns: true if the encoder chain returned true
     */
    bool finish_read();

    /**
     * Calls flush() then finish() on the write chain of encoders.
     * 
     * Does not flush() the stream.
     * 
     * Returns: true if the encoder chain returned true.
     */
    bool finish_write();

    /**
     * Defines isok() semantics for encoders.
     * 
     * Returns false on error or after all data has been read from
     * the internal buffers and readchain.isfinished() or
     * ! writechain.isok().
     * 
     * Returns: true if it is still possible to read and write data
     */
    virtual bool isok() const;

    virtual size_t uread(void *buf, size_t size);
    virtual size_t uwrite(const void *buf, size_t size);
    
protected:
    void pre_select(SelectInfo &si);
    bool post_select(SelectInfo &si);
    virtual bool flush_internal(time_t msec_timeout);

private:
    void checkreadisok();
    void checkwriteisok();
    
    // pulls a chunk of specified size from the underlying stream
    void pull(size_t size);

    // pushes a chunk to the underlying stream
    bool push(bool flush, bool finish);

public:
    const char *wstype() const { return "WvEncoderStream"; }
};

#endif // __WVENCODERSTREAM_H
