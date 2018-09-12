/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Gzip encoder/decoder based on zlib.
 */
#ifndef __WVGZIP_H
#define __WVGZIP_H

#include "wvencoder.h"
#include "wvencoderstream.h"

struct z_stream_s;

/**
 * An encoder implementing Gzip encryption and decryption.
 * 
 * When compressing:
 * 
 *  - On flush(), the encoded data stream is synchronized such that
 *     all data compressed up to this point can be fully decompressed.
 *     
 *  - On finish(), the encoded data stream is finalized an a Gzip
 *     end of data marker is emitted.
 * 
 * 
 * When decompressing:
 * 
 *  - The encoder will transition to isfinished() == true on its own
 *     if a Gzip end of data marker is detected in the input.  After
 *     this point, no additional data can be decompressed.
 * 
 * 
 */
class WvGzipEncoder : public WvEncoder
{
public:
    enum Mode {
        Deflate, /*!< Compress using deflate */
        Inflate  /*!< Decompress using inflate */
    };
    
    /**
     * Creates a Gzip encoder.
     *
     * "mode" is the compression mode
     */
    WvGzipEncoder(Mode mode, size_t _out_limit = 0);
    virtual ~WvGzipEncoder();

    /**
     * Limit the amount of output produced in one call to encode().
     * Defaults to 0, meaning no limit (empty the input buffer).
     */
    size_t out_limit;

    /**
     * Continue decompression if errors are found.
     * If true, upon running into a decompression error, the encoder will
     * seek to the next unbroken block.  Most useful if at least some blocks
     * are fully flushed (see 'full_flush' below).  Defaults to false.
     * Note that it may be impossible to ignore really bad decompression
     * errors, in which case the encoder will go !isok(), as it would
     * if this boolean is false.
     */
    bool ignore_decompression_errors;

    /**
     * Do full flushes.
     * Makes the encoder use Z_FULL_FLUSH instead of Z_SYNC_FLUSH, which
     * resets the compression state to improve chances of recovering
     * corrupted compressed data.  Degrades compression, so don't use it too
     * often.  Defaults to false.
     */
    bool full_flush;

protected:
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf, bool flush);
    virtual bool _finish(WvBuf &outbuf);
    virtual bool _reset();

private:
    struct z_stream_s *zstr;
    WvInPlaceBuf tmpbuf;
    Mode mode;
    size_t output;

    void init();
    void close();
    void prepare(WvBuf *inbuf);
    bool process(WvBuf &outbuf, bool flush, bool finish);
};


#endif // __WVGZIP_H
