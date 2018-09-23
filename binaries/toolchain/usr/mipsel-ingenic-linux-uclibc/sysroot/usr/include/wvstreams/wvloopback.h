/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */ 
#ifndef __WVLOOPBACK_H
#define __WVLOOPBACK_H

#include "wvfdstream.h"

/**
 * Implementation of a WvLoopback stream.  WvLoopback uses a
 * socketpair() to create a stream that allows you to read()
 * everything written to it, even (especially) across a fork() call.
 */
class WvLoopback : public WvFDStream
{
public:
    WvLoopback();

public:
    const char *wstype() const { return "WvLoopback"; }
};

#endif // __WVLOOPBACK_H
