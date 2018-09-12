/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * Defines a hierarchical registry abstraction.
 */
#ifndef __UNICONF_H
#define __UNICONF_H

#include <vector>

#include "uniconfgen.h"
#include "uniconfkey.h"
#include "wvtr1.h"

class WvStream;
class UniConf;
class UniConfRoot;

/**
 * The callback type for signalling key changes from UniConf.
 * 
 * Parameters: cfg, relkey
 *   cfg - the UniConf config object representing the key that has changed
 *   relkey - the changed keypath, relative to the config object
 */
typedef wv::function<void(const UniConf&, const UniConfKey&)> UniConfCallback;

/**
 * UniConf instances function as handles to subtrees of a UniConf
 * tree and expose a high-level interface for clients.
 * 
 * All operations are marked "const" unless they modify the target
 * of the handle.  In effect, this grants UniConf handles the
 * same semantics as pointers where a const pointer may point
 * to a non-const object, which simply means that the pointer
 * cannot be reassigned.
 * 
 * When handles are returned from functions, they are always marked
 * const to guard against accidentally assigning to a temporary by
 * an expression such as cfg["foo"] = cfg["bar"].
 * Instead this must be written as
 *     cfg["foo"].setme(cfg["bar"].getme())
 * which is slightly
 * less elegant but avoids many subtle mistakes.  Also for this
 * reason, unusual cast operators, assignment operators,
 * or copy constructors are not provided.  Please do not add any.
 * 
 */
class UniConf
{
    friend class UniConfRoot;
    
protected:
    UniConfRoot *xroot;
    UniConfKey xfullkey;

    /**
     * Creates a handle to the specified subtree of the given root.
     * 
     * You can't create non-NULL UniConf objects yourself - ask UniConfRoot
     * or another UniConf object to make one for you.
     */
    UniConf(UniConfRoot *root, const UniConfKey &fullkey = UniConfKey::EMPTY);
    
public:
    /** Creates a NULL UniConf handle, useful for reporting errors. */
    UniConf();
    
    /** Copies a UniConf handle. */
    UniConf(const UniConf &other);
    
    /** Destroys the UniConf handle. */
    virtual ~UniConf();

    
    /***** Handle Manipulation API *****/

    /** Returns a handle to the root of the tree. */
    UniConf root() const
        { return UniConf(xroot, UniConfKey::EMPTY); }

    /** Returns a handle to the parent of this node. */
    UniConf parent() const
        { return UniConf(xroot, xfullkey.removelast()); }
    
    /**
     * Returns a pointer to the UniConfRoot that manages this node.
     * This may be NULL, to signal an invalid handle.
     */
    UniConfRoot *rootobj() const
        { return xroot; }

    /** Returns true if the handle is invalid (NULL). */
    bool isnull() const
        { return xroot == NULL; }

    /** Returns the full path of this node, starting at the root. */
    UniConfKey fullkey() const
        { return xfullkey; }
    
    /** Returns the full path of this node, starting at the given key.
     * Assumes that k is an ancestor of fullkey(). */
    UniConfKey fullkey(const UniConfKey &k) const;
    
    /** Returns the full path of this node, starting at the given handle. */
    UniConfKey fullkey(const UniConf &cfg) const
        { return fullkey(cfg.fullkey()); }

    /** Returns the path of this node relative to its parent. */
    UniConfKey key() const
        { return xfullkey.last(); }

    /**
     * Returns a handle for a subtree below this key. 'key' is the path
     * of the subtree to be appended to the full path of this handle to
     * obtain the full path of the new handle.
     */
    const UniConf operator[] (const UniConfKey &key) const
        { return UniConf(xroot, UniConfKey(xfullkey, key)); }

    /**
     * Return a subtree handle (see operator[]).  Mainly to support bindings
     * for languages that can't handle methods named [].
     */
    const UniConf u(const UniConfKey &key) const
        { return (*this)[key]; }

    /** Reassigns the target of this handle to match a different one. */
    UniConf &operator= (const UniConf &other)
    {
        xroot = other.xroot;
        xfullkey = other.xfullkey;
        return *this;
    }

    
    /***** Key Retrieval API *****/
    
    /** See UniConfGen::prefetch(). */
    void prefetch(bool recursive) const;
    
