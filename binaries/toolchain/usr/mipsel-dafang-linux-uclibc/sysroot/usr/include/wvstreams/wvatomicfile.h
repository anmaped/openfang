/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2005 Net Integration Technologies, Inc.
 *
 * A simple class to access filesystem files using WvStreams.
 */
#ifndef __WVATOMFILE_H
#define __WVATOMFILE_H

#include "wvfile.h"

/**
 * WvAtomicFile implements a simple extension to wvfile to allow for 
 * atomic file creation.  Files normally can be created and written 
 * to, however, in the event of a kernel panic the file can be left in
 * an unusable state.
 *
 * A WvAtomicFile is atomically created on file close
 *
 */
class WvAtomicFile : public WvFile
{
private:
    WvString atomic_file;   
    WvString tmp_file;

public:
    WvAtomicFile(WvStringParm filename, int flags = O_TRUNC | O_CREAT, 
                    mode_t create_mode = 0666);
    ~WvAtomicFile();

    bool open(WvStringParm filename, int flags = O_TRUNC | O_CREAT, 
                mode_t create_mode = 0666);
    void close();
    
    // Like chmod(2), does *not* respect umask
    bool chmod(mode_t mode);
    
    bool chown(uid_t owner, gid_t group);

public:
    const char *wstype() const { return "WvAtomicFile"; }
};

#endif // __WVATOMFILE_H
