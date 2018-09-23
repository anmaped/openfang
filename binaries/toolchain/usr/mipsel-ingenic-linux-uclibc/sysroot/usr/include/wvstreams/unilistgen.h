/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * UniListGen is a UniConf generator to allow multiple generators to be
 * stacked in a priority sequence for get/set/etc.
 *
 */

#ifndef __UNICONFLISTGEN_H
#define __UNICONFLISTGEN_H

#include "uniconfgen.h"
#include "wvscatterhash.h"

/**
 * Accepts a list of UniConf generators, and stacks them, treating them as one
 * uniconf source.
 *
 * The standard way of using the list generator would be with a moniker. The
 * moniker takes the form of list:(tcl style string list).
 *
 * For example: list:readonly:ini:admin.ini ini:user.ini
 *
 * The list can also contain a list. This still uses tcl style string lists as
 * follows:
 *
 * list:readonly:ini:admin.ini list:{ini:user1.ini ini:user2.ini} ini:def.ini
 */
class UniListGen : public UniConfGen
{
public:
    UniListGen(UniConfGenList *_l);
    virtual ~UniListGen();

    UniConfGenList *l;

    /***** Overridden members *****/

    virtual void commit(); 
    virtual bool refresh();
    virtual void flush_buffers() { }
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual bool exists(const UniConfKey &key);
    virtual bool haschildren(const UniConfKey &key);
    virtual bool isok();
    virtual Iter *iterator(const UniConfKey &key);

    class IterIter;

protected:
    /**
     * Called by first generator when a key changes.
     * The default implementation calls delta(key).
     */
    virtual void gencallback(const UniConfKey &key, WvStringParm value);
};


#endif // __UNICONFLISTGEN_H
