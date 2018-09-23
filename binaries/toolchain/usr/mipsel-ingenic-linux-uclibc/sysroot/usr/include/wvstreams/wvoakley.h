/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 * Copyright (C) 2003 Net Integration Technologies, Inc.
 *
 * Diffie-Hellman shared secret creation.
 */

#ifndef __WVOAKLEY_H
#define __WVOAKLEY_H

#include "wvstream.h"
#include "wvdiffiehellman.h"

class WvOakleyAuth
{
public:
    WvOakleyAuth(int group);
    short public_len();
    short other_pub_len();
    short get_public_key(WvBuf &outbuf, short len);
    short get_other_public_key(WvBuf &outbuf, short len);
    void create_secret(WvBuf &_other_pub_key, short len);
    WvDynBuf dh_secret;

private:
    WvDiffieHellman *dh;
    short pub_len, other_len;
    short secret_len;
    WvDynBuf other_pub_key;
};

#endif /* __WVOAKLEY_H */
