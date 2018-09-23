/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2004 Net Integration Technologies, Inc.
 * 
 * WvStreams implementation of ptys under Linux.
 *
 * For more information on programming ptys, see chapter 19 of
 * Stevens' "Advanced Programming in the UNIX Environment"
 */
#ifndef __WVPTY_H
#define __WVPTY_H

#include "wvfdstream.h"
#include "wvtr1.h"

class WvPty: public WvFDStream
{
private:
    WvString _master, _slave;
    pid_t _pid;
    int _exit_status;

    static bool open_pty(WvString &master, int &master_fd,
			 WvString &slave, int &slave_fd);

    void monitor_child(bool wait);
        
public:
    typedef wv::function<bool(WvPty&)> Callback;

    Callback pre_exec_cb;
    Callback post_exec_cb; // This can only be called if exec() fails

    WvPty(const char *program, const char * const *argv,
	  Callback _pre_exec_cb = Callback(),
	  Callback _post_exec_cb = Callback());

    void kill(int signum);
    bool child_exited();
    bool child_killed();
    int finish();
    int exit_status();
        
    const char *master() const
	{ return _master; }
    const char *slave() const
	{ return _slave; }
    pid_t pid() const
	{ return _pid; }
            
    const char *wstype() const { return "WvPty"; }
};

#endif // __WVPTY_H