    /**
     * Fetches the string value for this key from the registry.  If the
     * key is not found, returns 'defvalue' instead.
     */
    WvString getme(WvStringParm defvalue = WvString::null) const;

    /** A different way to say cfg.getme(): use *cfg instead. */
    WvString operator* () const
        { return getme(); }

    /** A different way to say cfg.getme().num(): use cfg->num() instead. */
    WvStringStar operator -> () const
        { return getme(); }
    
    /** A different way to say cfg[x].getme(y). */
    WvString xget(WvStringParm key,
		  WvStringParm defvalue = WvString::null) const
        { return (*this)[key].getme(defvalue); }

    /**
     * Fetches the integer value for this key from the registry.  If the
     * key is not found, returns 'defvalue' instead.  (This is also used to
     * fetch booleans - 'true', 'yes', 'on' and 'enabled' are recognized as
     * 1, 'false', 'no', 'off' and 'disabled' as 0.  Note that a nonexistant
     * key is false by default.)
     */
    int getmeint(int defvalue = 0) const;

    /** A different way to say cfg[x].getmeint(y). */
    int xgetint(WvStringParm key, int defvalue = 0) const
        { return (*this)[key].getmeint(defvalue); }

    /**
     * Without fetching its value, returns true if this key exists.
     * 
     * This is provided because it is often more efficient to
     * test existance than to actually retrieve the value.
     */
    bool exists() const;


    /***** Key Storage API *****/

    /**
     * Stores a string value for this key into the registry.  If the value
     * is WvString::null, deletes the key and all of its children.
     */
    void setme(WvStringParm value) const;

    /**
     * Stores a string value for this key into the registry.
     */
    void setme(WVSTRING_FORMAT_DECL) const
        { return setme(WvString(WVSTRING_FORMAT_CALL)); }

    /** A different way to say cfg[x].setme(y). */
    void xset(WvStringParm key, WvStringParm value) const
        { (*this)[key].setme(value); }

    /**
     * Stores an integer value for this key into the registry.
     */
    void setmeint(int value) const;

    /** A different way to say cfg[x].setme(y). */
    void xsetint(WvStringParm key, int value) const
        { (*this)[key].setmeint(value); }


    /***** Key Handling API *****/

    /**
     * Equivalent to "mv" in a standard unix filesystem. This recursively
     * moves a given key and any subkeys to a new point. If the new point
     * exists then the key will be left as a subkey at the new point.
     * Otherwise, the key will also be renamed to the new point (as when
     * using mv).
     * 
     * Don't try to do dumb stuff like making dst a subkey of this one,
     * or vice versa, because we won't try to save you.
     * 
     * Unlike unix mv(), this is *not* currently atomic.  It's more like
     * cp-then-rm.
     */
    void move(const UniConf &dst) const;

    /**
     * Removes this key and all of its children from the registry.
     */
    void remove() const
        { setme(WvString::null); }

    /**
     * Equivalent to "cp -r" in a standard unix filesystem. This
     * recursively copies a given key to a new location. Any keys that
     * already exist at that location will not be overridden unless force
     * is true.
     * 
     * Don't try to do dumb stuff like making dst a subkey of this one,
     * or vice versa, because we won't try to save you.
     */
    void copy(const UniConf &dst, bool force) const;


    
    /***** Key Persistence API *****/

    /**
     * Refreshes information about this key recursively.
     * May discard uncommitted data.
     * Returns true on success.
     */
    bool refresh() const;
    
    /**
     * Commits information about this key recursively.
     */
    void commit() const;

    
    /***** Generator Mounting API *****/
    
    /**
     * Mounts a generator at this key using a moniker.
     * 
     * If 'refresh' is true, automatically refresh()es the generator
     * after mounting.
     *
     * Returns the mounted generator, or NULL on failure.
     */
    IUniConfGen *mount(WvStringParm moniker, bool refresh = true) const;
    
    /**
     * Mounts a generator at this key.
     * 
     * Takes ownership of the supplied generator instance.
     * 
     * If 'refresh' is true, automatically refresh()es the generator
     * after mounting.
     * 
     * Returns the mounted generator, or NULL on failure.
     */
    IUniConfGen *mountgen(IUniConfGen *gen, bool refresh = true) const;
    
