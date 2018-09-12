/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A simple class to access filesystem files using WvStreams.
 */
#ifndef __WVFILE_H
#define __WVFILE_H

#include "wvfdstream.h"
#include <fcntl.h>

#ifdef _WIN32
#define O_NONBLOCK 0
#define O_LARGEFILE 0
#define fcntl(a,b,c)
#endif

/**
 * WvFile implements a stream connected to a file or Unix device.  We
 * include no support for operations like seek().  Since files are not
 * really streams, you probably do not need WvStream support for seekable
 * files; just use standard C I/O functions in that case.
 *
 * WvFile is primarily useful for Unix device files, which have defined
 * select() behaviour for example.
 */
class WvFile : public WvFDStream
{
public:
    /** Create an empty WvFile that you'll open later with open() */
    WvFile();

    /** Create a WvFile from an existing fd.  Not available in win32. */
    WvFile(int rwfd);

    /** Create a WvFile given options like ::open() */
    WvFile(WvStringParm filename, int mode, int create_mode = 0666);
    bool open(WvStringParm filename, int mode, int create_mode = 0666);
    bool open(int _rwfd);
    
    bool readable, writable;

    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);

public:
    const char *wstype() const { return "WvFile"; }
};

#endif // __WVFILE_H
