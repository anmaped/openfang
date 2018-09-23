/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 * 
 * A UniConfGen framework to simplify writing filtering generators.
 */
#ifndef __UNIFILTERGEN_H
#define __UNIFILTERGEN_H

#include "uniconfgen.h"

/**
 * A UniConfGen that delegates all requests to an inner generator.  If you
 * derive from this, you can selectively override particular behaviours
 * of a sub-generator.
 */
class UniFilterGen : public UniConfGen
{
    IUniConfGen *xinner;

protected:
    UniFilterGen(IUniConfGen *inner);
    virtual ~UniFilterGen();

    /**
     * Rebinds the inner generator and prepares its callback.
     * The previous generator is NOT destroyed.
     */
    void setinner(IUniConfGen *inner);

public:
    /** Returns the inner generator. */
    IUniConfGen *inner() const
        { return xinner; }
    
    /**
     * A mapping function for filters that remap one keyspace onto another.
     * 
     * The default implementation of the various functions (get, set,
     * exists, etc) run their keys through this function before forwarding
     * the requests on to the inner generator.
     * 
     * The default implementation of this function doesn't change the key.
     *
     * Returns true if the key can be mapped, else false.
     */
    virtual bool keymap(const UniConfKey &unmapped_key, UniConfKey &mapped_key);

    /**
     * A mapping function for filters that unmap a keyspace.
     *
     * The default implementation of this function doesn't change the key.
     * 
     * Returns true if the key can be reverse-mapped, else false.
     */
    virtual bool reversekeymap(const UniConfKey &mapped_key, UniConfKey &unmapped_key);
    

    /***** Overridden methods *****/

    virtual void commit();
    virtual bool refresh();
    virtual void flush_buffers();
    virtual void prefetch(const UniConfKey &key, bool recursive);
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual bool exists(const UniConfKey &key);
    virtual bool haschildren(const UniConfKey &key);
    virtual bool isok();
    virtual Iter *iterator(const UniConfKey &key);
    virtual Iter *recursiveiterator(const UniConfKey &key);

protected:
    /**
     * Called by inner generator when a key changes.
     * The default implementation calls delta(key).
     */
    virtual void gencallback(const UniConfKey &key, WvStringParm value);
};

#endif //__UNIFILTERGEN_H
