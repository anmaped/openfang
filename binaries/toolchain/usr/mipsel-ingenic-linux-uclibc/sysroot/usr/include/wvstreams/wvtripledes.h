/* -*- Mode: C++ -*-
 * Worldvisions Tunnel Vision Software:
 *   Copyright (C) 1997-2003 Net Integration Technologies, Inc.
 *
 * TripleDES cryptography abstractions.
 */
#ifndef __WVTRIPLEDES_H
#define __WVTRIPLEDES_H

#include "wvencoder.h"
#include "wvencoderstream.h"
#include "wvcrypto.h"

#include <openssl/des.h>

/**
 * An encoder implementing the TripleDES encryption method.
 * 
 * Supports reset().
 * 
 */
class WvTripleDESEncoder : public WvCryptoEncoder
{
public:
    enum Mode {
        ECBEncrypt, /*!< Encrypt using ECB mode (avoid) */
        ECBDecrypt, /*!< Decrypt using ECB mode (avoid) */
        CFBEncrypt, /*!< Encrypt using CFB mode (simulates a stream) */
        CFBDecrypt, /*!< Decrypt using CFB mode (simulates a stream) */
	CBCEncrypt, /*!< Encrypt using CBC mode */
	CBCDecrypt  /*!< Decrypt using CBC mode */
    };

    /*
     * Creates a new TripleDES cipher encoder.
     *
     * "mode" is the encryption mode
     * "key[1-3]" are the initial keys
     */
    WvTripleDESEncoder(Mode mode, const void *key1, const void *key2, 
		       const void *key3);
/*     virtual ~WvTripleDESEncoder(); */

    /*
     * Sets the current TripleDES keys and resets the initialization
     * vector to all nulls.
     *
     * "key[1-3]" are the new keys
     */
    virtual void setkey(const void *key)
    { 
	setkey(key, (unsigned char*)key+DES_KEY_SZ, 
	       (unsigned char *)key+(DES_KEY_SZ*2)); 
	return; 
    }
    virtual void setkey(const void *_key1, const void *_key2, 
			const void *_key3);
    
    /*
     * Sets the current TripleDES initialization vector.
     *
     * "iv" is the new IV must be 8 bytes
     */
    virtual void setiv(const void *iv);

protected:
    virtual bool _encode(WvBuf &in, WvBuf &out, bool flush);
    virtual bool _reset(); // supported: restores most recently set
    // key and initialization vector

private:
    Mode mode;
    des_cblock key;
    des_key_schedule deskey1;
    des_key_schedule deskey2;
    des_key_schedule deskey3;
    des_cblock ivec; // initialization vector
    int ivecoff; // current offset into initvec
};


/**
 * A crypto stream implementing TripleDES encryption.
 * 
 * By default, written data is encrypted using
 * WvTripleDESEncoder::CFBEncrypt, read data is decrypted using
 * WvTripleDESEncoder::CFBDecrypt.
 * 
 * @see WvTripleDESEncoder
 */
class WvTripleDESStream : public WvEncoderStream
{
public:
    WvTripleDESStream(WvStream *_cloned, const void *_key1,
		      const void *_key2, const void *_key3,
		      WvTripleDESEncoder::Mode readmode = WvTripleDESEncoder::CFBDecrypt,
		      WvTripleDESEncoder::Mode writemode = WvTripleDESEncoder::CFBEncrypt);
    virtual ~WvTripleDESStream() { }
};

#endif // __WVTRIPLEDES_H
