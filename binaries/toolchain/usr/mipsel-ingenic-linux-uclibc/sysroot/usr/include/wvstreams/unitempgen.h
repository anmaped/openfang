/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 *
 * A UniConf generator that stores keys in memory.
 */
#ifndef __UNITEMPGEN_H
#define __UNITEMPGEN_H

#include "uniconfgen.h"
#include "uniconftree.h"
#include "wvstringcache.h"

/**
 * A UniConf generator that stores keys in memory.
 * 
 * Maintains a dirtyness indicator that is set whenever the contents
 * are changed.  Also dispatches notifications on such changes.
 */
class UniTempGen : public UniConfGen
{
    WvStringCache scache;

public:
    UniConfValueTree *root; /*!< the root of the tree */
    bool dirty; /*!< set whenever the tree actually changes */

    UniTempGen();
    virtual ~UniTempGen();

    /***** Overridden members *****/

    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual void flush_buffers() { };
    virtual bool haschildren(const UniConfKey &key);
    virtual Iter *iterator(const UniConfKey &key);
    virtual void commit();
    virtual bool refresh();

protected:
    void notify_deleted(const UniConfValueTree *node, void *);
};


#endif // __UNITEMPGEN_H
