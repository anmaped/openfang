/* -*- Mode: C++ -*-
 *
 * X.509 certificate class: This class abstracts some of the common operations
 * performed on basic X.509 certificates (signature verification, public
 * key identification, etc.). If you want to perform operations with
 * a certificate and its corresponding private key, consider using WvX509Mgr
 * instead.
 */ 
#ifndef __WVX509_H
#define __WVX509_H

#include "wvlog.h"
#include "wverror.h"
#include "wvrsa.h"
#include "wvstringlist.h"

// Structures to make the compiler happy so we don't have to include x509v3.h ;)
struct x509_st;
typedef struct x509_st X509;
struct ssl_ctx_st;
typedef struct ssl_ctx_st SSL_CTX;

struct X509_name_st;
typedef struct X509_name_st X509_NAME;

struct asn1_string_st;
typedef struct asn1_string_st ASN1_TIME;


// workaround for the fact that OpenSSL initialization stuff must be called
// only once.
void wvssl_init();
void wvssl_free();
WvString wvssl_errstr();


/**
 * X509 Class to handle certificates and their related
 * functions
 */
class WvX509 : public IObject
{
    IMPLEMENT_IOBJECT(WvX509);
public:
   /**
    * Type for the @ref encode() and @ref decode() methods.
    * CertPEM   = PEM Encoded X.509 Certificate
    * CertDER   = DER Encoded X.509 Certificate
    * CertHex   = DER Encoded X.509 Certificate in hexified form
    * CertFilePEM = PEM Encoded X.509 Certificate from file 
    * CertFileDER  = DER Encoded X.509 Certificate from file
    *
    * CertFilePEM and CertFileDER are only valid modes for @ref decode(),
    * calling encode with these modes will result in no effect.
    */
    enum DumpMode { CertPEM = 0, CertDER, CertHex, CertFilePEM, CertFileDER };

    enum FprintMode { FingerMD5 = 0, FingerSHA1 };
    /**
     * Initialize a completely empty X509 Object with an X509 certificate
     * that doesn't have anything it it... good for building custom 
     * certificates.
     */
    WvX509();
    
    /**
     * Initialize a blank X509 Object with the certificate *cert
     * (used for client side operations...)
     * 
     * This either initializes a completely empty object, or takes _cert,
     * and extracts the distinguished name into dname, and the RSA
     * public key into rsa. rsa->prv is empty.
     */
    WvX509(X509 *_cert);

    /** 
     * Copy Constructor. 
     */
    WvX509(const WvX509 &x509);

public:
    /** Destructor */
    virtual ~WvX509();
    
    /**
     * Allow us to access the certificate member - this will be going away 
     * eventually, but for now, it gets us out of a couple of issues :/
     */
    X509 *get_cert() { return cert; }

    /**
     * Set the public key of the certificate to the public key rsa_pubkey.
     */
    void set_pubkey(WvRSAKey &rsa_pubkey);
    
    /**
     * Create a certificate request (PKCS#10) using this function.. this 
     * request is what you would send off to Verisign, or Entrust.net (or any
     * other CA), to get your real certificate. It leaves the RSA key pair
     * in rsa, where you MUST save it for the certificate to be AT ALL
     * valid when you get it back. Returns a PEM Encoded PKCS#10 certificate
     * request, and leaves the RSA keypair in rsa.
     */    
    static WvString certreq(WvStringParm subject, const WvRSAKey &rsa);
        
    /**
     * Function to verify the validity of a certificate that has been
     * placed in cert. It checks and make sure that it was signed by
     * the CA certificate cacert, as well as that it is not expired (or
     * not yet valid).
     */
    bool validate(WvX509 *cacert = NULL) const;

   /**
    * Check the certificate in cert against the CA certificate in cacert
    * - returns true if cert was signed by that CA certificate.
    */
    bool signedbyca(WvX509 &cacert) const;

    /**
     * Check to see if the certificate in cert was issued by the CA
     * certificate in cacert. Note: You are going on the certificate's
     * say-so by using this function. You may also want to use signedbyca
     * to check if the certificate is actually signed by who it claims
     * to be issued by.
     */
    bool issuedbyca(WvX509 &cacert) const;

    /**
     * Verify that the contents of data were signed
     * by the certificate currently in cert. This only
     * checks the signature, it doesn't check the validity
     * of the certificate.
     */
    bool verify(WvBuf &original, WvStringParm signature) const;
    bool verify(WvStringParm original, WvStringParm signature) const;

    /** 
     * Return the information requested by mode. 
     */
    WvString encode(const DumpMode mode) const;
    void encode(const DumpMode mode, WvBuf &buf) const;

    /**
     * Load the information from the format requested by mode into
     * the class - this overwrites the certificate.
     */
    virtual void decode(const DumpMode mode, WvStringParm str);
    virtual void decode(const DumpMode mode, WvBuf &encoded);

    /** 
     * Get and set the Certificate Issuer (usually the CA who signed 
     * the certificate).
     */
    WvString get_issuer() const;
    void set_issuer(WvStringParm name);
    void set_issuer(const WvX509 &cacert);
    
    /**
     * get and set the Subject field of the certificate
     */
    WvString get_subject() const;
    void set_subject(WvStringParm name);
    void set_subject(X509_NAME *name);

    /**
     * get and set the serialNumber field of the certificate
     */
    WvString get_serial(bool hex = false) const;
    void set_serial(long serial_no);

    /** 
     * get and set the Netscape Comment extension
     */
    WvString get_nscomment() const;
    void set_nscomment(WvStringParm comment);
    
