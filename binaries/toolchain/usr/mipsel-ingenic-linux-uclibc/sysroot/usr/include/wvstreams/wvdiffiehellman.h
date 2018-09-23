/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 * Copyright (C) 2003 Net Integration Technologies, Inc.
 *
 * Diffie-Hellman shared secret creation.
 */

#ifndef __WVDIFFIEHELLMAN_H
#define __WVDIFFIEHELLMAN_H

#include <openssl/dh.h>

#include "wvstream.h"
#include "wvlog.h"

class WvDiffieHellman
{
public:
    WvDiffieHellman(const unsigned char *_key, int _keylen, 
		    BN_ULONG _generator);
    ~WvDiffieHellman() { DH_free(info); }

    void get_created_secret(WvBuf &outbuf, size_t len);
    int get_public_value(WvBuf &outbuf, int len);

    int pub_key_len();
    bool create_secret(WvBuf &inbuf, size_t in_len, WvBuf& outbuf);

protected:
    struct dh_st *info;
    BN_ULONG generator;

private:

    WvLog log;
};

#endif /* __WVDIFFIEHELLMAN_H */
