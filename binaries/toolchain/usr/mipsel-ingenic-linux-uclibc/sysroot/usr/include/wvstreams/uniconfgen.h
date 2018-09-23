/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 * 
 * An abstract data container that backs a UniConf tree.
 */
#ifndef __UNICONFGEN_H
#define __UNICONFGEN_H
 
#include "uniconfpair.h"
#include "wvcallbacklist.h"
#include "wvtr1.h"

class UniConfGen;
class UniListIter;

/**
 * The callback type for signalling key changes from a UniConfGen.
 * 
 * Generators that wrap other generators should catch notifications
 * and reissue them using themselves as the "gen" parameter and their
 * userdata as the "userdata parameter".  This can be done effectively by
 * invoking the delta() function on receipt of a notification from a
 * wrapped generator.  See UniFilterGen.
 * 
 * Parameters: gen, key, userdata
 *   gen - the externally visible generator whose key has changed
 *   key - the key that has changed
 */
typedef wv::function<void(const UniConfKey&, WvStringParm)> 
    UniConfGenCallback;

/**
 * An abstract data container that backs a UniConf tree.
 *
 * This is intended to be implemented to provide support for fetching
 * and storing keys and values using different access methods.
 */
class IUniConfGen : public IObject
{
public:
    virtual ~IUniConfGen();
    
    /***** Notification API *****/
    
    /** Adds a callback for change notification. */
    virtual void add_callback(void *cookie,
			      const UniConfGenCallback &callback) = 0;
    
    /** Removes a callback for change notification. */
    virtual void del_callback(void *cookie) = 0;
    

    /***** Status API *****/
    
    /**
     * Determines if the generator is usable and working properly.
     * The default implementation always returns true.
     */
    virtual bool isok() = 0;

    
    /***** Key Persistence API *****/
    
    /** Commits any changes. The default implementation does nothing. */
    virtual void commit() = 0;
    
    /**
     * Refreshes information about a key recursively.
     * May discard uncommitted data.
     *
     * The default implementation always returns true.
     */
    virtual bool refresh() = 0;

    /** 
     * Flushes any commitment/notification buffers . 
     *
     * The default implementation always returns true.
     *  NOTE: This method should be 'protected'
     */
    virtual void flush_buffers() = 0;

    /***** Key Retrieval API *****/
    
    /**
     * Indicate that we will eventually be interested in doing get(),
     * haschildren(), or other "get-like" operations on a particular key
     * or tree of keys.  The generator may be able to speed up these
     * operations by, say, caching them in advance.
     * 
     * This function is not allowed to do blocking operations.  It is allowed
     * to do nothing at all, however, and then get() might block later.
     */
    virtual void prefetch(const UniConfKey &key, bool recursive) = 0;
    
    /**
     * Fetches a string value for a key from the registry.  If the key doesn't
     * exist, the return value has .isnull() == true.
     */
    virtual WvString get(const UniConfKey &key) = 0;
    
    /**
     * Without fetching its value, returns true if a key exists.
     *
     * This is provided because it is often more efficient to
     * test existance than to actually retrieve the value.
     *
     * The default implementation returns !get(key).isnull().
     */
    virtual bool exists(const UniConfKey &key) = 0;


    /**
     * Converts a string to an integer.  If the string is null or not
     * recognized, return defvalue. 
     *
     * This is here to support the common str2int(get(key)).
     *
     * The default implementation recognizes the booleans 'true', 'yes', 'on'
     * and 'enabled' as 1, and 'false', 'no', 'off' and 'disabled' as 0.
     */
    virtual int str2int(WvStringParm s, int defvalue) const = 0;

    
    /***** Key Storage API *****/
    
    /**
     * Stores a string value for a key into the registry.  If the value is
     * WvString::null, the key is deleted.
     */
    virtual void set(const UniConfKey &key, WvStringParm value) = 0;


    /**
     * Stores multiple key-value pairs into the registry.  If the value is
     * WvString::null, the key is deleted.
     */
    virtual void setv(const UniConfPairList &pairs) = 0;


    /***** Key Enumeration API *****/
    
    /**
     * Returns true if a key has children.
     *
     * This is provided because it is often more efficient to
     * test existance than to actually retrieve the keys.
     * 
     * The default implementation uses the iterator returned by iterator()
     * to test whether the child has any keys.
     * Subclasses are strongly encouraged to provide a better implementation.
     */
    virtual bool haschildren(const UniConfKey &key) = 0;

    /** The abstract iterator type (see below) */
    class Iter;

    /** A concrete null iterator type (see below) */
    class NullIter;
    
    /** An iterator over a constant list of keys (see below) */
    typedef ::UniListIter ListIter;

    /**
     * Returns an iterator over the children of the specified key.
     * May return NULL or an empty iterator if the key has no children.
     *
     * The caller takes ownership of the returned iterator and is responsible
     * for deleting it when finished.
     */
    virtual Iter *iterator(const UniConfKey &key) = 0;
    
