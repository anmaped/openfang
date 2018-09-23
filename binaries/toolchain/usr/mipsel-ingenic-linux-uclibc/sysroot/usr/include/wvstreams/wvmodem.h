/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 *   Copyright (C) 1999 Red Hat, Inc.
 *
 * Definition of the WvModemBase and WvModem classes.  Inherit from WvFile,
 * but do various important details related to modems, like setting baud
 * rates and dropping DTR and the like.
 *
 */

#ifndef __WVMODEM_H
#define __WVMODEM_H

#include "wvlockdev.h"
#include "wvfile.h"
#include "wvlog.h"
#include <termios.h>

#ifndef IUCLC
#define IUCLC 0
#endif

#ifndef OLCUC
#define OLCUC 0
#endif

#ifndef XCASE
#define XCASE 0
#endif

/**
 * WvModemBase provides the methods used to control a modem, but
 * without real implementation for most of them, so that they can
 * be used in contexts where modem control is undesirable without
 * reimplementing calling code for such uses.
 */
class WvModemBase : public WvFile
{
protected:
    struct termios	t;
    int			baud;

    WvModemBase() { }
    
    int get_real_speed();

public:
    bool die_fast;
    
    WvModemBase(int _fd);
    virtual ~WvModemBase();
    
    /** do-nothing method that is not needed in WvModemBase */
    virtual void close();

    /** do-nothing method that is not needed in WvModemBase */
    virtual bool carrier();

    /** do-nothing method that is not needed in WvModemBase */
    virtual int speed(int _baud);

    /** this one really is needed */
    int getspeed()
	{ return baud; }

    /** may need to hangup for redial reasons */
    virtual void hangup();
    
public:
    const char *wstype() const { return "WvModemBase"; }
};


/**
 * WvModem implements a named modem that really needs to be opened,
 * closed, and manipulated in lots of ways
 */
class WvModem : public WvModemBase
{
private:
    WvLockDev		lock;
    WvLog               log;
    bool                have_old_t;
    struct termios	old_t;
    bool		closing;
    bool                no_reset;
    
    /**
     * Setup all of the terminal characteristics, and leave the modem in CLOCAL
     * mode to make sure that we can communicate easily with the modem later.
     */
    void setup_modem(bool rtscts);
    
    /** Check the status of the modem */
    int getstatus();
    
public:
    WvModem(WvStringParm filename, int _baud, bool rtscts = true, 
	    bool _no_reset = false);
    virtual ~WvModem();
    
    /** Close the connection to the modem */
    virtual void close();
    
    /** Is there a carrier present? */
    virtual bool carrier();
    
    /**
     * _baud is the desired speed, that you wish the modem to communicate with,
     * and this method returns the actual speed that the modem managed to achieve.
     */
    virtual int speed(int _baud);
    
public:
    const char *wstype() const { return "WvModem"; }
};

#endif
