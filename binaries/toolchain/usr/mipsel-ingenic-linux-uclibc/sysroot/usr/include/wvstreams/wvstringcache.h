/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2005 Net Integration Technologies, Inc.
 *
 * Definition for the WvStringCache class.
 */
#ifndef __WVSTRINGCACHE_H
#define __WVSTRINGCACHE_H

#include "wvstringtable.h"

/**
 * A cache table of WvString objects.  If you think you might be reusing
 * the same string objects over and over (for example, because the user
 * provides the same string value a million times in a config file you're
 * reading), you might be able to save a lot of memory by sharing the
 * strings via a WvStringCache.
 * 
 * To potentially share a string, call get(string), throw away the input
 * string, and use the output string (which is guaranteed to have the same
 * content) in its place.  The string will be saved in the cache table for
 * next time.
 * 
 * Every once and a while, you should call clean() to remove any strings from
 * the table that are no longer referenced elsewhere.  This is especially
 * important after deleting a large data structure, because you won't actually
 * free up the memory used by those strings until clean() is called.
 * 
 * All WvStringCaches in the app are shared, to optimize the benefits of
 * the cache.
 */
class WvStringCache
{
    static WvStringTable *t;
    static int refcount;
    static size_t clean_threshold;
    
public:
    WvStringCache();
    ~WvStringCache();
    
    /** Get a shared string corresponding to 's'. */
    WvString get(WvStringParm s);
    
    /** Remove any now-unused strings from the cache. */
    void clean();
};


#endif // __WVSTRINGCACHE_H
