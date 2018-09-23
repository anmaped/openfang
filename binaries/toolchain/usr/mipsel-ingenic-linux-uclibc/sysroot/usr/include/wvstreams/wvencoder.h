/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A top-level data encoder class and a few useful encoders.
 */
#ifndef __WVENCODER_H
#define __WVENCODER_H

#include "wvbuf.h"
#include "wvlinklist.h"
#include "wvstring.h"

/**
 * The base encoder class.
 * 
 * Encoders read data from an input buffer, transform it in some
 * way, then write the results to an output buffer.  The resulting
 * data may be of a different size or data type, and may or may
 * not depend on previous data.
 * 
 * Encoders may or may not possess the following characteristics:
 * 
 *  - Statefulness: encoding of successive input elements may
 *     depend on previous one
 *  - Error states: encoding may enter an error state indicated
 *     by isok() == false due to problems detected
 *     in the input, or by the manner in which the encoder has
 *     been user
 *  - Minimum input block size: data will not be drawn from the
 *     input buffer until enough is available or the encoder
 *     is flushed
 *  - Minimum output block size: data will not be written to the
 *     output buffer until enough free space is available
 *  - Synchronization boundaries: data is process or generated
 *     in chunks which can be manipulated independently of any
 *     others, in which case flush() may cause the encoder to
 *     produce such a boundary in its output
 *  - Recognition of end-of-data mark: a special sequence marks
 *     the end of input, after which the encoder transitions to
 *     isfinished() == true
 *  - Generation of end-of-data mark: a special sequence marks
 *     the end of output when the encoder transitions to
 *     isfinished() == true, usually by an explicit
 *     call to finish()
 *  - Reset support: the encoder may be reset to its initial
 *     state and thereby recycled at minimum cost
 * 
 * 
 * Helper functions are provided for encoding data from plain
 * memory buffers and from strings.  Some have no encode(...)
 * equivalent because they cannot incrementally encode from
 * the input, hence they always use the flush option.
 * 
 * The 'mem' suffix has been tacked on to these functions to
 * resolve ambiguities dealing with 'char *' that should be
 * promoted to WvString.  For instance, consider the signatures
 * of strflushmem(const void*, size_t) and strflushstr(WvStringParm,
 * bool).
 * 
 * Another reason for these suffixes is to simplify overloading
 * the basic methods in subclasses since C++ would require the
 * subclass to redeclare all of the other signatures for
 * an overloaded method.
 * 
 */
class WvEncoder
{
protected:
    bool okay; /*!< false iff setnotok() was called */
    bool finished; /*!< true iff setfinished()/finish() was called */
    WvString errstr; /*!< the error message */

public:
    /** Creates a new WvEncoder. */
    WvEncoder();

    /** Destroys the encoder.  Unflushed data is lost. */
    virtual ~WvEncoder();
    
    /**
     * Returns true if the encoder has not encountered an error.
     * 
     * This should only be used to record permanent failures.
     * Transient errors (eg. bad block, but recoverable) should be
     * detected in a different fashion.
     * 
     * Returns: true if the encoder is ok
     */
    bool isok() const
        { return okay && _isok(); }

    /**
     * Returns true if the encoder can no longer encode data.
     * 
     * This will be set when the encoder detects and end-of-data
     * mark in its input, or when finish() is called.
     * 
     * Returns: true if the encoder is finished
     */
    bool isfinished() const
        { return finished || _isfinished(); }

    /**
     * Returns an error message if any is available.
     *
     * Returns: the error message, or the null string is isok() == true
     */
    WvString geterror() const;

