/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 * 
 * A UniConfGen that makes everything slow.
 */
#ifndef __UNISLOWGEN_H
#define __UNISLOWGEN_H

#include "unifiltergen.h"

/**
 * A UniConfGen that counts all "potentially synchronous" (ie. slow)
 * operations, so you'll know for sure if you're calling UniConf synchronous
 * operations when you shouldn't be.  Hint: wrapping this one in a
 * UniCacheGen is supposed to cut slow operations to a bare minimum, so a
 * good test is to try a moniker like this:
 * 
 *   cache:slow:tcp:localhost
 * 
 * ...and see if everything still works but there are no slow operations.
 * 
 * (A previous version of this used to actually make things slow by doing
 * a deliberate sleep() on synchronous operations, but that wasn't really
 * needed.  Now it just logs them instead.)
 */
class UniSlowGen : public UniFilterGen
{
public:
    UniSlowGen(IUniConfGen *inner);
    virtual ~UniSlowGen();

    virtual void commit();
    virtual bool refresh();
    virtual void flush_buffers() { }
    virtual WvString get(const UniConfKey &key);
    virtual bool exists(const UniConfKey &key);
    virtual bool haschildren(const UniConfKey &key);
    virtual Iter *iterator(const UniConfKey &key);
    virtual Iter *recursiveiterator(const UniConfKey &key);
    
    int how_slow() const
        { return slowcount; }
    
    void reset_slow()
        { slowcount = 0; }
    
private:
    int slowcount;
    
    void be_slow(WvStringParm what);
    void be_slow(WVSTRING_FORMAT_DECL)
        { be_slow(WvString(WVSTRING_FORMAT_CALL)); }
};

#endif //__UNISLOWGEN_H
