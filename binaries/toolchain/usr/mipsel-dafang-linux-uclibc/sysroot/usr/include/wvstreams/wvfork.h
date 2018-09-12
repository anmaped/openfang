/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 */
/** \file
 * Provides support for forking processes.
 */

#ifndef __WVFORK_H
#define __WVFORK_H

#ifndef _WIN32
#include <unistd.h>
#else
typedef int pid_t;
#endif

#include "wvscatterhash.h"
#include "wvtr1.h"

DeclareWvScatterTable(int);
typedef wv::function<void(pid_t)> WvForkCallback;

/**
 * Register a callback to be called during wvfork.
 * It will be called (in both parent and child process) after the fork has
 * happened but before wvfork returns.  It is passed the return value of the
 * fork.
 * 
 * NOTE: There is no way to undo this operation!
 */
extern void add_wvfork_callback(WvForkCallback cb);

/**
 * wvfork_start is just like fork, except that it will block the
 * parent until the child process closes the waitfd, to avoid race
 * conditions.
 *
 * For example, wvfork uses it, closing the waitfd only when it is
 * done closing the close-on-exec file descriptors.
 */
extern pid_t wvfork_start(int *waitfd);

/**
 * wvfork() just runs fork(), but it closes all file descriptors that
 * are flagged close-on-exec, since we don't necessarily always run
 * exec() after we fork()...
 *
 * This fixes the year-old mystery bug where WvTapeBackup caused
 * watchdog reboots because the CHILD process wasn't touching it, and
 * it was already open before the fork()...
 */
extern pid_t wvfork(int dontclose1 = -1, int dontclose2 = -1);
extern pid_t wvfork(intTable &dontclose );

#endif
