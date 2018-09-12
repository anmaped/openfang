/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002-2005 Net Integration Technologies, Inc.
 * 
 * A simple implementation of a UniConfGen iterator.
 */
#ifndef __UNILISTITER_H
#define __UNILISTITER_H

#include "uniconfgen.h"
#include "wvscatterhash.h"
#include "wvstringcache.h"
#include "wvstringlist.h"

/**
 * An iterator that iterates through a constant list of keys.  This is
 * handy if you know the list of keys is relatively short and you don't
 * want to write your own iterator, and/or you know your own object state
 * might change during iteration, so you would have to pre-generate the list
 * of keys anyway.
 * 
 * The creator of the iter is responsible for filling the 'keys' and 'values'
 * lists by calling add().  If the 'values' list runs out of values before
 * 'keys', the remaining values will be retrieved from the given generator
 * (using get()) instead.
 */
class UniListIter : public UniConfGen::Iter
{
    IUniConfGen *gen;
    
    DeclareWvScatterTable(UniConfKey);
    UniConfKeyTable keylook;


    UniConfKeyList keys;
    WvStringList values;
    
    UniConfKeyList::Iter ki;
    WvStringList::Iter vi;
    
    WvStringCache scache;
    
public:
    UniListIter(IUniConfGen *_gen);
    
    /**
     * Add a key/value pair to the list that gets returned by this iterator.
     * If v is 'noval' (the default), the value of the returned item is
     * retrieved by calling get(k).  Otherwise the value is v.
     * 
     * This function should only be called by the creator of the iterator,
     * not the end user.
     */
    void add(const UniConfKey &k, WvStringParm v = WvString::null);
    
    /**
     * Automatically fill the contents of this iterator by calling add()
     * for each element of the 'source' iterator.  This is handy if the
     * source iterator might be unsafe (eg. can't handle set() type
     * operations on the generator without restarting the iteration).
     * 
     * This function should only be called by the creator of the iterator,
     * not the end user.
     */
    void autofill(IUniConfGen::Iter *source);

    /***** Overridden members *****/
    virtual void rewind();
    virtual bool next();
    virtual UniConfKey key() const;
    virtual WvString value() const;
};

#endif // __UNILISTITER_H
