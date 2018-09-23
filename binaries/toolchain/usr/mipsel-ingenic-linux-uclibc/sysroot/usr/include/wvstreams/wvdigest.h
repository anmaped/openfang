/* -*- Mode: C++ -*-
 * Worldvisions Tunnel Vision Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * MD5, SHA-1 and HMAC digest abstractions.
 */
#ifndef __WVDIGEST_H
#define __WVDIGEST_H

#include "wvencoder.h"
#include <stdint.h>

struct env_md_st;
struct env_md_ctx_st;
struct hmac_ctx_st;

/**
 * Superclass for all message digests.
 * 
 * All message digest encoders have the following semantics:
 * 
 *  - On encode() or flush(), data from the input buffer is
 *     consumed and a message digest function is applied to
 *     incrementally update the internal digest state.
 *     No output is ever generated.
 *  - On finish(), the message digest is finalized and its value
 *     is written to the output buffer.  Afterwards, no new data
 *     can be processed unless reset() is called.
 *  - On reset(), the current digest state is discarded allowing
 *     a new stream of data to be processed.
 * 
 */
class WvDigest : public WvEncoder
{
public:
    /** Returns the number of bytes in the message digest. */
    virtual size_t digestsize() const = 0;
};


/**
 * @internal
 * Base class for all digests constructed using the OpenSSL EVP API.
 */
class WvEVPMDDigest : public WvDigest
{
    friend class WvHMACDigest;
    const env_md_st *evpmd;
    env_md_ctx_st *evpctx;
    bool active;

public:
    virtual ~WvEVPMDDigest();
    virtual size_t digestsize() const;

protected:
    WvEVPMDDigest(const env_md_st *_evpmd);
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf,
        bool flush); // consumes input
    virtual bool _finish(WvBuf &outbuf); // outputs digest
    virtual bool _reset(); // supported: resets digest value
    
    const env_md_st *getevpmd()
        { return evpmd; }

private:
    void cleanup();
};


/**
 * MD5 Digest.
 * Has a digest length of 128 bits.
 */
class WvMD5Digest : public WvEVPMDDigest
{
public:
    /** Creates an MD5 digest encoder. */
    WvMD5Digest();
    virtual ~WvMD5Digest() { }
};


/**
 * SHA-1 Digest.
 * Has a digest length of 160 bits.
 */
class WvSHA1Digest : public WvEVPMDDigest
{
public:
    /** Creates an SHA1 digest encoder. */
    WvSHA1Digest();
    virtual ~WvSHA1Digest() { }
};


/**
 * HMAC Message Authentication Code.
 * Has a digest length that equals that of its underlying
 * message digest encoder.
 */
class WvHMACDigest : public WvDigest
{
    WvEVPMDDigest *digest;
    unsigned char *key;
    size_t keysize;
    hmac_ctx_st *hmacctx;
    bool active;

public:
    /**
     * Creates an HMAC digest encoder.
     *
     * "digest" is the message digest encoder to use as a
     *        hash function
     * "key" is the authentication key
     * "keysize" is the key size in bytes
     */
    WvHMACDigest(WvEVPMDDigest *_digest, const void *_key,
		 size_t _keysize);
    virtual ~WvHMACDigest();
    virtual size_t digestsize() const;

protected:
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf,
        bool flush); // consumes input
    virtual bool _finish(WvBuf &outbuf); // outputs digest
    virtual bool _reset(); // supported: resets digest value

private:
    void cleanup();
};


/**
 * CRC32 checksum
 * Digest length of 4 bytes.
 */
class WvCrc32Digest : public WvDigest
{
    uint32_t crc;

public:
    WvCrc32Digest();
    virtual ~WvCrc32Digest() { }

    virtual size_t digestsize() const;
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf,
                         bool flush); // consumes input
    virtual bool _finish(WvBuf &outbuf); // outputs digest
    virtual bool _reset(); // supported: resets digest value
};


/**
 * Adler32 checksum
 * Digest length of 4 bytes.
 */
class WvAdler32Digest : public WvDigest
{
    uint32_t crc;

public:
    WvAdler32Digest();
    virtual ~WvAdler32Digest() { }

    virtual size_t digestsize() const;
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf,
                         bool flush); // consumes input
    virtual bool _finish(WvBuf &outbuf); // outputs digest
    virtual bool _reset(); // supported: resets digest value
};

#endif // __WVDIGEST_H
