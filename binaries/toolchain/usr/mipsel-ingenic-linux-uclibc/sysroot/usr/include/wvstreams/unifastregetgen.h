/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002-2005 Net Integration Technologies, Inc.
 */
#ifndef __UNIFASTREGETGEN_H
#define __UNIFASTREGETGEN_H

#include "unifiltergen.h"

class UniConfValueTree;

/**
 * A lightwight but slightly dangerous variant of UniCacheGen.
 * 
 * UniCacheGen tries to make all operations asynchronous, by storing a local
 * cache of absolutely everything in the entire UniConf subtree that it wraps.
 * UniFastRegetGen, on the other hand, tries to be helpful without actually
 * wasting too much memory, but leaves many operations still synchronous.
 * 
 * This is useful for programs that may need to access a variety of different
 * UniConf keys, don't need to worry *too* much about blocking occasionally
 * while waiting for UniConf, but would like most get() operations to be
 * fast.
 * 
 * UniFastRegetGen makes subsequent get() operations on a key, after the
 * first one, be guaranteed asynchronous.  It caches keys which you've
 * previously done get() on, and accepts notifications for those keys
 * (so the value is always up to date, but you'll never have to wait for it
 * again).  It makes no attempt to optimize any other operations, including
 * set(), refresh/commit, or iterate.  It ignores notifications for any
 * keys you haven't explicitly done get() on in the past.
 */
class UniFastRegetGen : public UniFilterGen
{
public:
    UniFastRegetGen(IUniConfGen *_inner);
    virtual ~UniFastRegetGen();

    /***** Overridden members *****/
    virtual WvString get(const UniConfKey &key);
    virtual bool exists(const UniConfKey &key);
    virtual bool haschildren(const UniConfKey &key);

private:
    IUniConfGen *inner;
    UniConfValueTree *tree;
    
protected:
    virtual void gencallback(const UniConfKey &key, WvStringParm value);
};


#endif // __UNIFASTREGETGEN_H
