/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * Defines the root management class for UniConf.
 */
#ifndef __UNICONFROOT_H
#define __UNICONFROOT_H

#include "uniconf.h"
#include "uniconftree.h"
#include "unimountgen.h"


/**
 * @internal
 * Holds information about a single watch.
 */
class UniWatchInfo
{
public:
    void *cookie;
    bool recurse;
    UniConfCallback cb;

    UniWatchInfo(void *_cookie, bool _recurse, UniConfCallback _cb)
        : cookie(_cookie), recurse(_recurse), cb(_cb) { }

    /** Returns watch recursion */
    bool recursive()
        { return recurse; }

    /** Notifies that a key has changed. */
    void notify(const UniConf &cfg, const UniConfKey &key)
        { cb(cfg, key); }

    /** Equality test. */
    bool operator== (const UniWatchInfo &other) const
        { return other.cookie == cookie; }
};
DeclareWvList(UniWatchInfo);


/**
 * @internal
 * Data structure to track requested watches.
 */
class UniWatchInfoTree : public UniConfTree<UniWatchInfoTree>
{
public:
    UniWatchInfoList watches;
    
    UniWatchInfoTree(UniWatchInfoTree *parent,
		 const UniConfKey &key = UniConfKey::EMPTY)
        : UniConfTree<UniWatchInfoTree>(parent, key) { }

    /** Returns true if the node should not be pruned. */
    bool isessential()
        { return haschildren() || ! watches.isempty(); }
};


/**
 * Represents the root of a hierarhical registry consisting of pairs
 * of UniConfKeys and associated string values.
 *
 * Any number of data containers may be mounted into the tree at any
 * number of mount points to provide a backing store from which
 * registry keys and values are fetched and into which they are
 * stored.  Multiple data containers may be mounted at the same
 * location using standard unix semantics.
 */
class UniConfRoot : public UniConf
{
    friend class UniConf;
    friend class UniConf::Iter;
    friend class UniConf::RecursiveIter;

    UniWatchInfoTree watchroot;
    
    /** undefined. */
    UniConfRoot(const UniConfRoot &other);

public:
    /** Creates an empty UniConf tree with no mounted stores. */
    UniConfRoot();

    /** 
     * Creates a new UniConf tree and mounts the given moniker at the root.
     * Since most people only want to mount one generator, this should save
     * a line of code here and there.
     */
    UniConfRoot(WvStringParm moniker, bool refresh = true);

    /** 
     * Creates a new UniConf tree and mounts the given generator at the root.
     * Since most people only want to mount one generator, this should save
     * a line of code here and there.
     */
    UniConfRoot(UniConfGen *gen, bool refresh = true);
    
    /** Destroys the UniConf tree along with all uncommitted data. */
    ~UniConfRoot();

    /**
     * Requests notification when any of the keys covered by the
     * recursive depth specification change by invoking a callback.
     */
    void add_callback(void *cookie, const UniConfKey &key,
		      const UniConfCallback &callback, bool recurse = true);
    
    /**
     * Cancels notification requested using add_callback().
     */
    void del_callback(void *cookie, const UniConfKey &key,
		      bool recurse = true);

    /**
     * Requests notification when any of the keys covered by the
     * recursive depth specification change by setting a flag.
     */
    void add_setbool(const UniConfKey &key, bool *flag, bool recurse = true);

    /**
     * Cancels notification requested using add_setbool().
     */
    void del_setbool(const UniConfKey &key, bool *flag, bool recurse = true);

private:
    /**
     * Checks a branch of the watch tree for notification candidates.
     *   node - the current node
     *   key - the key that changed
     *   segleft - the number of segments left in the key (possibly negative)
     */
    void check(UniWatchInfoTree *node, const UniConfKey &key, int segleft);

    /**
     * Recursively checks a branch of the watch tree for notification candidates.
     *   node - the current node
     *   key - the key that changed
     */
    void deletioncheck(UniWatchInfoTree *node, const UniConfKey &key);

    /** Prunes a branch of the watch tree. */
    void prune(UniWatchInfoTree *node);
    
    /** Callback from UniMountTreeGen (FIXME: that's a lie.) */
    void gen_callback(const UniConfKey &key, WvStringParm value);

protected:
    friend class UniUnwrapGen;
    UniMountGen mounts;
    
public:
    /** Internal callback for setbool style notifications. */
    static void setbool_callback(bool *flag, const UniConf &,
				 const UniConfKey &)
        { *flag = true; }

};

#endif //__UNICONFROOT_H
