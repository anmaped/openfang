/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A simple lockfile class using WvStreams.
 */

#ifndef __WVLOCKFILE_H
#define __WVLOCKFILE_H

#include "wvfile.h"


class WvLockFile
{
public:
    WvLockFile(WvStringParm _lockname);

    /** Check to make sure no lock is established or that it's owned by us. */
    bool isok();

    /** Creates the lockfile with the current pid. Returns success/failure. */
    bool lock();

    /**
     * Removes the lockfile if present. If there's no lockfile after,
     * returns true, otherwise false.
     */
    bool unlock();

    /**
     * Returns one of three things:
     *   -1 if the lockfile exists, but is inaccessible.
     *   0  if there is no lockfile, or the process is not running.
     *   >0 The pid of the known-running process that owns the lock.
     */
    pid_t readpid();

protected:
    WvString lockname;
};

#endif // __WVLOCKFILE_H
