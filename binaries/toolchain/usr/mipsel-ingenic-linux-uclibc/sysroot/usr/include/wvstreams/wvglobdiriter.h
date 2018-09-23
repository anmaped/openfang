/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Directory iterator.  Recursively uses opendir and readdir, so you don't
 * have to.  Basically implements 'find'.
 *
 */

#ifndef __WVGLOBDIRITER_H
#define __WVGLOBDIRITER_H

#include "wvdiriter.h"

class WvGlob;

class WvGlobDirIter : public WvDirIter
{

private:

    WvGlob          *glob;

public:

    WvGlobDirIter( WvStringParm dirname, WvStringParm globstr,
	       bool _recurse = true, bool _skip_mounts = false,
	       size_t sizeof_stat = sizeof(struct stat) );
    ~WvGlobDirIter();

    bool next();

};

#endif