    /**
     * get and set the Netscape SSL Server extension
     */
    WvString get_nsserver() const;
    void set_nsserver(WvStringParm server_fqdn);
    
    /**
     * get the CRL Distribution points if they exist, WvString::null
     * if they don't.
     */
    WvString get_crl_dp() const;

    /**
     * Get any certificate Policy OIDs. Returns true if the policy oids 
     * extension is present, false otherwise.
     */
    bool get_policies(WvStringList &policy_oids) const;
    
    /**
     * Set the Certificate Policy OIDs in the certificate to that of
     * the input array.
     */
    void set_policies(WvStringList &policy_oids);

    /**
     * Set the Certificate to use X509v3, since that's all modern
     * PKI uses anyways :)
     */
    void set_version();

    /**
     * Get and set the keyUsage field.
     */
    WvString get_key_usage() const;
    void set_key_usage(WvStringParm values);

    /**
     * Get and set the extendedKeyUsage field.
     */
    WvString get_ext_key_usage() const;
    void set_ext_key_usage(WvStringParm values);
    
    /**
     * Return the Subject alt name if it exists, and WvString::null if
     * it doesn't.
     */
    WvString get_altsubject() const;

    /**
     * Set the Subject Alt Name.
     */
    void set_altsubject(WvStringParm name);
    
    /**
     * Get the values in the basic constraints extension. Returns true if the
     * basic constraints extension exists and is valid, false otherwise.
     */
    bool get_basic_constraints(bool &ca, int &pathlen) const;

    /**
     * Set the values in the basic constraints extension.
     */
    void set_basic_constraints(bool ca, int pathlen);

    /**
     * Get the values in the policy constraints extension. Returns true if the
     * policy constraints extension exists, false otherwise.
     */
    bool get_policy_constraints(int &require_explicit_policy, 
                                int &inhibit_policy_mapping) const;
    /**
     * Set the values in the policy constraints extension.
     */
    void set_policy_constraints(int require_explicit_policy, 
                                int inhibit_policy_mapping);
    
    struct PolicyMap {
        PolicyMap(WvStringParm _issuer_domain, WvStringParm _subject_domain)
        {
            issuer_domain = _issuer_domain;
            subject_domain = _subject_domain;
        }
        WvString issuer_domain;
        WvString subject_domain;
    };
    DeclareWvList(PolicyMap);

    /**
     * Get the policy mappings for this certificate. Returns true if there 
     * were any policy mappings to be found.
     */
    bool get_policy_mapping(PolicyMapList &list) const;

    /**
     * Set the policy mappings for this certificate.
     */
    void set_policy_mapping(PolicyMapList &list);

    /**
     * Return the not before and not after in a format we're more able to easily use.
     */
    time_t get_notvalid_before() const;
    time_t get_notvalid_after() const;
    
    /**
     * Set the lifetime to be used for this certificate... the lifetime starts
     * from the minute that the certificate is signed...
     */
    void set_lifetime(long seconds);
    
    /**
     * Get the authority info access information. Usually includes a list of URLs
     * where the issuer's CA certificate may be fetched, as well as a list of
     * OCSP responders. Note that this function returns this information in
     * a giant string: get_ca_urls and get_ocsp may return this information in a 
     * more useful format.
     */
    WvString get_aia() const;

    /**
     * Set a list of urls that have the Certificate of the CA that issued 
     * this certificate, as well as the list of OCSP responders for this
     * certificate.
     */
    void set_aia(WvStringList &ca_urls, WvStringList &responders);

    /**
     * Get a list of OCSP Responders for this certificate
     */
    void get_ocsp(WvStringList &responders) const;
    
    /**
     * Get a list of urls that have the Certificate 
     * of the CA that issued this certificate
     */
    void get_ca_urls(WvStringList &urls) const;
    
    /**
     * Get a list of URLs that are valid CRL distribution
     * points for this certificate.
     */
    void get_crl_urls(WvStringList &urls) const;

    /**
     * Set the list of URLs that are valid CRL distribution
     * points for this certificate.
     */
    void set_crl_urls(WvStringList &urls);

    /**
     * Get the Subject Key Info
     */
    WvString get_ski() const;
    
    /**
     * Get the Authority key Info
     */
    WvString get_aki() const;

    /**
     * Get the certHash (fingerprint) of the certificate
     */
    WvString get_fingerprint(const FprintMode mode = FingerSHA1) const;

    /**
     * Is the certificate object valid?
     */
    virtual bool isok() const;

     /**
      * Returns an error string if isok() is not true.
      */
    virtual WvString errstr() const;

    /**
     * The not operator returns true if !isok()
     */
    bool operator! () const;


private:
    friend class WvCRL;
    friend class WvX509Mgr;
    friend class WvOCSPReq;
    friend class WvOCSPResp;

    /** X.509v3 Certificate - this is why this class exists */
    X509     *cert;

    mutable WvLog debug;

    /**
     * Get and the Extension information - returns NULL if extension doesn't exist
     * Used internally by all of the get_??? and set_??? functions (crl_dp, cp_oid, etc.).
     */
    WvString get_extension(int nid) const;
    void set_extension(int nid, WvStringParm values);
    
    /**
     * Populate the Subject Key Info (from the public key)
     */
    void set_ski();

    /**
     * Populate the Authority key Info, based on the Subject Key Info in
     * cacert.
     */
    void set_aki(const WvX509 &cacert);

    /**
     * Helper method to log an error message when we try to set an X509 value when
     * cert is null.
     */
    void warningset(WvStringParm var);

    /**
     * Return a WvRSAKey filled with the public key from the
     * certificate in cert
     */
    WvRSAKey *get_rsa_pub() const;
};

#endif // __WVX509_H
