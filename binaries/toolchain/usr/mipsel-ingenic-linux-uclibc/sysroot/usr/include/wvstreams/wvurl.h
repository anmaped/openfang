/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * WvURL is a simple URL-parsing class with built-in (though still somewhat
 * inconvenient) DNS resolution.
 */ 
#ifndef __WVURL_H
#define __WVURL_H

#include "wvstring.h"
#include "wvresolver.h"

class WvIPPortAddr;

class WvUrl
{
public:
    WvUrl(WvStringParm url);
    WvUrl(const WvUrl &url);
    ~WvUrl();
    
    bool isok() const
        { return port != 0 && (resolving || addr != NULL); }
    WvStringParm errstr() const
        { return err; }
    bool resolve(); // dns-resolve the hostname (returns true if done)

    operator WvString () const;
    
    // not actually defined - this just prevents accidental copying
    const WvUrl &operator= (const WvUrl &);
    
    WvStringParm getproto() const
        { return proto; }
    
    // this one is ONLY valid if resolve() returns true!
    const WvIPPortAddr getaddr() const
        { return addr ? *addr : WvIPPortAddr(); }
    
    WvStringParm getfile() const
        { return file; }
    WvStringParm gethost() const
        { return hostname; }
    int getport() const
        { return port; }
    WvStringParm getuser() const
        { return user; }
    WvStringParm getpassword() const
        { return password; }

protected:
    WvString proto, hostname, user, password;
    int port;
    bool resolving;
    WvResolver dns;
    WvIPPortAddr *addr;
    WvString file, err;
};


// backwards compatibility
typedef WvUrl WvURL;

#endif // __WVURL_H
