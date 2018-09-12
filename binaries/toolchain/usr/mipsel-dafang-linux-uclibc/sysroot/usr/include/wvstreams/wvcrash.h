/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Routines to generate a stack backtrace automatically when a program
 * crashes.
 */
#ifndef __WVCRASH_H
#define __WVCRASH_H

#include <sys/types.h>

void wvcrash_setup(const char *_argv0, const char *_desc = 0);
void wvcrash(int sig);
void wvcrash_add_signal(int sig);

// Leave a last will and testament in the WvCrash, if your program dies.
void wvcrash_leave_will(const char *will);
// Read the will back.
const char *wvcrash_read_will();
// Read the assertion back.
const char *wvcrash_read_assert();


class IWvStream;

struct WvCrashInfo
{
    // This is kind of ugly and used only for the guts of WvStreams,
    // but it's a significant rather than a premature optimization,
    // unfortunately.
    static IWvStream *in_stream;
    static const char *in_stream_id;
    static enum InStreamState {
	UNUSED,
	PRE_SELECT,
	POST_SELECT,
	EXECUTE,
    } in_stream_state;
};

const int wvcrash_ring_buffer_order = 12;
const int wvcrash_ring_buffer_size = 1 << wvcrash_ring_buffer_order;
void wvcrash_ring_buffer_put(const char *str);
void wvcrash_ring_buffer_put(const char *str, size_t len);
const char *wvcrash_ring_buffer_get();

#if defined(_WIN32)
extern void setup_console_crash();
#endif

#endif // __WVCRASH_H
