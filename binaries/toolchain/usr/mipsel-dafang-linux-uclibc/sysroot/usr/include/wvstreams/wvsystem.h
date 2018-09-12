/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 */
#ifndef __WVSYSTEM_H
#define __WVSYSTEM_H

#include "wvsubproc.h"

/**
 * WvSystem is a mostly-replacement for the libc system() function call,
 * which people usually use because of its notational convenience, not
 * because it calls the Unix shell.  In fact, some people don't even realize
 * it calls the shell, leading to security holes when people forget to
 * quote user-provided parameters correctly.
 * 
 * WvSystem() uses WvSubProc but makes sure it can be called in a single
 * line of C++ code with a minimum of fluff.  For example:
 * 
 *    WvSystem("rm", "-rf", filename, NULL);
 * is like
 *    system(WvString("rm -rf %s", filename));
 * except that you don't have weird security bugs if "filename" contains
 * special characters like newline, space, quotation mark, etc.
 * 
 * See WvSubProc and WvSubProcQueue for less concise, but more flexible ways
 * to run subprograms.
 */
class WvSystem : private WvSubProc
{
public:
    /**
     * Construct a WvSystem from a simple list of strings.
     * 
     * For example:
     *    WvSystem("rm", "-rf", dirname);
     * 
     * Note: this is unlike WvSubProc::prepare(cmd, ...) because you
     * don't need to provide argv[0] yourself.  "cmd" is automatically
     * inserted as argv[0].  It also lets you pass WvString objects in
     * without manually calling cstr(), because it doesn't use varargs.
     * Unfortunately, that means it's limited to 20 arguments.
     */
    WvSystem(const char cmd[],
	     const char *a0 = NULL,
	     const char *a1 = NULL,
	     const char *a2 = NULL,
	     const char *a3 = NULL,
	     const char *a4 = NULL,
	     const char *a5 = NULL,
	     const char *a6 = NULL,
	     const char *a7 = NULL,
	     const char *a8 = NULL,
	     const char *a9 = NULL,
	     const char *a10 = NULL,
	     const char *a11 = NULL,
	     const char *a12 = NULL,
	     const char *a13 = NULL,
	     const char *a14 = NULL,
	     const char *a15 = NULL,
	     const char *a16 = NULL,
	     const char *a17 = NULL,
	     const char *a18 = NULL,
	     const char *a19 = NULL
	     )
    {
	// this function is inline so it can be a little bit less wasteful...
	const char * const argv[] = {
	    cmd,
	    a0, a1, a2, a3, a4, a5, a6, a7, a8, a9,
	    a10, a11, a12, a13, a14, a15, a16, a17, a18, a19,
	    NULL
	};
	init(argv);
    }

    /**
     * Construct a WvSystem from an argv array.  This is exactly the same
     * as WvSubProc's argv[] constructor, but the command name is always
     * taken from argv[0] rather than provided separately.
     * 
     * For example:
     *     const char *argv[] = { "rm", "-rf", dirname, NULL };
     *     WvSystem(argv);
     */
    WvSystem(const char * const *argv)
        { init(argv); }
    
    /**
     * Destroy the WvSystem object.  If you haven't yet called go(), the
     * command is run before destruction.
     */
    virtual ~WvSystem();
    
    /**
     * Explicitly start the command running and wait for it to finish.
     * This will happen automatically at object destruction time, but if you
     * want to check the return code, you'll need to call go().
     */
    int go();
    
    /** Redirect stdin from the given input file. */
    WvSystem &infile(WvStringParm filename);
    
    /** Redirect stdout to the given output file, which is overwritten. */
    WvSystem &outfile(WvStringParm filename);
    
    /** Redirect stderr to the given output file, which is overwritten. */
    WvSystem &errfile(WvStringParm filename);

private:
    bool started;
    WvString fdfiles[3]; // stdin, stdout, stderr
    
    void init(const char * const *argv);
    virtual int fork(int *waitfd);
};


#endif // __WVSYSTEM_H
