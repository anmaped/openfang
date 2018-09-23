/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Provides an encoder for applying a functor to data extracted
 * from a buffer and stored in another.
 * Assumes binary input is in machine order.
 */
#ifndef __WVFUNCTORENCODER_H
#define __WVFUNCTORENCODER_H

#include "wvtypedencoder.h"

/**
 * Functor specifies the functor type which must have an operator()
 * with a signature compatible with invocations of the form:
 *   const IT data = ...;
 *   OT result = func(data);
 *
 * The best way to use this monster is to subclass with friendly
 * names for the implementations that are needed.  For maximum
 * performance, define the functor as a struct that provides an
 * operator() inline.  This is extremely efficent since this class is
 * templated, so there's room for lots of compiler optimization.
 *
 * "IT" is the input buffer datatype
 * "OT" is the output buffer datatype
 * "FT" is the functor type
 */
template<class IT, class OT, class FT>
class WvFunctorEncoder : public WvTypedEncoder<IT, OT>
{
protected:
    FT f;
    
public:
    typedef FT FType;
    typedef IT IType;
    typedef OT OType;
    typedef WvBufBase<IType> IBuffer;
    typedef WvBufBase<OType> OBuffer;  
    WvFunctorEncoder(const FType &f) : f(f) { }
    virtual ~WvFunctorEncoder() { }

protected:
    virtual bool _typedencode(IBuffer &inbuf, OBuffer &outbuf,
        bool flush)
    {
        size_t count;
        while ( (count = inbuf.optgettable()) )
        {
            size_t avail = outbuf.optallocable();
            if (avail == 0)
                return ! flush;
            if (avail < count)
                count = avail;
            const IType *indata = inbuf.get(count);
            OType *outdata = outbuf.alloc(count);
            while (count-- > 0)
                *(outdata++) = f(*(indata++));
        }
        return true;
    }
    virtual bool _reset()
    {
        // Assume most functor encoders will be stateless and therefore
        // support reset() implicitly.
        // If this is not the case, then override this method for
        // particular subclasses to return false.
        return true;
    }
};

#endif // __WVFUNCTORENCODER_H