    /** Unmounts the generator providing this key and destroys it. */
    void unmount(IUniConfGen *gen, bool commit) const;
    
    /** Determines if any generators are mounted at this key. */
    bool ismountpoint() const;
    
    /** Returns true if the generator at this key isok(). */
    bool isok() const;
    
    /**
     * Finds the generator that owns this key.
     * 
     * If the key exists, returns the generator that provides its
     * contents.  Otherwise returns the generator that would be
     * updated if a value were set.
     * 
     * If non-NULL, 'mountpoint' is set to the actual key where the generator
     * is mounted.
     */
    IUniConfGen *whichmount(UniConfKey *mountpoint = NULL) const;

    
    /***** Notification API *****/

    /**
     * Requests notification when any of the keys covered by the
     * recursive depth specification change by invoking a callback.
     *
     * As a programmer, you probably DO NOT want to use this.  Use
     * UniWatchList.add() instead.  Otherwise, make sure you call
     * del_callback at the appropriate time.
     */
    void add_callback(void *cookie, const UniConfCallback &callback,
		      bool recurse = true) const;
    
    /**
     * Cancels notification requested using add_callback().
     */
    void del_callback(void *cookie, bool recurse = true) const;

    /**
     * Requests notification when any of the keys covered by the
     * recursive depth specification change by setting a flag.
     */
    void add_setbool(bool *flag, bool recurse = true) const;

    /**
     * Cancels notification requested using add_setbool().
     */
    void del_setbool(bool *flag, bool recurse = true) const;
    
    /**
     * Pauses notifications until matched with a call to unhold_delta().
     * 
     * While paused, notification events are placed into a pending list.
     * Redundant notifications may be discarded.
     *
     * Use this to safeguard non-reentrant code.
     */
    void hold_delta();

    /**
     * Resumes notifications when each hold_delta() has been matched.
     * 
     * On resumption, dispatches all pending notifications except
     * those that were destined to watches that were removed.
     * 
     * Use this to safeguard non-reentrant code.
     */
    void unhold_delta();
    
    /**
     * Clears the list of pending notifications without sending them.
     * Does not affect the hold nesting count.
     */
    void clear_delta();

    /**
     * Flushes the list of pending notifications by sending them.
     * Does not affect the hold nesting count.
     */
    void flush_delta();
    
    
    /***** Key Enumeration API *****/
    
    /**
     * Prints the entire contents of this subtree to a stream.
     * If 'everything' is true, also prints empty values.
     */
    void dump(WvStream &stream, bool everything = false) const;
    
    /**
     * Returns true if this key has children.
     * 
     * This is provided because it is often more efficient to
     * test existance than to actually retrieve the keys.
     */
    bool haschildren() const;
    
    /*** Iterators (see comments in class declaration) ***/

    // internal base class for all of the key iterators
    class IterBase;
    // iterates over direct children
    class Iter;
    // iterates over all descendents in preorder traversal
    class RecursiveIter;
    // iterates over children matching a wildcard
    class XIter;

    // internal base class for sorted key iterators
    class SortedIterBase;
    // sorted variant of Iter
    class SortedIter;
    // sorted variant of RecursiveIter
    class SortedRecursiveIter;
    // sorted variant of XIter
    class SortedXIter;

    // lists of iterators
    DeclareWvList(Iter);
};


/**
 * An implementation base class for key iterators.
 */
class UniConf::IterBase
{
protected:
    UniConf top;
    UniConf current;

    IterBase(const UniConf &_top)
        : top(_top)
        { }

public:
    const UniConf *ptr() const
        { return &current; }
    WvIterStuff(const UniConf);
};


/**
 * This iterator walks through all immediate children of a UniConf node.
 */
class UniConf::Iter : public UniConf::IterBase
{
    UniConfGen::Iter *it;
    
public:
    /** Creates an iterator over the direct children of a branch. */
    Iter(const UniConf &_top);

    ~Iter()
        { delete it; }

    void rewind()
        { it->rewind(); }
    bool next()
    {
        if (!it->next())
            return false;
        current = top[it->key()];
        return true;
    }
    
    // FIXME: this is a speed optimization only.  Don't use this unless
    // you're apenwarr.  It will change.
    WvString _value() const
        { return it->value(); }
};


