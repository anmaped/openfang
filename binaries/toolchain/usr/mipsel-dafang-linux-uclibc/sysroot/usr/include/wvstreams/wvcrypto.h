/* -*- Mode: C++ -*-
 * Worldvisions Tunnel Vision Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Miscellaneous cryptography primitives.
 */
#ifndef __WVCRYPTO_H
#define __WVCRYPTO_H

#include "wvencoder.h"
#include "wvfile.h"


/** A very simple stream that returns randomness from /dev/urandom */
class WvRandomStream : public WvFile
{
public:
    WvRandomStream();
};

class WvCryptoEncoder : public WvEncoder
{
public:
    /*
     * Sets the current TripleDES keys and resets the initialization
     * vector to all nulls.
     *
     * "key[1-3]" are the new keys
     */
    virtual void setkey(const void *_key)
    {
	return;
    }
    
    virtual void setkey(const void *_key1, const void *_key2, 
			const void *_key3)
    {
	return;
    }

    /*
     * Sets the current TripleDES initialization vector.
     *
     * "iv" is the new IV must be 8 bytes
     */
    virtual void setiv(const void *iv)
    {
	return;
    }

};


#endif // __WVCRYPTO_H
