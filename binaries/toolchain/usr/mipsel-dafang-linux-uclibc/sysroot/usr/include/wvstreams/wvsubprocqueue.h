/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A class for running a series or set of processes, one at a time.
 */
#ifndef __WVSUBPROCQUEUE_H
#define __WVSUBPROCQUEUE_H

#include "wvsubproc.h"

/**
 * An ordered queue of WvSubProc instances.
 * 
 * You can enqueue any number of subprocesses to run sequentially, with
 * a specified maximum number of processes running at a time.  The processes
 * run in the order they are added to the queue, except that (of course)
 * running more than one process at a time may cause processes to overlap
 * in an undefined way.
 * 
 * You can define "sync point" subprocesses using the "cookie" parameter to
 * add().  A sync point is guaranteed to be started:
 *  - alone, not in parallel with anything else
 *  - at least once after you enqueue it
 *  - at least once after every already-enqueued process has finished
 *  - immediately after every already-enqueued process has finished, *unless*
 *    an earlier sync point is still waiting.
 *  - no more than twice after the final time it is enqueued.
 * 
 * That sounds complicated, but it lets you easily implement a common
 * type of "delayed event" queuing: "Some time after each of these
 * things S, I need to run X, but I don't need to run it for *every*
 * element of S, but I don't want it to wait forever because I keep adding
 * to S."
 * 
 * For example, imagine you have one type of operation, add() that
 * creates a new file in a directory, and another type of operation
 * that regenerates the directory index.  You need to generate the
 * index after any create operation before it will really be done,
 * but there is no need to generate the index more than once when
 * doing a lot of create operations.  You could do a sequence like
 * this:
 * 
 *        for (i = 0; i < 10; i++)
 *            { add_file(i); }
 * 
 * Where "add" can trivially just do q.add(NULL, whatever1) and then
 * q.add(&reindex_cookie, whatever2).  WvSubProcQueue
 * will ensure that "whatever2" runs as soon as possible (to prevent
 * arbitrary delays because of nonstop add_file() calls) and also exactly
 * once at the very end, but not every single time.
 * 
 * In case it wasn't obvious, if you create more than one
 * WvSubProcQueue, they operate totally independently of each other.  That
 * means if you have two queues with a max of 10 processes, you might have
 * up to 20 processes running at a time.
 */
class WvSubProcQueue
{
public:
    /**
     * Create a WvSubProcQueue.  _maxrunning is the maximum number of
     * processes to have running in parallel.  1 is usually a good choice.
     */
    WvSubProcQueue(unsigned _maxrunning);
    
    virtual ~WvSubProcQueue();
    
    /**
     * Enqueue a process.  If cookie is NULL, the process will simply
     * be added at the end of the queue.  If cookie is non-NULL, it will
     * be treated as a "sync point" as described above.
     * 
     * WARNING!  Do not start_again() the proc before passing it to the
     * WvSubProcQueue.  This is done automatically in some WvSubProc
     * constructors.  Use WvSubProc::prepare() or preparev() instead.
     */
    void add(void *cookie, WvSubProc *proc);
    
    /**
     * Like add(cookie, proc) but you don't have to build the WvSubProc
     * yourself for simple cases.
     */
    void add(void *cookie, const char *cmd, const char * const *argv);
    
    /**
     * Clean up after any running processes in the queue, and start running
     * additional processes if any are waiting.  Never blocks.
     * 
     * Returns the number of new processes which were started on this run.
     * 
     * WARNING: you must call this rather often in order to keep your
     * queue moving.
     */
    int go();
    
    /**
     * Wait synchronously for all processes in the entire queue to finish.
     * This might block forever!!  You should probably only call it in
     * test programs or if you really know what you're doing.  Otherwise
     * just call go() occasionally.
     */
    void finish();

    /// Return the number of currently running processes.
    unsigned running() const;
    
    /// Return the number of unfinished (ie. running or waiting) processes.
    unsigned remaining() const;
    
    /// True if there are no unfinished (ie. running *or* waiting) processes.
    bool isempty() const;
    
private:
    struct Ent
    {
	Ent(void *_cookie, WvSubProc *_proc)
	{
	    cookie = _cookie;
	    proc = _proc;
	    redo = false;
	}
	
	~Ent()
	{
	    if (proc) delete proc;
	}
	
	void *cookie;
	WvSubProc *proc;
	bool redo;
    };
    DeclareWvList(Ent);
    
    unsigned maxrunning;
    EntList runq, waitq;

    bool cookie_running();
};


#endif // __WVSUBPROCQUEUE_H