    /**
     * Reads data from the input buffer, encodes it, and writes the result
     * to the output buffer.
     * 
     * If flush == true, the input buffer will be drained and the output
     * buffer will contain all of the encoded data including any that
     * might have been buffered internally from previous calls.  Thus it
     * is possible that new data will be written to the output buffer even
     * though the input buffer was empty when encode() was called.  If the
     * buffer could not be fully drained because there was insufficient
     * data, this function returns false and leaves the remaining unflushed
     * data in the buffer.
     * 
     * If flush == false, the encoder will read and encode as much data
     * as possible (or as is convenient) from the input buffer and store
     * the results in the output buffer.  Partial results may be buffered
     * internally by the encoder to be written to the output buffer later
     * when the encoder is flushed.
     * 
     * If finish = true, the encode() will be followed up by a call to
     * finish().  The return values will be ANDed together to yield the
     * final result.  Most useful when flush is also true.
     *
     * If a permanent error occurs, then isok() will return false, this
     * function will return false and the input buffer will be left in an
     * undefined state.
     * 
     * If a recoverable error occurs, the encoder should discard the
     * problematic data from the input buffer and return false from this
     * function, but isok() will remain true.
     * 
     * A stream might become isfinished() == true if an encoder-
     * specific end-of-data marker was detected in the input.
     * 
     * "inbuf" is the input buffer
     * "outbuf" is the output buffer
     * "flush" is if true, flushes the encoder
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     * @see _encode for the actual implementation
     */
    bool encode(WvBuf &inbuf, WvBuf &outbuf, bool flush = false,
        bool finish = false);

    /**
     * Flushes the encoder and optionally finishes it.
     *
     * "inbuf" is the input buffer
     * "outbuf" is the output buffer
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */
    bool flush(WvBuf &inbuf, WvBuf &outbuf,
        bool finish = false)
        { return encode(inbuf, outbuf, true, finish); }

    /**
     * Tells the encoder that NO MORE DATA will ever be encoded.
     * 
     * The encoder will flush out any internally buffered data
     * and write out whatever end-of-data marking it needs to the
     * supplied output buffer before returning.
     * 
     * Clients should invoke flush() on the input buffer before
     * finish() if the input buffer was not yet empty.
     * 
     * It is safe to call this function multiple times.
     * The implementation will simply return isok() and do nothing else.
     * 
     * "outbuf" is the output buffer
     * Returns: true on success
     * @see _finish for the actual implementation
     */
    bool finish(WvBuf &outbuf);

    /**
     * Asks an encoder to reset itself to its initial state at
     * creation time, if supported.
     * 
     * This function may be called at any time, even if
     * isok() == false, or isfinished() == true.
     * 
     * If the behaviour is not supported or an error occurs,
     * then false is returned and afterwards isok() == false.
     * 
     * Returns: true on success
     * @see _reset for the actual implementation
     */
    bool reset();

    /**
     * Flushes data through the encoder from a string to a buffer.
     *
     * "instr" is the input string
     * "outbuf" is the output buffer
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */
    bool flushstrbuf(WvStringParm instr, WvBuf &outbuf,
        bool finish = false);
        
    /**
     * Flushes data through the encoder from a string to a string.
     * 
     * The output data is appended to the target string.
     * 
     * "instr" is the input string
     * "outstr" is the output string
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */
    bool flushstrstr(WvStringParm instr, WvString &outstr,
        bool finish = false);

    /**
     * Encodes data from a buffer to a string.
     * 
     * The output data is appended to the target string.
     * 
     * "inbuf" is the input buffer
     * "outstr" is the output string
     * "flush" is if true, flushes the encoder
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */   
    bool encodebufstr(WvBuf &inbuf, WvString &outstr,
        bool flush = false, bool finish = false);

    /**
     * Flushes data through the encoder from a buffer to a string.
     * 
     * The output data is appended to the target string.
     * 
     * "inbuf" is the input buffer
     * "outstr" is the output string
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */   
    bool flushbufstr(WvBuf &inbuf, WvString &outstr,
        bool finish = false)
        { return encodebufstr(inbuf, outstr, true, finish); }
    
    /**
     * Flushes data through the encoder from a string to a string.
     *
     * "inbuf" is the input buffer
     * "finish" is if true, calls finish() on success
     * Returns: the resulting encoded string, does not signal errors
     */   
    WvString strflushstr(WvStringParm instr, bool finish = false);
    
    /**
     * Flushes data through the encoder from a buffer to a string.
     *
     * "inbuf" is the input buffer
     * "finish" is if true, calls finish() on success
     * Returns: the resulting encoded string, does not signal errors
     */   
    WvString strflushbuf(WvBuf &inbuf, bool finish = false);

