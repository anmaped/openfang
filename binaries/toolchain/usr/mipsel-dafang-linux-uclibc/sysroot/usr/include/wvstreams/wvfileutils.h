/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2005 Net Integration Technologies, Inc.
 *
 * Various little file functions...
 *
 */

#ifndef __WVFILEUTILS_H
#define __WVFILEUTILS_H

#include "wvstring.h"
#include "wvstringlist.h"

/**
 * Like mkdir(), but has the same parameters in both Unix and Windows.
 */
int wvmkdir(WvStringParm _dir, int create_mode = 0700);

/**
 * Create a directory and any subdirectories required along the way.
 * (Equivalent to mkdir -p).
 * 
 * The default permissions on created directories is 0700, but this can be
 * changed at will.
 */
int mkdirp(WvStringParm _dir, int create_mode = 0700);

/**
 * Safely remove an entire filesystem directory hierarchy.
 * (Equivalent to rm -rf).  Just like "rm -rf", it may or may not successfully
 * delete everything.  It's your job to check that afterwards.
 */
void rm_rf(WvStringParm _dir);

/**
 * Copy from src to dst preserving permissions and time stamp. This does not
 * preserve ownership, however.
 *
 * Two versions of this are provided. One for giving two filenames/paths, and
 * another for giving two starting directories and a relative path from there.
 */
bool fcopy(WvStringParm src, WvStringParm dst);
bool fcopy(WvStringParm srcdir, WvStringParm dstdir, WvStringParm relname);

/**
 * Create the file if it doesn't exist, or update the file's modification and
 * access times if it does.
 * (Equivalent to touch).
 */
bool ftouch(WvStringParm file, time_t mtime = 0);

/**
 * Reads the contents of a symlink.  Returns the contents, or 
 * WvString::null on error.
 */
WvString wvreadlink(WvStringParm path);

/**
 * Check whether two files have the same date/time stamp. This can be used as a
 * quick check whether files are unchanged / the same, though obviously it
 * doesn't verify that they are indeed the same file.
 *
 * Two versions are provided, one for giving two files, and another for giving
 * two starting directories and a relative path from there.
 */
bool samedate(WvStringParm file1, WvStringParm file2);
bool samedate(WvStringParm dir1, WvStringParm dir2, WvStringParm relname);

/**
 * Runs fnmatch against everything in the patterns list.  We also interpret
 * .cvsignore-style '!' patterns, which makes us very fancy.
 */
#ifndef _WIN32
bool wvfnmatch(WvStringList &patterns, WvStringParm name, int flags = 0);
#endif

/**
 * Replacement for tmpfile() that works correctly in win32 as well as Unix.
 */
FILE *wvtmpfile();

/* Returns a unique filename suitable for a temporary file. Obviously there is
 * the caveat that someone else may claim this file name before you open it:
 * do not use this routine where that race may be a real concern (this would
 * apply only to security-sensitive code)
 */
WvString wvtmpfilename(WvStringParm prefix);

#ifndef _WIN32
/**
 * Basically our own implementation of the NetBSD lchmod() call.
 */
int wvchmod(const char *path, mode_t mode);
#endif

/**
 * A simple helper function to get the current umask.
 */
#ifndef _WIN32
mode_t get_umask();
#endif

#endif // __WVFILEUTILS_H