    /**
     * Like iterator(), but the returned iterator is recursive, that is,
     * it will return children of the immediate children, not just the
     * immediate children themselves.
     * 
     * May return NULL if the key has no immediate children (since that means
     * there are also no indirect children).
     * 
     * Note that UniConfGen::recursiveiterator() is a default
     * implementation that just calls iterator() recursively, so it'll work
     * in any derived class without you overriding this function.  However,
     * you might want to do it anyway if it would be more efficient in your
     * particular case.
     */
    virtual Iter *recursiveiterator(const UniConfKey &key) = 0;
};

DEFINE_IID(IUniConfGen, {0x7ca76e98, 0xb694, 0x43ca,
    {0xb0, 0x56, 0x8b, 0x9d, 0xde, 0x9a, 0xbe, 0x9f}});


/**
 * A default implementation of IUniConfGen, providing various handy features
 * that save trouble when implementing typical generators.
 */
class UniConfGen : public IUniConfGen
{
    IMPLEMENT_IOBJECT(UniConfGen);

    // These fields are deliberately hidden to encourage use of the
    // special notification members

    WvCallbackList<UniConfGenCallback> cblist;
    int hold_nesting;
    UniConfPairList deltas;
    
protected:
    /** Creates a UniConfGen object. */
    UniConfGen();

public:
    /** Destroys the UniConfGen and may discard uncommitted data. */
    virtual ~UniConfGen();

    /***** Notification API *****/
    
    /**
     * Adds a callback for change notification.
     * Must *not* be reimplemented by subclasses of UniConfGen.
     */
    virtual void add_callback(void *cookie, 
			      const UniConfGenCallback &callback);
    virtual void del_callback(void *cookie);
    
    /**
     * Immediately sends notification that a key has possibly changed.
     * Takes care of the details of invoking the callback.
     *
     * Note: You probably want to be using delta() instead.
     */
    void dispatch_delta(const UniConfKey &key, WvStringParm value);

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

    /**
     * Call this when a key's value or children have possibly changed.
     * 
     * If the hold nesting count is 0, the notification is sent immediately.
     * Otherwise it is added to a pending list for later.
     */
    void delta(const UniConfKey &key, WvStringParm value);   
    
    /***** Status API *****/
    virtual bool isok();
    
    /***** Key Persistence API *****/
    virtual void commit() { }
    virtual bool refresh() { return true; }
    virtual void prefetch(const UniConfKey &key, bool recursive) { }
    virtual WvString get(const UniConfKey &key) = 0;
    virtual bool exists(const UniConfKey &key);
    virtual int str2int(WvStringParm s, int defvalue) const;

    /***** Key Storage API *****/
    virtual void set(const UniConfKey &key, WvStringParm value) = 0;
    virtual void setv(const UniConfPairList &pairs) = 0;

    virtual void flush_buffers() = 0;

    /***** Key Enumeration API *****/
    virtual bool haschildren(const UniConfKey &key);
    virtual Iter *iterator(const UniConfKey &key) = 0;
    
    // a helpful default that just calls iterator() recursively
    virtual Iter *recursiveiterator(const UniConfKey &key);

protected:
    // A naive implementation of setv() that uses only set().
    void setv_naive(const UniConfPairList &pairs);
};

DeclareWvList(IUniConfGen);
DeclareWvList2(UniConfGenList, IUniConfGen);


/**
 * An abstract iterator over keys and values in a generator.
 *
 * Unlike other WvStreams iterators, this one declares virtual methods so
 * that UniConfGen implementations can supply the right behaviour
 * through a common interface that does not depend on static typing.
 *
 * The precise traversal sequence is defined by the iterator implementation.
 *
 * The iterator need not support concurrent modifications of the underlying
 * data structures.
 * 
 * TODO: Consider changing this rule depending on observed usage patterns.
 */
class UniConfGen::Iter
{
public:
    /** Destroys the iterator. */
    virtual ~Iter() { }

    /**
     * Rewinds the iterator.
     * Must be called prior to the first invocation of next().
     */
    virtual void rewind() = 0;

    /**
     * Seeks to the next element in the sequence.
     * Returns true if that element exists.
     * Must be called prior to the first invocation of key().
     */
    virtual bool next() = 0;

    /** Returns the current key. */
    virtual UniConfKey key() const = 0;
    
    /** 
     * Returns the value of the current key.  You could just do a get(),
     * but maybe your generator has a more efficient way.
     */
    virtual WvString value() const = 0;
};


/**
 * An iterator that's always empty.
 * This is handy if you don't have anything good to iterate over.
 */
class UniConfGen::NullIter : public UniConfGen::Iter
{
public:
    /***** Overridden members *****/
    
    virtual void rewind() { }
    virtual bool next() { return false; }
    virtual UniConfKey key() const { return UniConfKey::EMPTY; }
    virtual WvString value() const { return WvString(); }
};


#endif // __UNICONFGEN_H
