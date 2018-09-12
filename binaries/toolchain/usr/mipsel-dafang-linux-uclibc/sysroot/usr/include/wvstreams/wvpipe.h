/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Provides support for piping data to/from subprocesses.
 */ 
#ifndef __WVPIPE_H
#define __WVPIPE_H

#include "wvfdstream.h"
#include "wvsubproc.h"

/**
 * Implementation of a WvPipe stream.  These allow you to create a new
 * process, attaching its stdin/stdout to a WvStream.
 * 
 * Unlike pipes created with the popen() system call, you can capture
 * both stdin and stdout for the given process.  This is because we
 * actually use the socketpair() call instead.  If you try this,
 * however, you must be very careful to always use the select() call
 * before reading from the stream.  (Think what would happen if both
 * ends of the pipe do a read() simultaneously!)
 * 
 * Note that we do not go as far as actually using a pty.  That means
 * programs which deliberately open /dev/tty will not be redirected.
 * 
 * When the WvPipe is destroyed, it makes sure that the child process
 * is killed.  Since it tries to do it politely (SIGTERM, wait up to
 * 2 seconds, SIGKILL) it can take up to 2 seconds to destroy a
 * WvPipe.
 */
class WvPipe : public WvFDStream
{
    WvSubProc proc;
protected:
    void setup(const char *program, const char * const *argv,
	       bool writable, bool readable, bool catch_stderr,
	       int stdin_fd, int stdout_fd, int stderr_fd,
	       WvStringList *env);
public:
    /**
     * default pipe constructor; if you just want to use a pipe, use this.
     * For each of stdin, stdout, and stderr of the child process, it can
     * do one of three things:
     *    - leave it alone (ie. the same as for the parent process)
     *    - redirect it through the WvPipe (eg. if writable==true)
     *    - redirect it to any open file descriptor (std*_fd are only
     *       used if the corresponding bool is false, however)
     * Note that you need either writable or readable set to true if you
     * want the pipe to close automatically (for instance, when it's
     * appened to the globallist).  Use the ignore_read() callback if
     * you really don't care about its output.
     */
    WvPipe(const char *program, const char * const *argv,
	   bool writable, bool readable, bool catch_stderr,
	   int stdin_fd = 0, int stdout_fd = 1, int stderr_fd = 2,
           WvStringList *env = NULL);
    
    /**
     * This constructor does much the same thing as the previous one,
     * except that std*_str are WvStreams instead.  The target process
     * accesses the 'fd' member of the stream (NOT using
     * the WvStream read() and write() functions).
     *
     * Again, we only redirect to the given WvStreams if the corresponding
     * bool is false; otherwise, we redirect to the pipe.
     *
     * It is okay for the same WvStream to occur more than once.  Also,
     * you must naturally make sure that the stream doesn't disappear
     * before WvPipe does!
     */
    WvPipe(const char *program, const char * const *argv,
	   bool writable, bool readable, bool catch_stderr,
	   WvFDStream *stdin_str, WvFDStream *stdout_str = NULL,
	   WvFDStream *stderr_str = NULL, WvStringList *env = NULL);
    
    /**
     * This constructor is the same again, except that it uses the features
     * of the WvFDStream class to get all its fds from one place.
     */
    WvPipe(const char *program, const char **argv,
	   bool writable, bool readable, bool catch_stderr,
	   WvFDStream *stdio_str, WvStringList *env = NULL);

    /** kill the child process and close the stream. */
    virtual ~WvPipe();

    /**
     * send the child a signal
     * (signal names are defined in signal.h)
     */
    void kill(int signum);
    
    /** wait for child to die.  Returns exit_status() */
    int finish(bool wait_children = true);
    
    /** returns true if child is dead. */
    bool child_exited();

    /** returns true if child is dead because of a signal. */
    bool child_killed() const;
    
    /**
     * returns the exit status:
     *   if child_killed()==true, the signal that killed the child.
     *   if child_killed()==false, the return code of the program.
     */
    int exit_status();

    // returns pid
    int getpid() const { return proc.pid; };

    // callback to ignore everything.  see comment in wvpipe.cc.
    static void ignore_read(WvStream &s);
    
public:
    const char *wstype() const { return "WvPipe"; }
};

#endif // __WVPIPE_H
