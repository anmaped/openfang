/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A fast, easy-to-use, parallelizing, pipelining HTTP/1.1 file retriever.
 * 
 * Just create a WvHttpPool object, add it to your list, and use pool.addurl()
 * to get a WvStream* that gives you the file you requested.
 */ 
#ifndef __WVHTTPPOOL_H
#define __WVHTTPPOOL_H

#include "ftpparse.h"
#include "wvurl.h"
#include "wvistreamlist.h"
#include "wvstreamclone.h"
#include "wvlog.h"
#include "wvhashtable.h"
#include "wvbufstream.h"
#include "wvbuf.h"
#include "wvcont.h"
#include "wvtcp.h"

class WvBufUrlStream;
class WvUrlStream;
class WvHttpStream;

static const WvString DEFAULT_ANON_PW("weasels@");

struct WvHTTPHeader
{
    WvString name, value;
    
    WvHTTPHeader(WvStringParm _name, WvStringParm _value)
	: name(_name), value(_value) 
    		{}
};


DeclareWvDict(WvHTTPHeader, WvString, name);


class WvUrlRequest
{
public:
    WvUrl url;
    WvString headers;
    WvUrlStream *instream;
    WvBufUrlStream *outstream;
    WvStream *putstream;

    bool pipeline_test;
    bool inuse;
    bool is_dir;
    bool create_dirs;
    WvString method;
    
    WvUrlRequest(WvStringParm _url, WvStringParm _method, WvStringParm _headers,
		 WvStream *content_source, bool _create_dirs, bool _pipeline_test);
    ~WvUrlRequest();
    
    void done();
};

DeclareWvList(WvUrlRequest);


struct WvUrlLink
{
    WvString linkname;
    WvUrl url;

    WvUrlLink(WvStringParm _linkname, WvStringParm _url)
	: linkname(_linkname), url(_url)
    {}
};
DeclareWvList(WvUrlLink);


class WvBufUrlStream : public WvBufStream
{
public:
    WvString url;
    WvString proto;
    WvUrlLinkList links;  // HTML links or FTP directory listing

    // HTTP stuff...
    WvString version;
    int status;
    WvHTTPHeaderDict headers; 

    WvBufUrlStream() : status(0), headers(10)
        {}
    virtual ~WvBufUrlStream()
        {}

public:
    const char *wstype() const { return "WvBufUrlStream"; }
};

DeclareWvTable(WvIPPortAddr);


class WvUrlStream : public WvStreamClone
{
public:
    class Target
    {
    public:
	WvIPPortAddr remaddr;
	WvString username;

	Target(const WvIPPortAddr &_remaddr, WvStringParm _username)
	    : remaddr(_remaddr), username(_username) {}

	~Target() {}

	bool operator== (const Target &n2) const
        { return (username == n2.username && remaddr == n2.remaddr); }
    };
    Target target;
    static int max_requests;

protected:
    WvLog log;
    WvUrlRequestList urls, waiting_urls;
    int request_count;
    WvUrlRequest *curl; // current url
    virtual void doneurl() = 0;
    virtual void request_next() = 0;

public:
    WvUrlStream(const WvIPPortAddr &_remaddr, WvStringParm _username, 
        WvStringParm logname)
	: WvStreamClone(new WvTCPConn(_remaddr)), target(_remaddr, _username),
	  log(logname, WvLog::Debug)
    {
    	request_count = 0;
    	curl = NULL;
    }

    virtual ~WvUrlStream() {};

    virtual void close() = 0;
    void addurl(WvUrlRequest *url);
    void delurl(WvUrlRequest *url);
    // only implemented in WvHttpStream
    virtual size_t remaining()
    { return 0; }
    
    virtual void execute() = 0;
    
public:
    const char *wstype() const { return "WvUrlStream"; }
};

unsigned WvHash(const WvUrlStream::Target &n);

DeclareWvDict(WvUrlStream, WvUrlStream::Target, target);


class WvHttpStream : public WvUrlStream
{
public:
    static bool global_enable_pipelining;
    bool enable_pipelining;
    
private:
    int pipeline_test_count;
    bool ssl;
    bool sent_url_request;      // Have we sent a request to the server yet?
    WvIPPortAddrTable &pipeline_incompatible;
    WvString http_response, pipeline_test_response;
    WvDynBuf putstream_data;
    
    enum { Unknown, Chunked, ContentLength, Infinity,
	   PostHeadInfinity, PostHeadChunked, PostHeadStream,
	   ChuckInfinity, ChuckChunked, ChuckStream } encoding;
    size_t bytes_remaining;
    bool in_chunk_trailer, last_was_pipeline_test, in_doneurl;

    virtual void doneurl();
    virtual void request_next();
    void start_pipeline_test(WvUrl *url);
    WvString request_str(WvUrlRequest *url, bool keep_alive);
    void send_request(WvUrlRequest *url);
    void pipelining_is_broken(int why);
    
public:
    WvHttpStream(const WvIPPortAddr &_remaddr, WvStringParm _username,
         bool ssl, WvIPPortAddrTable &_pipeline_incompatible);
    virtual ~WvHttpStream();

    virtual void close();
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
    virtual void execute();
    virtual size_t remaining()
    { return bytes_remaining; }
    
public:
    const char *wstype() const { return "WvHttpStream"; }
};


class WvFtpStream : public WvUrlStream
{
    bool logged_in, pasv_acked;
    WvString password;
    WvTCPConn *data;
    time_t last_request_time;
    bool sure;

    virtual void doneurl();
    virtual void request_next();

    // Disregard all lines that are of the form "xxx-", meaning that another
    // line follows.  Only the last line is important for us.
    char *get_important_line();

    // Parse response to "PASV" command and returns a pointer to the address
    // of the data port (or NULL if it can't parse the response)..
    // This mucks about with line.
    WvIPPortAddr *parse_pasv_response(char *line);

    WvString parse_for_links(char *line);

    WvCont cont;
    void* real_execute(void*);

public:
    WvFtpStream(const WvIPPortAddr &_remaddr, WvStringParm _username,
		WvStringParm _password);

    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
    virtual void close();
    virtual void execute();
    
public:
    const char *wstype() const { return "WvFtpStream"; }
};


// FIXME: Rename this to WvUrlPool someday.
class WvHttpPool : public WvIStreamList
{
    WvLog log;
    WvResolver dns;
    WvUrlStreamDict conns;
    WvUrlRequestList urls;
    int num_streams_created;
    bool sure;
    
    WvIPPortAddrTable pipeline_incompatible;
    
public:
    WvHttpPool();
    virtual ~WvHttpPool();
    
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
    virtual void execute();
    
    WvBufUrlStream *addurl(WvStringParm _url, WvStringParm _method = "GET",
                            WvStringParm _headers = "",
                            WvStream *content_source = NULL,
                            bool create_dirs = false);

    // For URL uploads.  create_dirs should be true if you want all
    // non-existent directories in _url to be created.
//    WvBufUrlStream *addputurl(WvStringParm _url, WvStringParm _headers,
//			      WvStream *s, bool create_dirs = false);
private:
    void unconnect(WvUrlStream *s);
    
public:
    bool idle() const 
        { return !urls.count(); }
    
public:
    const char *wstype() const { return "WvHttpPool"; }
};


#endif // __WVHTTPPOOL_H
