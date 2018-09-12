/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */ 
#ifndef __WVLOOPBACK2_H
#define __WVLOOPBACK2_H

#include "wvfdstream.h"

/**
 * A two-way WvLoopback.  Uses a socketpair() to create two streams that
 * allow you to read() from one everything written to the other and vice
 * versa, even (especially) across a fork() call.
 * 
 * s1 and s2 are changed to point at the pair of streams.  There is no
 * condition in which this function returns without setting s1 and s2 to
 * valid non-NULL streams.
 * 
 * You are responsible for doing WVRELEASE on each of the returned streams.
 * 
 * In case of an error, nonfunctional streams are created and returned, and
 * they have their error codes set appropriately.
 * 
 * See also WvLoopback, WvBufStream.
 */
void wvloopback2(IWvStream *&s1, IWvStream *&s2);

#endif // __WVLOOPBACK2_H
