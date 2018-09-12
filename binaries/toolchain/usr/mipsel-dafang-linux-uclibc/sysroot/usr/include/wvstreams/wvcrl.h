/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2007 Net Integration Technologies, Inc. and others.
 *
 * X.509v3 CRL management class.
 */ 
#ifndef __WVCRL_H
#define __WVCRL_H

#include "wverror.h"
#include "wvlog.h"
#include "wvx509.h"

// Structures to make the compiler happy so we don't have to include x509v3.h ;)
struct X509_crl_st;
typedef struct X509_crl_st X509_CRL;
struct ssl_ctx_st;
typedef struct ssl_ctx_st SSL_CTX;
struct asn1_string_st;
typedef struct asn1_string_st ASN1_INTEGER;

class WvX509Mgr;

/**
 * CRL Class to handle certificate revocation lists and their related
 * functions
 */
class WvCRL
{
public:
    /**
     * Type for the @ref encode() and @ref decode() methods:
     * CRLPEM   = PEM Encoded X.509 CRL
     * CRLDER   = DER Encoded X.509 CRL 
     * CRLFilePEM   = PEM Encoded X.509 CRL
     * CRLFileDER   = DER Encoded X.509 CRL 
     */
    enum DumpMode { CRLPEM = 0, CRLDER, CRLFilePEM, CRLFileDER };

    /**
     * Initialize a blank (null) CRL object.
     */
    WvCRL();
    
    /**
     * Initialize a CRL object, signed and created by the certificate
     * 'ca'.
     */
    WvCRL(const WvX509Mgr &ca);

    /** Destructor */
    virtual ~WvCRL();

    /** Accessor for CRL */
    X509_CRL *getcrl()
    { return crl; }
 
    /**
     * Check the CRL in crl against the CA certificate in cert
     * - returns true if CRL was signed by that CA certificate.
     */
    bool signedbyca(const WvX509 &cacert) const;

    /**
     * Check the issuer name of the CRL in crl against the CA certificate in cert
     * - returns true if the names match.
     */
    bool issuedbyca(const WvX509 &cacert) const;

    /**
     * Checks to see if the CRL is expired (i.e.: the present time is past the
     * nextUpdate extension).
     * - returns true if CRL has expired.
     */
    bool expired() const;

    /*
     * Checks to see if the CRL has any critical extensions in it.
     * - returns true if the CRL has any critical extensions.
     */
    bool has_critical_extensions() const;

    /**
     * Type for @ref validate() method:
     * ERROR = there was an error that happened..
     * VALID = the certificate is valid
     * NOT_THIS_CA = the certificate is not signed by this CA
     * NO_VALID_SIGNATURE = the certificate claims to be signed by this CA (Issuer is the same),
     *                      but the signature is invalid.
     */    
    enum Valid { CRLERROR = -1, VALID, NOT_THIS_CA, NO_VALID_SIGNATURE, 
                 EXPIRED, UNHANDLED_CRITICAL_EXTENSIONS };

    /**
     * Checks to see that a CRL is signed and issued by a CA certificate, and
     * that it has not expired.
     * - returns a validity status.
     * Get the Authority key Info
     */
    Valid validate(const WvX509 &cacert) const;

    /**
     * Get the Authority key Info
     */
    WvString get_aki() const;

    /** 
     * Get the CRL Issuer.
     */
    WvString get_issuer() const;

    /**
     * Do we have any errors... convenience function..
     */
    bool isok() const;
    
    /** 
     * Return the information requested by mode as a WvString. 
     */
    WvString encode(const DumpMode mode) const;
    void encode(const DumpMode mode, WvBuf &buf) const;

    /**
     * Load the information from the format requested by mode into
     * the class - this overwrites the CRL.
     */
    void decode(const DumpMode mode, WvStringParm encoded);
    void decode(const DumpMode mode, WvBuf &encoded);

    /**
     * Is the certificate in cert revoked?
     */
    bool isrevoked(const WvX509 &cert) const;
    bool isrevoked(WvStringParm serial_number) const;

    /**
     * Add the certificate specified by cert to the CRL.
     */
    void addcert(const WvX509 &cert);

    /**
     * Counts the number of certificates in this CRL.
     * WARNING: this method will be very slow and will consume a lot
     * of memory for large CRLs.
     */
    int numcerts() const;
    
private:    
    mutable WvLog debug;
    X509_CRL *crl;
};

#endif // __WVCRL_H
