/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * An abstraction for encoders that manipulate typed buffers.
 */
#ifndef __WVTYPEDENCODER_H
#define __WVTYPEDENCODER_H

#include "wvencoder.h"
#include "wvbufbase.h"

/**
 * This template facilitates the creation and use of encoders
 * that manipulate typed buffers.
 * 
 * A typed encoder accepts both typed and untyped buffers, but
 * is implementated in terms of typed buffers.  Untyped buffers
 * are automatically wrapped into the required form before being
 * passed on to the implementation.
 * 
 * This type is designed to function as a statically bound mixin
 * to make it easier to incorporate typed encoders into untyped
 * encoder hierarchies.  This is somewhat ugly, but necessary.
 * 
 *
 * "IT" is the input buffer datatype
 * "OT" is the output buffer datatype
 * "S" is the WvEncoder supertype
 * @see WvEncoder
 */
template<class IT, class OT, class S = WvEncoder>
class WvTypedEncoder : public S
{
public:
    typedef IT IType;
    typedef OT OType;
    typedef WvBufBase<IType> IBuffer;
    typedef WvBufBase<OType> OBuffer;
    typedef WvBufViewBase<IType> IBufferView;
    typedef WvBufViewBase<OType> OBufferView;

    /**
     * Typed variant of encode().
     * @see encode(WvBuf&, WvBuf&, bool, bool)
     */
    bool encode(IBuffer &inbuf, OBuffer &outbuf, bool flush = false,
        bool finish = false)
    {
        WvBufView inview(inbuf);
        WvBufView outview(outbuf);
        return S::encode(inview, outview, flush, finish);
    }

    /**
     * Typed variant of flush().
     * @see flush(WvBuf, WvBuf, bool)
     */
    bool flush(IBuffer &inbuf, OBuffer &outbuf, bool finish = false)
    {
        WvBufView inview(inbuf);
        WvBufView outview(outbuf);
        return S::flush(inview, outview, finish);
    }

    /**
     * Typed variant of finish().
     * @see finish(WvBuf)
     */
    bool finish(OBuffer &outbuf)
    {
        WvBufView outview(outbuf);
        return S::finish(outview);
    }
    
    bool encode(WvBuf &inbuf, WvBuf &outbuf,
        bool flush = false, bool finish = false)
    {
        return S::encode(inbuf, outbuf, flush, finish);
    }
    bool flush(WvBuf &inbuf, WvBuf &outbuf,
        bool finish = false)
    {
        return S::flush(inbuf, outbuf, finish);
    }
    bool finish(WvBuf &outbuf)
    {
        return S::finish(outbuf);
    }

protected:
    /**
     * Typed variant of _encode().
     * @see _encode(WvBuf&, WvBuf&, bool)
     */
    virtual bool _typedencode(IBuffer &inbuf, OBuffer &outbuf,
        bool flush) = 0;
    
    /**
     * Typed variant of _finish().
     * @see _finish(WvBuf&)
     */
    virtual bool _typedfinish(OBuffer &outbuf)
        { return true; }

    /** Wrapper implementation of _encode(). */
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf,
        bool flush)
    {
        IBufferView inview(inbuf);
        OBufferView outview(outbuf);
        return _typedencode(inview, outview, flush);
    }
    
    /** Wrapper implementation of _finish(). */
    virtual bool _finish(WvBuf &outbuf)
    {
        OBufferView outview(outbuf);
        return _typedfinish(outview);
    }
};

/**
 * Partial template specialization for unsigned char output
 * buffer type to avoid compilation errors.
 *
 * "IType" is the input buffer datatype
 */
template<class IT, class S>
class WvTypedEncoder<IT, unsigned char, S> : public S
{
public:
    typedef IT IType;
    typedef unsigned char OType;
    typedef WvBufBase<IType> IBuffer;
    typedef WvBufBase<OType> OBuffer;
    typedef WvBufViewBase<IType> IBufferView;
    typedef WvBufViewBase<OType> OBufferView;

    /**
     * Typed variant of encode().
     * @see encode(WvBuf&, WvBuf&, bool, bool)
     */
    bool encode(IBuffer &inbuf, OBuffer &outbuf, bool flush = false,
        bool finish = false)
    {
        WvBufView inview(inbuf);
        return S::encode(inview, outbuf, flush, finish);
    }

    /**
     * Typed variant of flush().
     * @see flush(WvBuf, WvBuf, bool)
     */
    bool flush(IBuffer &inbuf, OBuffer &outbuf, bool finish = false)
    {
        WvBufView inview(inbuf);
        return S::flush(inview, outbuf, finish);
    }
    
    bool encode(WvBuf &inbuf, WvBuf &outbuf,
        bool flush = false, bool finish = false)
    {
        return S::encode(inbuf, outbuf, flush, finish);
    }
    bool flush(WvBuf &inbuf, WvBuf &outbuf,
        bool finish = false)
    {
        return S::flush(inbuf, outbuf, finish);
    }

protected:
    /**
     * Typed variant of _encode().
     * @see _encode(WvBuf&, WvBuf&, bool)
     */
    virtual bool _typedencode(IBuffer &inbuf, OBuffer &outbuf,
        bool flush) = 0;
    
    /**
     * Typed variant of _finish().
     * @see _finish(WvBuf&)
     */
    virtual bool _typedfinish(OBuffer &outbuf)
        { return true; }
    
    /** Wrapper implementation of _encode(). */
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf,
        bool flush)
    {
        IBufferView inview(inbuf);
        return _typedencode(inview, outbuf, flush);
    }
    
    /** Wrapper implementation of _finish(). */
    virtual bool _finish(WvBuf &outbuf)
    {
        return _typedfinish(outbuf);
    }
};


/**
 * Partial template specialization for unsigned char input
 * and output buffer types to avoid compilation errors.
 */
template<class S>
class WvTypedEncoder<unsigned char, unsigned char, S> : public S
{
public:
    typedef unsigned char IType;
    typedef unsigned char OType;
    typedef WvBufBase<IType> IBuffer;
    typedef WvBufBase<OType> OBuffer;
    typedef WvBufViewBase<IType> IBufferView;
    typedef WvBufViewBase<OType> OBufferView;

protected:
    /**
     * Typed variant of _encode().
     * @see _encode(WvBuf&, WvBuf&, bool)
     */
    virtual bool _typedencode(IBuffer &inbuf, OBuffer &outbuf,
        bool flush) = 0;
    
    /**
     * Typed variant of _finish().
     * @see _finish(WvBuf&)
     */
    virtual bool _typedfinish(OBuffer &outbuf)
        { return true; }

    /** Wrapper implementation of _encode(). */
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf,
        bool flush)
    {
        return _typedencode(inbuf, outbuf, flush);
    }
    
    /** Wrapper implementation of _finish(). */
    virtual bool _finish(WvBuf &outbuf)
    {
        return _typedfinish(outbuf);
    }
};

#endif // __WVTYPEDENCODER
