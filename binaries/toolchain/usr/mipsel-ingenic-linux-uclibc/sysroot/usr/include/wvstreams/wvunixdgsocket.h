#ifndef __WVUNIXDGSOCKET_H
#define __WVUNIXDGSOCKET_H

#include <sys/types.h>
#include <sys/syslog.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "wvlog.h"
#include "wvstring.h"
#include "wvlinklist.h"
#include "wvfdstream.h"
#include "wvaddr.h"

class WvUnixDGListener;
class WvUnixDGConn;

/** 
 * WvStream-based Unix datagram domain socket base class.  
 *
 * You probably want to use WvUnixDGConn and WvUnixDGListener as this will
 * probably be deprecated one day.
 */
class WvUnixDGSocket : public WvFDStream {

    bool server;
    int backoff;

    DeclareWvList(WvBuf);
    WvBufList bufs;

public:
    WvUnixDGSocket(WvStringParm filename, bool _server, int perms = 0222);

    virtual  ~WvUnixDGSocket();

    virtual size_t uwrite(const void *buf, size_t count);
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
   
protected:
     WvString socketfile;
     
public:
    const char *wstype() const { return "WvUnixDGSocket"; }

    size_t bufsize;
};

/** 
 * WvStream-based Unix datagram domain socket connection class that listens on
 * filename.
 * 
 * The specified file should already exist before using this.
*/
class WvUnixDGConn : public WvUnixDGSocket
{
public:
    WvUnixDGConn(WvStringParm filename)
        : WvUnixDGSocket(filename, false)
        {}
        
public:
    const char *wstype() const { return "WvUnixDGConn"; }
};

/** 
 * Server end of a Unix datagram socket stream.
 * 
 * Makes a datagram socket at filename with its permissions set to perms and
 * then listens to it like a good little WvStream.
 * 
 * FIXME: Make this suck less.  Do we want this to look like a wvunixsocket?
 */
class WvUnixDGListener : public WvUnixDGSocket 
{ 
public:
    WvUnixDGListener(WvStringParm filename, int perms = 0222)
        : WvUnixDGSocket(filename, true, perms)
        {}

public:
    const char *wstype() const { return "WvUnixDGListener"; }
};



#endif 
