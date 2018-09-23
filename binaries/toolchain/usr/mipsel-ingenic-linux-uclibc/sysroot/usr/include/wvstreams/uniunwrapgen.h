/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2004 Net Integration Technologies, Inc.
 * 
 * A totally evil UniConfGen that "unwraps" a UniConf object by turning it
 * back into a UniConfGen.
 */
#ifndef __UNIUNWRAPGEN_H
#define __UNIUNWRAPGEN_H

#include "uniconf.h"

/**
 * Deprecated: a UniConfGen that delegates all requests to an inner UniConf.
 * This is sort of like UniFilterGen, except that it has the extremely
 * questionable behaviour of turning a UniConf (which normally wraps a
 * UniConfGen) back into a UniConfGen.  The problem is that IUniConfGen has
 * a stable API/ABI, while the UniConf object itself does not.
 * 
 * On the other hand, we get some free features by doing this: in particular,
 * since a UniConf object can have multiple owners, it can be wrapped by
 * multiple UniUnwrapGen objects, which is almost as good as a UniConfGen
 * having multiple owners.  Also, you can implement "symlinks" (mount a
 * subtree of your UniConf hierarchy somewhere else), "chroot" (export
 * only a subtree to clients, making sure they can't access above that point
 * in the tree), and other creative things.
 * 
 * FIXME: a much cleaner way to do this would be to move the
 * callback-multiplexing and subtree-accessing parts of UniConfRoot into their
 * own generators, so that we could use these features without going through
 * a UniConfRoot.
 */
class UniUnwrapGen : public UniConfGen
{
    UniConf xinner;
    UniConfKey xfullkey;

public:
    UniUnwrapGen(const UniConf &inner);
    virtual ~UniUnwrapGen();

    void setinner(const UniConf &inner);

    /** Returns the inner generator. */
    const UniConf &inner() const
        { return xinner; }

    /***** Overridden methods *****/
    virtual void commit();
    virtual bool refresh();
    virtual void flush_buffers() { }
    virtual void prefetch(const UniConfKey &key, bool recursive);
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual bool exists(const UniConfKey &key);
    virtual bool haschildren(const UniConfKey &key);
    virtual bool isok();
    virtual Iter *iterator(const UniConfKey &key);
    virtual Iter *recursiveiterator(const UniConfKey &key);

private:
    /**
     * Called by inner generator when a key changes.
     * The default implementation calls delta(key).
     */
    virtual void gencallback(const UniConfKey &key, WvStringParm value);
    
    /** Like xinner[key], but skips calling [] if key.isnull(). */
    UniConf _sub(const UniConfKey &key);
    
    class Iter;
    class RecursiveIter;
    
    bool refreshing, committing;
};

#endif //__UNIUNWRAPGEN_H
