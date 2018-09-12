/* -*- Mode: C++ -*-
 * Worldvisions Tunnel Vision Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Functions to make us compile with both newer and older versions of openssl.
 * 
 * The trick here is to write C wrappers for functions where different
 * versions of openssl have changed the pointer types of arguments: C
 * files only give warnings about mismatched pointers, where C++ files
 * fail completely.
 */
#ifndef __WVSSLHACKS_H
#define __WVSSLHACKS_H

#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/ocsp.h>

#ifdef __cplusplus
extern "C" {
#endif
    
RSA *wv_d2i_RSAPublicKey(RSA **a, const unsigned char **pp, long length);
RSA *wv_d2i_RSAPrivateKey(RSA **a, const unsigned char **pp, long length);
DSA *wv_d2i_DSAPublicKey(DSA **a, const unsigned char **pp, long length);
DSA *wv_d2i_DSAPrivateKey(DSA **a, const unsigned char **pp, long length);
X509_REQ *wv_d2i_X509_REQ(X509_REQ **a, const unsigned char **pp, long length);    
X509 *wv_d2i_X509(X509 **a, unsigned char **pp, long length);

int wv_i2d_OCSP_REQUEST_bio(BIO *bio, OCSP_REQUEST *req);


#ifdef __cplusplus
};
#endif

#endif // __WVSSLHACKS_H
