/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Base64 encoder and decoder implementations.
 */
#ifndef __WVBASE64_H
#define __WVBASE64_H

#include "wvencoder.h"

/**
 * A base 64 encoder.
 * 
 * On finish(), outputs any needed pad characters.
 * 
 * Supports reset().
 * 
 */
class WvBase64Encoder : public WvEncoder
{
    enum State {
        ATBIT0, ATBIT2, ATBIT4
    };
    State state;
    unsigned int bits; // remaining bits shifted left 8 bits
    
public:
    /** Creates a base 64 encoder. */
    WvBase64Encoder();
    virtual ~WvBase64Encoder() { }

protected:
    // on flush, outputs any needed pad characters
    virtual bool _encode(WvBuf &in, WvBuf &out, bool flush);
    virtual bool _finish(WvBuf &out);
    virtual bool _reset(); // supported
};


/**
 * A base 64 decoder.
 * 
 * Becomes isfinished() == true on detection of padding.
 * 
 * Supports reset().
 * 
 */
class WvBase64Decoder : public WvEncoder
{
    enum State {
        ATBIT0, ATBIT2, ATBIT4, ATBIT6, PAD
    };
    State state;
    unsigned int bits; // remaining bits shifted left 6 bits
    
public:
    /** Creates a base 64 decoder. */
    WvBase64Decoder();
    virtual ~WvBase64Decoder() { }

protected:
    virtual bool _encode(WvBuf &in, WvBuf &out, bool flush);
    virtual bool _reset(); // supported
};

#endif // __WVBASE64_H
