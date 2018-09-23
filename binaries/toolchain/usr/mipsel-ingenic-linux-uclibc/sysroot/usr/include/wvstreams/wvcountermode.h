/* -*- Mode: C++ -*-
 * Worldvisions Tunnel Vision Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A 'counter mode' cryptography engine abstraction.
 */
#ifndef __WVCOUNTERMODE_H
#define __WVCOUNTERMODE_H

#include "wvencoder.h"

/** A counter mode encryption encoder. */
class WvCounterModeEncoder : public WvEncoder
{
public:
    WvEncoder *keycrypt;

    /**
     * Create a new counter mode encoder / decoder.
     *   _keycrypt    : the underlying encoder for generating the keystream
     *                  (note: takes ownership of this encoder)
     *   _counter     : the initial counter value
     *   _countersize : the counter size, must equal crypto block size
     */
    WvCounterModeEncoder(WvEncoder *_keycrypt,
        const void *_counter, size_t _countersize);
    virtual ~WvCounterModeEncoder();

    /**
     * Sets the Counter mode auto-incrementing counter.
     *   counter     : the counter
     *   countersize : the new counter size, must equal crypto block size
     */
    void setcounter(const void *counter, size_t countersize);

    /**
     * Stores the current counter in the supplied buffer.
     *   counter : the array that receives the counter
     */
    void getcounter(void *counter) const;

    /** Returns the counter size. */
    size_t getcountersize() const
        { return countersize; }
    
    /** Increments the counter. */
    virtual void incrcounter();
    
private:
    WvConstInPlaceBuf counterbuf;

protected:
    unsigned char *counter; // auto-incrementing counter
    size_t countersize; // counter size in bytes
    
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf, bool flush);
};

#endif // __WVCOUNTERMODE_H
