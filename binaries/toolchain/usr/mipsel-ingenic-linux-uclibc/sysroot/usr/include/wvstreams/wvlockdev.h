/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Some handy functions to create/remove /var/lock lockfiles.
 */
#ifndef __WVLOCKDEV_H
#define __WVLOCKDEV_H

#include "wvstring.h"

/**
 * Class to handle Lock files - useful for WvDial and other places where we 
 * need to guarantee exclusive access to a file or device.  Creates/Removes
 * lockfiles in /var/lock.
 */
class WvLockDev
{
    WvString devicename, filename;
    int lock_count;
public:
    WvLockDev(WvString _devicename);
    ~WvLockDev();
    
    bool lock();
    void unlock();
    bool islocked() const
       { return lock_count != 0; }
};

#endif // __WVLOCKDEV_H
