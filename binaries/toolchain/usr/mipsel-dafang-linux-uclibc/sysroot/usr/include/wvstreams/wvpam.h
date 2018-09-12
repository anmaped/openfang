/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2004 Net Integration Technologies, Inc.
 *
 * A WvStream that authenticates with PAM.  If WvStreams is compiled without
 * PAM, it just fails.  Note that if you don't check isok, you can still read
 * and write to the stream - in particular, anything written in the
 * constructor will go through before authentication begins.
 *
 * For now, this only works for PAM modules that don't require any user
 * interaction (not even a password!), such as ssoya.
 */
#ifndef __WVPAM_H
#define __WVPAM_H

#include "wvstringlist.h"
#include "wvlog.h"

class WvPamData;

class WvPam
{
private:
    bool init();
    WvPamData *d;
    WvLog log;
    WvString appname;
    
    /**
     * Log the result of the last PAM step, based on the pam_status flag,and
     * write a failure message to the cloned stream on error.  step is the
     * name to use in the log message.  Returns true if the last step
     * succeeded, false if it failed.
     */
    bool check_pam_status(WvStringParm step);
    
public:
    /**
     * Start up PAM (presumably you will want to call authenticate()
     * later.
     */      
    WvPam(WvStringParm svcname);
    
    /**
     * Start up PAM, and authenticate user from rhost with password
     */
    WvPam(WvStringParm svcname, WvStringParm rhost,
	  WvStringParm user = WvString::null,      
	  WvStringParm password = WvString::null);
    
    virtual ~WvPam();
    
    /**
     * Authenticate the user from rhost with password.
     */
    bool authenticate(WvStringParm rhost = WvString::null,
		      WvStringParm user = WvString::null, 
		      WvStringParm password = WvString::null);
    
    /**
     * Get the groups that the currently sessioned user is logged
     * in with
     */       
    void getgroups(WvStringList &groups) const;
    
    /**
     * Get the user's name
     */
    WvString getuser() const;
    
    /**
     * Check here to see if the user is validated or not
     */
    WvError err;
};

#endif // __WVPAM_H
