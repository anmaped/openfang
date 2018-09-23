/* -*- Mode: C++ -*-
 *
 * OCSP request and response abstractions.
 *
 * OCSP provides a quick way of checking whether a certificate is valid or 
 * not. For more information, see: http://en.wikipedia.org/wiki/OCSP
 *
 * For the sake of both ease of implementation and use, these classes only 
 * expose a simplified subset of OCSP functionality.
 *  - A nonce (unique identifier for the request) is always sent in the 
 *    request.
 *  - Both the request and response objects assume only one certificate is to 
 *    be validated.
 *
 */ 
#ifndef __WVOCSP_H
#define __WVOCSP_H
#include "wvx509.h"

#include <openssl/ocsp.h>


class WvOCSPReq
{
public:
    WvOCSPReq(const WvX509 &cert, const WvX509 &issuer);
    virtual ~WvOCSPReq();

    void encode(WvBuf &buf);

private:
    WvOCSPReq(WvOCSPReq &); // not implemented yet
    friend class WvOCSPResp;
    OCSP_CERTID *id;
    OCSP_REQUEST *req;
};


class WvOCSPResp
{
public:
    WvOCSPResp();
    virtual ~WvOCSPResp();

    void decode(WvBuf &buf);

    bool isok() const;
    bool check_nonce(const WvOCSPReq &req) const;    
    bool signedbycert(const WvX509 &cert) const;
    WvX509 get_signing_cert() const;

    enum Status { Error, Good, Revoked, Unknown };
    Status get_status(const WvX509 &cert, const WvX509 &issuer) const;
    static WvString status_str(Status status);

private:
    WvOCSPResp(WvOCSPResp &); // not implemented yet
    OCSP_RESPONSE *resp;
    OCSP_BASICRESP * bs;
    mutable WvLog log;
};

#endif // __WVOCSP_H