    /**
     * Flushes data through the encoder from memory to a buffer.
     *
     * "inmem" is the input data pointer
     * "inlen" is the input data length
     * "outbuf" is the output buffer
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */
    bool flushmembuf(const void *inmem, size_t inlen, WvBuf &outbuf,
        bool finish = false);
        
    /**
     * Flushes data through the encoder from memory to memory.
     * 
     * The outlen parameter specifies by reference
     * the length of the output buffer.  It is updated in place to
     * reflect the number of bytes copied to the output buffer.
     * If the buffer was too small to hold the data, the overflow
     * bytes will be discarded and false will be returned.
     * 
     * "inmem" is the input data pointer
     * "inlen" is the input data length
     * "outmem" is the output data pointer
     * "outlen" is the output data length, by reference
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */
    bool flushmemmem(const void *inmem, size_t inlen, void *outmem,
        size_t *outlen, bool finish = false);
        
    /**
     * Encodes data from a buffer to memory.
     * 
     * The outlen parameter specifies by reference
     * the length of the output buffer.  It is updated in place to
     * reflect the number of bytes copied to the output buffer.
     * If the buffer was too small to hold the data, the overflow
     * bytes will be discarded and false will be returned.
     * 
     * "inmem" is the input data pointer
     * "inlen" is the input data length
     * "outmem" is the output data pointer
     * "outlen" is the output data length, by reference
     * "flush" is if true, flushes the encoder
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */
    bool encodebufmem(WvBuf &inbuf, void *outmem, size_t *outlen,
        bool flush = false, bool finish = false);   
        
    /**
     * Flushes data through the encoder from a buffer to memory.
     * 
     * The outlen parameter specifies by reference
     * the length of the output buffer.  It is updated in place to
     * reflect the number of bytes copied to the output buffer.
     * If the buffer was too small to hold the data, the overflow
     * bytes will be discarded and false will be returned.
     * 
     * "inbuf" is the input buffer
     * "outmem" is the output data pointer
     * "outlen" is the output data length, by reference
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */
    bool flushbufmem(WvBuf &inbuf, void *outmem, size_t *outlen,
        bool finish = false)
        { return encodebufmem(inbuf, outmem, outlen, true, finish); }

    /**
     * Flushes data through the encoder from a string to memory.
     * 
     * The outlen parameter specifies by reference
     * the length of the output buffer.  It is updated in place to
     * reflect the number of bytes copied to the output buffer.
     * If the buffer was too small to hold the data, the overflow
     * bytes will be discarded and false will be returned.
     * 
     * "instr" is the input string
     * "outmem" is the output data pointer
     * "outlen" is the output data length, by reference
     * "finish" is if true, calls finish() on success
     * Returns: true on success
     */
    bool flushstrmem(WvStringParm instr, void *outmem, size_t *outlen,
        bool finish = false);

    /**
     * Flushes data through the encoder from memory to a string.
     *
     * "inmem" is the input data pointer
     * "inlen" is the input data length
     * "finish" is if true, calls finish() on success
     * Returns: the resulting encoded string, does not signal errors
     */
    WvString strflushmem(const void *inmem, size_t inlen, bool finish = false);

protected:
    /** Sets 'okay' to false explicitly. */
    void setnotok()
        { okay = false; }

    /** Sets an error condition, then setnotok(). */
    void seterror(WvStringParm message)
        { errstr = message; setnotok(); }

    /** Sets an error condition, then setnotok(). */
    void seterror(WVSTRING_FORMAT_DECL)
        { seterror(WvString(WVSTRING_FORMAT_CALL)); }

    /** Sets 'finished' to true explicitly. */
    void setfinished()
        { finished = true; }

protected:
    /**
     * Template method implementation of isok().
     * 
     * Not called if any of the following cases are true:
     * 
     *  - okay == false
     * 
     * 
     * Most implementations do not need to override this.
     * 
     * Returns: true if the encoder is ok
     * @see setnotok
     */
    virtual bool _isok() const
        { return true; }

