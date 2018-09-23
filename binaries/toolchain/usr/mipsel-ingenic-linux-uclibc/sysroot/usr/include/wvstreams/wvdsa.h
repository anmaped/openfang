/*
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2004 Net Integration Technologies, Inc.
 *
 * DSA cryptography abstractions.
 */
#ifndef __WVDSA_H
#define __WVDSA_H

#include "wverror.h"
#include "wvencoder.h"
#include "wvencoderstream.h"

struct dsa_st;

/**
 * An DSA public key or public/private key pair that can be used for
 * encryption.
 * 
 * Knows how to encode/decode itself into a string of hex digits
 * for easy transport.
 */
class WvDSAKey : public WvErrorBase
{
    WvString pub, prv;

    void init(WvStringParm keystr, bool priv);
    static WvString hexifypub(struct dsa_st *dsa);
    static WvString hexifyprv(struct dsa_st *dsa);

public:
    struct dsa_st *dsa;

    WvDSAKey(const WvDSAKey &k);
    WvDSAKey(struct dsa_st *_dsa, bool priv); // note: takes ownership

    /**
     * Populate the DSA key with a hexified() key 
     */
    WvDSAKey(WvStringParm keystr, bool priv);

    /**
     * Create a new DSA key of bits strength.
     */
    WvDSAKey(int bits);
    
    ~WvDSAKey();
    
    virtual bool isok() const;
    
    /**
     * Retrieve the private key as a hexified string
     * returns WvString::null if there is only a public
     * key.
     */
    WvString private_str() const
        { return prv; }

    /**
     * Retrieve the public key as a hexified string
     */
    WvString public_str() const
        { return pub; }
    
    /**
     * Retrieve the public or private key in PEM encoded
     * format.
     */
    WvString getpem(bool privkey);
};


#endif // __WVDSA_H
