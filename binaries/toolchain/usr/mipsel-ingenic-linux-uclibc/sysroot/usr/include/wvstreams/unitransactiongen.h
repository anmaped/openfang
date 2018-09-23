/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 *
 * A UniConfGen that represents pending transactions to another generator.
 */
#ifndef __UNITRANSACTIONGEN_H
#define __UNITRANSACTIONGEN_H

#include "uniconfgen.h"

class UniConfChangeTree;
class UniConfValueTree;

/**
 * A UniConfGen that represents pending transactions to another generator.
 * It's moniker is "transaction" followed by a moniker to create
 * the underlying generator, or alternatively an IUniConfGen object.
 * 
 * A set() call on a UniTransactionGen records the fact that you wish to
 * perform that set() call on the underlying generator.
 *
 * When you call commit(), the underlying generator is modified to 
 * incorporate all set() calls since the last commit() or refresh() (or
 * since creation if none) as if they had all been made on the underlying
 * generator at the moment of your call to commit(). However, commit()
 * does this in a way that prevents unnecessary extra callbacks from
 * being issued by the underlying generator.
 *
 * When you call refresh(), all set() calls since the last commit() or
 * refresh() (or since creation if none) are discarded.
 *
 * WARNING!!!  Your calls to commit() and refresh() will propagate to
 * any upstream generators.  To prevent this, you will want to use a
 * UniBachelorGen.
 *
 * When you use get(), exists(), haschildren(), iterator(), or
 * recursiveiterator(), the results that you get are equivalent to the results
 * that you would have gotten if you had called commit() and then used these
 * methods on the underlying generator, except that they do not
 * modify the underlying generator.
 *
 * Note that the UniTransactionGen's iterators might iterate over keys 
 * in a different order than would the underlying generator's iterators
 * after a commit(). Also, the UniTransactionGen's iterators presently do
 * not support concurrent modification (regardless of whether the underlying
 * generator's iterators do).
 *
 * The UniTransactionGen issues callbacks due to calls to set() and refresh()
 * and also due to changes to the underlying generator from other sources. If
 * the underlying generator issues precisely the callbacks needed to indicate
 * a change in state, then so will the UniTransactionGen.
 *
 * The UniTransactionGen assumes that the underlying generator recursively
 * issues deletion callbacks whenever someone deletes a key from it that has
 * children, but the semantics in r4_2 and below don't require that. In those
 * versions, or when wrapping a generator with a broken callback system, the
 * UniTransactionGen won't issue callbacks properly, but everything else
 * should work.
 *
 * In order to work properly, the UniTransactionGen has to assume that the
 * underlying generator completely obeys the UniConfGen semantics. If that
 * is not true, then the UniTransactionGen might not obey its own class
 * contract or even the UniConfGen semantics either. (Note that few
 * generators completely obey the UniConfGen semantics.) However, provided
 * that the underlying generator's personal semantics are at least sane, the
 * UniTransactionGen's get()-like methods will probably show the underlying
 * generator as how your uncommitted set()s indicate you want it to be,
 * commit() will probably try and make it be like that (without any callbacks
 * made for things it couldn't commit), and refresh() will probably work as
 * designed.
 *
 * Using a UniTransactionGen and/or its underlying generator in multiple
 * threads will probably break it.
 *
 * Though similar in concept to a UniFilterGen, the UniTransactionGen
 * doesn't derive from it because we have basically no need for any of
 * its functionality.
 */
class UniTransactionGen : public UniConfGen
{
public:
    /** 
     * Constructs a UniTransactionGen for the given underlying generator,
     * which must be non-null.
     */
    UniTransactionGen(IUniConfGen *_base);

    /**
     * Destroys the UniTransactionGen and the underlying generator. Does
     * not commit uncommitted data.
     */
    ~UniTransactionGen();


    /***** Overridden methods *****/
    
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual void commit();
    virtual bool refresh();
    virtual Iter *iterator(const UniConfKey &key);
    virtual bool isok();
    virtual void flush_buffers();
    
protected:
    UniConfChangeTree *root;
    IUniConfGen *base;

    /**
     * A recursive helper function for commit().
     */
    void apply_changes(UniConfChangeTree *node,
		       const UniConfKey &section);

    /**
     * A recursive helper function for apply_changes().
     */
    void apply_values(UniConfValueTree *newcontents,
		      const UniConfKey &section);

    /**
     * A recursive helper function for refresh().
     */
    void cancel_changes(UniConfChangeTree *node,
			const UniConfKey &section);

    /**
     * A recursive helper function for cancel_changes().
     */
    void cancel_values(UniConfValueTree *newcontents,
		       const UniConfKey &section);

    /**
     * The callback function for the underlying generator.
     */
    void gencallback(const UniConfKey &key,
		     WvStringParm value);

    /**
     * Four functions to implement the functionality of set() so
     * that it isn't two pages long.
     */
    UniConfValueTree *create_value(UniConfValueTree *parent,
				   const UniConfKey &key,
				   int seg,
				   WvStringParm value);

    UniConfChangeTree *create_change(UniConfChangeTree *parent,
				     const UniConfKey &key,
				     int seg,
				     WvStringParm value);

    UniConfValueTree *set_value(UniConfValueTree *node,
				const UniConfKey &key,
				int seg,
				WvStringParm value);

    UniConfChangeTree *set_change(UniConfChangeTree *node,
				  const UniConfKey &key,
				  int seg,
				  WvStringParm value);

    /**
     * A recursive helper function for create_change().
     */
    void deletion_simulator(const UniConfKey &key);

    /**
     * A recursive helper function for set_change().
     */
    void deletion_simulator2(const UniConfKey &key);

    /**
     * A UniConfTree visitor function for set_value(), cancel_values(), and
     * cancel_changes().
     */
    void deletion_visitor(const UniConfValueTree *node, void *userdata);
};

#endif