    /**
     * Template method implementation of isfinished().
     * 
     * Not called if any of the following cases are true:
     * 
     *  - finished == true
     * 
     * 
     * Most implementations do not need to override this.
     * 
     * Returns: true if the encoder is finished
     * @see setfinished
     */
    virtual bool _isfinished() const
        { return false; }

    /**
     * Template method implementation of geterror().
     * 
     * Not called if any of the following cases are true:
     * 
     *  - isok() == true
     *  - errstr is not null
     * 
     * 
     * Most implementations do not need to override this.
     * 
     * Returns: the error message, or the null string if _isok() == true
     * @see seterror
     */
    virtual WvString _geterror() const
        { return WvString::null; }

    /**
     * Template method implementation of encode().
     * 
     * Not called if any of the following cases are true:
     * 
     *  - okay == false
     *  - finished == true
     *  - in.used() == 0 && flush == false
     * 
     * 
     * All implementations MUST define this.
     * 
     * If you also override _isok() or _isfinished(), note that they
     * will NOT be consulted when determining whether or not to
     * invoke this function.  This allows finer control over the
     * semantics of isok() and isfinished() with respect to encode().
     * 
     * "inbuf" is the input buffer
     * "outbuf" is the output buffer
     * "flush" is if true, flushes the encoder
     * Returns: true on success
     * @see encode
     */
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf, bool flush) = 0;

    /**
     * Template method implementation of finish().
     * 
     * Not called if any of the following cases are true:
     * 
     *  - okay == false
     *  - finished == true
     * 
     * 
     * The encoder is marked finished AFTER this function exits.
     * 
     * Many implementations do not need to override this.
     * 
     * If you also override _isok() or _isfinished(), note that they
     * will NOT be consulted when determining whether or not to
     * invoke this function.  This allows finer control over the
     * semantics of isok() and isfinished() with respect to finish().
     * 
     * "outbuf" is the output buffer
     * Returns: true on success
     * @see finish
     */
    virtual bool _finish(WvBuf &outbuf)
        { return true; }

    /**
     * Template method implementation of reset().
     * 
     * When this method is invoked, the current local state will
     * be okay == true and finished == false.  If false is returned,
     * then okay will be set to false.
     * 
     * May set a detailed error message if an error occurs.
     * 
     * Returns: true on success, false on error or if not supported
     * @see reset
     */
    virtual bool _reset()
        { return false; }
};


/** An encoder that discards all of its input. */
class WvNullEncoder : public WvEncoder
{
protected:
    virtual bool _encode(WvBuf &in, WvBuf &out, bool flush);
    virtual bool _reset(); // supported: does nothing
};


/**
 * A very efficient passthrough encoder that just merges the
 * input buffer into the output buffer.
 * 
 * Counts the number of bytes it has processed.
 * 
 * Supports reset().
 * 
 */
class WvPassthroughEncoder : public WvEncoder
{
    size_t total;
    
public:
    WvPassthroughEncoder();
    virtual ~WvPassthroughEncoder() { }

    /**
     * Returns the number of bytes processed so far.
     * Returns: the number of bytes
     */
    size_t bytes_processed() { return total; }
    
protected:
    virtual bool _encode(WvBuf &in, WvBuf &out, bool flush);
    virtual bool _reset(); // supported: resets the count to zero
};


/**
 * An encoder chain owns a list of encoders that are used in sequence
 * to transform data from a source buffer to a target buffer.
 * 
 * Supports reset() if all the encoders it contains also support
 * reset().
 * 
 */
class WvEncoderChain : public WvEncoder
{
    class ChainElem
    {
    public:
        WvEncoder *enc;
        WvDynBuf out;
        bool autofree;

        ChainElem(WvEncoder *enc, bool autofree)
	    : enc(enc), autofree(autofree) { }
        ~ChainElem() { if (autofree) delete enc; }
    };
    DeclareWvList(ChainElem);

    ChainElemList encoders;
    ChainElem *last_run;
    
public:
    /** Creates an initially empty chain of encoders. */
    WvEncoderChain();

