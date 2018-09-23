/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 *
 * A UniConf generator that caches keys/values in memory.
 */
#ifndef __UNIREPLICATEGEN_H
#define __UNIREPLICATEGEN_H

#include "uniconftree.h"
#include "wvlog.h"

/**
 * A UniConf generator that replicates generators between an ordered list
 * of inner generators, with the priority given by the list.
 *
 * Replication of the data occurs when the generator is contructed, whenever
 * prepend() or append() is called, or whenever any of the generators in the
 * list goes from !isok() to isok().  If two inner generators contain different
 * values for the same before replication, then they will both have the value
 * of the earlier generator (as ordered in the moniker) after replication.
 *
 * If an asynchronous change occurs in any of the inner generators, the
 * new value will be set in all generators irrespective of priority.
 */
class UniReplicateGen : public UniConfGen
{
private:
    struct Gen
    {
    	IUniConfGen *gen;
    	bool was_ok;
    	bool auto_free;
    	
    	Gen(IUniConfGen *_gen, bool _auto_free)
    	    	: gen(_gen), was_ok(gen->isok()), auto_free(_auto_free) {}
    	~Gen() { if (auto_free) WVRELEASE(gen); }
    	
    	bool isok() { return was_ok = gen->isok(); }
    };
    DeclareWvList(Gen);
    GenList gens;
    
    bool processing_callback;

    Gen *first_ok() const;
    
    void replicate_if_any_have_become_ok();
    
protected:
    void replicate(const UniConfKey &key = "/");
    void deltacallback(Gen *src_gen, const UniConfKey &key,
		       WvStringParm value);

public:
    UniReplicateGen();
    UniReplicateGen(const IUniConfGenList &_gens, bool autofree = true);
    virtual ~UniReplicateGen();
    
    void prepend(IUniConfGen *gen, bool autofree = true);
    void append(IUniConfGen *gen, bool autofree = true);

    /***** Overridden members *****/
    virtual bool isok();
    virtual void commit();
    virtual bool refresh();
    virtual void flush_buffers() { }
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual WvString get(const UniConfKey &key);
    virtual Iter *iterator(const UniConfKey &key);
};

#endif // __UNIREPLICATEGEN_H
