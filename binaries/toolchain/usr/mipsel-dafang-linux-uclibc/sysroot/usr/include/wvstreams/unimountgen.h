/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * Defines a UniConfGen that manages a tree of UniConfGen instances.
 */
#ifndef __UNIMOUNTGEN_H
#define __UNIMOUNTGEN_H

#include "uniconfgen.h"
#include "wvmoniker.h"
#include "wvstringlist.h"
#include "wvtr1.h"


/** The UniMountTree implementation realized as a UniConfGen. */
class UniMountGen : public UniConfGen
{
protected:

    // Class to hold the generator with its mountpoint    
    class UniGenMount
    {
    public:
        UniGenMount(IUniConfGen *gen, const UniConfKey &key)
            : gen(gen), key(key) 
	    { }

        xplc_ptr<IUniConfGen> gen;
        UniConfKey key;
    };

    typedef class WvList<UniGenMount> MountList;
    MountList mounts;

    /** undefined. */
    UniMountGen(const UniMountGen &other);

public:
    /** Creates an empty UniConf tree with no mounted stores. */
    UniMountGen();

    /** Destroys the UniConf tree along with all uncommitted data. */
    virtual ~UniMountGen();
    
    void zap();
    
    /**
     * Mounts a generator at a key using a moniker.
     
     * Returns the generator instance pointer, or NULL on failure.
     */
    virtual IUniConfGen *mount(const UniConfKey &key,
			       WvStringParm moniker,
			       bool refresh);
    
    /**
     * Mounts a generator at a key.
     * Takes ownership of the supplied generator instance.
     * 
     * "key" is the key
     * "gen" is the generator instance
     * "refresh" is if true, refreshes the generator after mount
     * Returns: the generator instance pointer, or NULL on failure
     */
    virtual IUniConfGen *mountgen(const UniConfKey &key,
				  IUniConfGen *gen,
				  bool refresh);

    /**
     * Unmounts the generator at a key and releases it.
     *
     * "gen" is the generator instance
     * "commit" is if true, commits the generator before unmount
     */
    virtual void unmount(IUniConfGen *gen, bool commit);
    
    /**
     * Finds the generator that owns a key.
     * 
     * If the key exists, returns the generator that provides its
     * contents.  Otherwise returns the generator that would be
     * updated if a value were set.
     * 
     * "key" is the key
     * "mountpoint" is if not NULL, replaced with the mountpoint
     *        path on success
     * Returns: the handle, or a null handle if none
     */
    virtual IUniConfGen *whichmount(const UniConfKey &key,
				   UniConfKey *mountpoint);

    /** Determines if a key is a mountpoint. */
    virtual bool ismountpoint(const UniConfKey &key);
    
    /***** Overridden members *****/
    
    virtual bool exists(const UniConfKey &key);
    virtual bool haschildren(const UniConfKey &key);
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual void commit();
    virtual bool refresh();
    virtual void flush_buffers() { }
    virtual Iter *iterator(const UniConfKey &key);
    virtual Iter *recursiveiterator(const UniConfKey &key);

private:
    /** Find the active generator for a given key. */
    UniGenMount *findmount(const UniConfKey &key);
    /** Find a unique active generator a given key, will return NULL if
     *  there are other generators beneath that key. */
    UniGenMount *findmountunder(const UniConfKey &key);

    // Trim the key so it matches the generator starting point
    UniConfKey trimkey(const UniConfKey &foundkey, const UniConfKey &key)
        { return key.removefirst(foundkey.numsegments()); }

    /** Called by generators when a key changes. */
    void gencallback(const UniConfKey &base, const UniConfKey &key,
		     WvStringParm value);

    void makemount(const UniConfKey &key);

    /** Return true if the given key has a subkey 
     *  if you used findmount first, give the result as a parameter to
     *  improve efficiency*/
    bool has_subkey(const UniConfKey &key, UniGenMount *found = NULL);

    struct UniGenMountPairs;
    DeclareWvDict(UniGenMountPairs, WvFastString, key);

};

#endif //__UNIMOUNTGEN_H