    /** Destroys the encoder chain.  Calls zap(). */
    virtual ~WvEncoderChain();

    /**
     * Appends an encoder to the tail of the chain.
     * if "autofree" is true, takes ownership of the encoder
     */
    void append(WvEncoder *enc, bool autofree);

    /**
     * Prepends an encoder to the head of the chain.
     * if "autofree" is true, takes ownership of the encoder
     */
    void prepend(WvEncoder *enc, bool autofree);

    /**
     * Gets the autofree state of a particular encoder in the chain.
     *
     * If an encoder is in the chain multiple times, this will return
     * true if at least one instance is set to autofree.
     */
    bool get_autofree(WvEncoder *enc) const;

    /**
     * Sets the autofree state of a particular encoder in the chain.
     *
     * if "autofree" is true, takes ownership of the encoder, by ensuring
     * only one of the encoders has autofree set.  If it is false, then
     * all encoders have their autofree cleared.
     */
    void set_autofree(WvEncoder *enc, bool autofree);

    /**
     * Unlinks the encoder from the chain.
     * Deletes the encoder if it was added with autofree == true.
     */
    void unlink(WvEncoder *enc);

    /**
     * Clears the encoder chain.
     * Deletes any encoders that were added with autofree == true.
     */
    void zap();
    
    /**
     * "Continues" encoding a buffer.  Runs all data in the buffer through
     * any encoders that have been added to the chain since the last encode.
     * 
     * This is only useful once, immediately after you add one or more new
     * encoders to the chain.  However, you can call it whenever you want,
     * and if no new encoders have been added, the given data will simply
     * be (perhaps) copied unchanged into outbuf.  Note that this call
     * never does flush() or finish(), so outbuf may or may not be used.
     * 
     * The supplied inbuf is guaranteed to be empty when this function
     * returns.
     */
    bool continue_encode(WvBuf &inbuf, WvBuf &outbuf);

    /**
     * Returns true if there is data in an internal buffer.
     */
    size_t buffered();

protected:
    /**
     * Returns true if the encoder has not encountered an error.
     * 
     * WvEncoderChain is special in that it may transition from
     * isok() == false to isok() == true if the offending encoders
     * are removed from the list.
     * 
     * Returns: true iff all encoders return isok() == true
     * @see WvEncoder::_isok
     */
    virtual bool _isok() const;
    
    /**
     * Returns true if the encoder can no longer encode data.
     * 
     * WvEncoderChain is special in that it may transition from
     * isfinished() == true to isfinished() == false if the offending
     * encoders are removed from the list, but not if finish() is
     * called.
     * 
     * Returns: false iff all encoders return isfinished() == false
     */
    virtual bool _isfinished() const;

    /**
     * Returns the error message, if any.
     * 
     * WvEncoderChain is special in that it may transition from
     * !geterror() = false to !geterror() = true if the offending
     * encoders are removed from the list.
     * 
     * Returns: the first non-null error message in the chain
     */
    virtual WvString _geterror() const;
    
    /**
     * Passes the data through the entire chain of encoders.
     * Returns true iff all encoders return true.
     */
    virtual bool _encode(WvBuf &in, WvBuf &out, bool flush);
    
    /**
     * Finishes the chain of encoders.
     * 
     * Invokes finish() on the first encoder in the chain, then
     * flush() on the second encoder if new data was generated,
     * then finish() on the second encoder, and so on until all
     * encoders have been flushed and finished (assuming the first
     * encoder had already been flushed).
     * 
     * Returns true iff all encoders return true.
     */
    virtual bool _finish(WvBuf & out);

    /**
     * Resets the chain of encoders.
     * 
     * Resets all of the encoders in the chain and discards any
     * pending buffered input.  Preserves the list of encoders.
     * 
     * Returns: true iff all encoders return true.
     */
    virtual bool _reset();
    
private:
    /** Used by _encode() and _finish() */
    bool do_encode(WvBuf &in, WvBuf &out, ChainElem *start_after,
		   bool flush, bool finish);
};

#endif // __WVENCODER_H
