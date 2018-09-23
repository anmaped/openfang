/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A generator that is always empty and rejects changes.
 */
#ifndef __UNICONFNULL_H
#define __UNICONFNULL_H

#include "uniconfgen.h"

/**
 * A generator that is always empty and rejects changes.
 *
 * To mount, use the moniker "null:".
 *
 */
class UniNullGen : public UniConfGen
{
public:
    UniNullGen() { };
    virtual ~UniNullGen() { };

    /***** Overridden methods *****/

    virtual void flush_buffers() { }
    virtual WvString get(const UniConfKey &key) { return WvString::null; }
    virtual void set(const UniConfKey &key, WvStringParm value) { }
    virtual void setv(const UniConfPairList &pairs) { }
    virtual bool haschildren(const UniConfKey &key) { return false; }
    virtual Iter *iterator(const UniConfKey &key) { return new NullIter(); }
};


#endif // __UNICONFNULL_H
