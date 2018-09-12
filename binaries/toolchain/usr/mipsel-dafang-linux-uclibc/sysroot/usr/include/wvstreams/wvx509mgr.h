/* -*- Mode: C++ -*-
 *
 * X.509 certificate management class: This class builds upon the 
 * functionality provided by the WvX509 class, adding operations that are
 * made possible with the addition of a private key (e.g. signing certificates
 * and CRLs).
 */ 
#ifndef __WVX509MGR_H
#define __WVX509MGR_H

#include "wvx509.h"
#include "wvcrl.h"

class WvX509Mgr : public WvX509
{
  public:
    /**
     * Constructor to create a blank certificate + keypair (useful if, for
     * example, you were going to load the appropriate values in later).
     */
    WvX509Mgr();

    /**
     * Constructor to create a self-signed certificate for the given dn and
     * RSA key.  If you don't already have a WvRSAKey, try the other
     * constructor, below, which creates one automatically. If 'ca' is true, 
     * the certificate will be created as a certificate authority.
     * 
     * For SSL Servers, the dname must contain a "cn=" section in order to
     * validate correctly with some clients, particularly web browsers.
     * For example, if your domain name is nit.ca, you can try this for
     * _dname: "cn=nit.ca,o=Net Integration,c=CA", or maybe this instead:
     * "cn=nit.ca,dc=nit,dc=ca"
     * 
     * We don't check automatically that your _dname complies with these
     * restrictions, since non-SSL certificates may be perfectly valid
     * without this.  If you want to generate invalid certs, that's up to
     * you.
     */
    WvX509Mgr(WvStringParm _dname, WvRSAKey *_rsa, bool ca = false);
    
    /**
     * Constructor to create a new self-signed certificate for the given dn
     * and number of bits.  See the previous constructor for details on how
     * to choose _dname.  'bits' is the number of bits in the auto-generated
     * RSA key; 1024 or 2048 are good values for this. If 'ca' is true, the
     * certificate will be created as a certificate authority.
     */
    WvX509Mgr(WvStringParm _dname, int bits, bool ca=false);

    /** 
     * Copy Constructor. 
     */
    WvX509Mgr(const WvX509Mgr &mgr);

  protected:
    /**
     * Given the Distinguished Name dname and an already generated keypair in 
     * rsa, return a Self Signed Certificate in cert.
     * If is_ca, it will generate a self-issued certificate with the 
     * appropriate values for a certificate authority (or at least the most
     * common ones). Note that a certificate created in this way will not be
     * signed: 
     */
    void create_selfissued(WvStringParm dname, bool is_ca = false);

public:
    /** Destructor */
    virtual ~WvX509Mgr();

    /**
     * Says if this certificate+key pair is good for use. Checks to make sure 
     * that both are present and that they match.
     */
    virtual bool isok() const;

    /**
     * Says what the error is, if isok() is not true.
     */
    virtual WvString errstr() const;

    /**
     * The not operator returns true if !isok()
     */
    bool operator! () const;

    /**
     * Allow us access to the RSA member.
     */
    WvRSAKey *get_rsa() { return rsa; }
    void set_rsa(WvRSAKey *_rsa) { WVDELETE(rsa); rsa = new WvRSAKey(*_rsa); }

    /**
     * Avoid a lot of ugliness by having it so that we are binding to the SSL
     * context, and not the other way around, since that would make ownership
     * of the cert and rsa keys ambiguous.
     */
    bool bind_ssl(SSL_CTX *ctx);

    /**
     * Take the PKCS#10 request in the string pkcs10req, sign it with the
     * private key in rsa, and then spit back a new X509 Certificate in
     * PEM format.
     */
    WvString signreq(WvStringParm pkcs10req) const;

    /**
     * Sign the certificate with the rsa key associated with this class.
     */
    bool signcert(WvX509 &unsignedcert) const;

    /**
     * Sign the CRL with the rsa key associated with this class. This method
     * will also update the lastUpdate time, and set the CRL's validity period 
     * to 30 days.
     */
    bool signcrl(WvCRL &unsignedcrl) const;

    /**
     * Test to make sure that a certificate and a keypair go together.
     * You can call it if you want to test a certificate yourself. 
     * (Such as after a decode)
     */
    bool test() const;

    /**
     * Sign the contents of data and return the signature as a BASE64
     * string.
     */
    WvString sign(WvBuf &data) const;
    WvString sign(WvStringParm data) const;

    /**
     * Encodes the information requested by mode into a buffer.
     */
    virtual WvString encode(const WvX509::DumpMode mode) const;
    virtual WvString encode(const WvRSAKey::DumpMode mode) const;
    virtual void encode(const WvX509::DumpMode mode, WvBuf &buf) const;
    virtual void encode(const WvRSAKey::DumpMode mode, WvBuf &buf) const;

    /**
     * Load the information from the format requested by mode into
     * the class - this overwrites the certificate, and possibly the
     * key - and to enable two stage loading (the certificate first, then the
     * key), it DOES NOT call test() - that will be up to the programmer
     */
    virtual void decode(const WvX509::DumpMode mode, WvStringParm encoded);
    virtual void decode(const WvRSAKey::DumpMode mode, WvStringParm encoded);
    virtual void decode(const WvX509::DumpMode mode, WvBuf &encoded);
    virtual void decode(const WvRSAKey::DumpMode mode, WvBuf &encoded);

    /**
     * This writes the certificate and RSA keys in PKCS12 format to the file 
     * specified by filename, setting the password to "_pkcs12pass". Returns 
     * true if the operation was successful, false otherwise.
     */
    bool write_p12(WvStringParm _fname, WvStringParm _pkcs12pass) const;
    
    /**
     * And this reads from the file specified in filename using the password 
     * "_pkcs12pass", and fills the RSA and cert members with the decoded 
     * information.
     */
    void read_p12(WvStringParm _fname, WvStringParm _pkcs12pass);

  private:
    /**
     * The Public and Private RSA keypair associated with the certificate
     * Make sure that you save this somewhere!!! If you don't, then you won't
     * really be able to use the certificate for anything...
     */
    mutable WvRSAKey *rsa;

    mutable WvLog debug;
};
#endif
