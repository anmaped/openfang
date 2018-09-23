/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * SSL (Socket Security Layer) communications via WvStreams.
 */ 
#ifndef __WVSSLSTREAM_H
#define __WVSSLSTREAM_H

#include "wvfdstream.h"
#include "wvlog.h"
#include "wvstreamclone.h"
#include "wvtr1.h"

struct ssl_st;
struct ssl_ctx_st;
struct ssl_method_st;

typedef struct ssl_ctx_st SSL_CTX;
typedef struct ssl_st SSL;
typedef struct ssl_method_st SSL_METHOD;

class WvX509;
class WvX509Mgr;
class WvSSLStream;

typedef wv::function<bool(WvX509*)> WvSSLValidateCallback;
typedef wv::function<bool(WvX509*, WvSSLStream *)> WvSSLGlobalValidateCallback;

/**
 * SSL Stream, handles SSLv2, SSLv3, and TLS
 * Methods - If you want it to be a server, then you must feed the constructor
 * a WvX509Mgr object
 */
class WvSSLStream : public WvStreamClone
{
public:
    /* This ValidateCallback is purely more convenient to set (not passed in
     * via constructor) than its local cousin.  It is used when you want an
     * easy way to assign a validation function to any WvSSLStream you might
     * be using.  NOTE:  It should be assigned before you instantiate a stream,
     * and should never be changed while WvSSLStreams still linger.
     *
     * NOTE:  Using wv::bind can effectively bind an object with a particular
     * function for this callback, so you can do all sorts of interesting stuff
     * with it.
     */
    static WvSSLGlobalValidateCallback global_vcb;
    /**  
     * Start an SSL connection on the stream _slave.  The x509 structure
     * is optional for a client, and mandatory for a server.  You need to
     * keep the X509 object around for the entire life of this object!
     */
    WvSSLStream(IWvStream *_slave, WvX509Mgr *_x509 = NULL, 
    		WvSSLValidateCallback _vcb = 0, bool _is_server = false);
    
    /** Cleans up everything (calls close + frees up the SSL Objects used) */
    virtual ~WvSSLStream();
    
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
    
    virtual void close();
    virtual bool isok() const;
    virtual void noread();
    virtual void nowrite();
    
protected:
    WvX509Mgr *x509;
    
    /** SSL Context - used to create SSL Object */
    SSL_CTX *ctx;
    
    /**
     * Main SSL Object - after SSL_set_fd() we make all calls through the
     * connection through here
     */
    SSL *ssl;
    
    virtual size_t uwrite(const void *buf, size_t len);
    virtual size_t uread(void *buf, size_t len);
    
private:
    /**
     * Connection Status Flag, since SSL takes a few seconds to
     * initialize itself.
     */
    bool sslconnected;
    SelectRequest connect_wants;

    /** Set the connected flag and flush the unconnected_buf */
    void setconnected(bool conn);
    
    /** Keep track of whether we are a client or a server */
    bool is_server;
    
    /** We have to override the WvStream noread/nowrite implementation... */
    bool ssl_stop_read, ssl_stop_write;
    
    /** Keep track of whether we want to check the peer who connects to us */
    WvSSLValidateCallback vcb;
    
    /** Internal Log Object */
    WvLog debug;

    /**
     * SSL_write() may return an SSL_ERROR_WANT_WRITE code which
     * indicates that the function should be called again with
     * precisely the same arguments as the last time.  To ensure that
     * this can happen, we must unfortunately copy data into a bounce
     * buffer and remeber the fact.  We use a WvBuf here to allow
     * an arbitrary amount of data to be set aside.
     */
    WvInPlaceBuf write_bouncebuf;
    size_t write_eat;

    /** Similar nastiness happens with SSL_read() */
    WvInPlaceBuf read_bouncebuf;
    bool read_pending;

    /** Need to buffer writes until sslconnected */
    WvDynBuf unconnected_buf;

    /** Prints out the entire SSL error queue */
    void printerr(WvStringParm func);

public:
    const char *wstype() const { return "WvSSLStream"; }
};

#endif // __WVSSLSTREAM_H