/**
 * This iterator performs depth-first traversal of a subtree.
 */
class UniConf::RecursiveIter : public UniConf::IterBase
{
    UniConfGen::Iter *it;

public:
    /** Creates a recursive iterator over a branch. */
    RecursiveIter(const UniConf &_top);

    ~RecursiveIter()
        { delete it; }

    void rewind()
        { it->rewind(); }
    bool next()
    {
        if (!it->next())
            return false;
        current = top[it->key()];
        return true;
    }   

    // FIXME: this is a speed optimization only.  Don't use this unless
    // you're apenwarr.  It will change.
    WvString _value() const
        { return it->value(); }
};


/**
 * This iterator walks over all children that match a wildcard
 * pattern.
 * 
 * See UniConfKey::matches(const UniConfKey&) for information about patterns.
 * 
 * Example patterns: (where STAR is the asterisk character, '*')
 *
 * "": a null iterator
 * "a": matches only the key "a" if it exists
 * "STAR": matches all direct children
 * "STAR/foo": matches any existing key "foo" under direct children
 * "STAR/STAR": matches all children of depth exactly 2
 * "foo/...": matches all keys including and below "foo"
 * "foo/STAR/...": matches all keys below "foo"
 * ".../foo/STAR": matches all keys below any subkey named "foo" in the tree
 */
class UniConf::XIter : public UniConf::IterBase
{
    UniConfKey pathead;
    UniConfKey pattail;
    UniConf::XIter *subit;
    UniConf::Iter *it; /*!< iterator over direct children */
    UniConf::RecursiveIter *recit; /*!< iterator over descendents */
    bool ready; /*!< next key is ready */

public:
    /** Creates a wildcard iterator. */
    XIter(const UniConf &_top, const UniConfKey &pattern);
    ~XIter();

    void rewind();
    bool next();
    
private:
    void cleanup();
    bool qnext();
    void enter(const UniConf &child);
};


/**
 * An implementation base class for sorted key iterators.
 * 
 * Unfortunately WvSorter is too strongly tied down to lists and pointers
 * to be of use here.  The main problem is that UniConf::Iter and company
 * return pointers to temporary objects whereas WvSorter assumes that the
 * pointers will remain valid for the lifetime of the iterator.
 */
class UniConf::SortedIterBase : public UniConf::IterBase
{
public:
    typedef int (*Comparator)(const UniConf &a, const UniConf &b);

    /** Default comparator. Sorts alphabetically by full key. */
    static int defcomparator(const UniConf &a, const UniConf &b);

    SortedIterBase(const UniConf &_top, Comparator comparator = defcomparator);
    ~SortedIterBase();

    bool next();

private:
    Comparator xcomparator;
    int index;
    int count;
    
    void _purge();
    void _rewind();
    
protected:
    std::vector<UniConf> xkeys;
    
    template<class Iter>
    void populate(Iter &i)
    {
        _purge();
        for (i.rewind(); i.next(); )
            xkeys.push_back(*i);
        _rewind();
    }
};


/**
 * A sorted variant of UniConf::Iter.
 */
class UniConf::SortedIter : public UniConf::SortedIterBase
{
    UniConf::Iter i;

public:
    SortedIter(const UniConf &_top, Comparator comparator = defcomparator)
        : SortedIterBase(_top, comparator), i(_top)
        { }

    void rewind()
        { populate(i); }
};


/**
 * A sorted variant of UniConf::RecursiveIter.
 */
class UniConf::SortedRecursiveIter : public UniConf::SortedIterBase
{
    UniConf::RecursiveIter i;

public:
    SortedRecursiveIter(const UniConf &_top,
                        Comparator comparator = defcomparator)
        : SortedIterBase(_top, comparator), i(_top)
        { }

    void rewind()
        { populate(i); }
};


/**
 * A sorted variant of UniConf::XIter.
 */
class UniConf::SortedXIter : public UniConf::SortedIterBase
{
    UniConf::XIter i;

public:
    SortedXIter(const UniConf &_top, const UniConfKey &pattern,
                Comparator comparator = defcomparator) 
        : SortedIterBase(_top, comparator), i(_top, pattern) 
        { }

    void rewind()
        { populate(i); }
};

#endif // __UNICONF_H
