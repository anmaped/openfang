/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2003 Net Integration Technologies, Inc.
 *
 * A generator that exposes the windows registry.
 *
 * When linking statically, use the following #pragma to ensure this 
 * generator gets registered:
 * #pragma comment(linker, "/include:?UniRegistryGenMoniker@@3V?$WvMoniker@VUniConfGen@@@@A")
 */
#ifndef __UNICONFREGISTRY_H
#define __UNICONFREGISTRY_H

#include "uniconfgen.h"
#include "wvlog.h"
#include "windows.h"

/**
 * A generator that exposes the windows registry.
 *
 * To mount, use the moniker "registry:".
 *
 */
class UniRegistryGen : public UniConfGen
{
    WvLog m_log;
    HKEY m_hRoot;

protected:
    virtual void flush_buffers() { }

public:
    UniRegistryGen(WvString _base);
    virtual ~UniRegistryGen();

    /***** Overridden methods *****/
    
    virtual bool isok();
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual bool exists(const UniConfKey &key);
    virtual bool haschildren(const UniConfKey &key);
    virtual Iter *iterator(const UniConfKey &key);
};

class UniRegistryGenIter : public UniConfGen::Iter
{
public:
    UniRegistryGenIter(UniRegistryGen &gen, const UniConfKey &key, HKEY base);

    /** Destroys the iterator. */
    virtual ~UniRegistryGenIter();

    /**
     * Rewinds the iterator.
     * Must be called prior to the first invocation of next().
     */
    virtual void rewind();

    /**
     * Seeks to the next element in the sequence.
     * Returns true if that element exists.
     * Must be called prior to the first invocation of key().
     */
    virtual bool next();

    /** Returns the current key. */
    virtual UniConfKey key() const;

    /** Returns the value of the current key. */
    virtual WvString value() const;

private:
    LONG next_key();
    LONG next_value();

    HKEY m_hKey;
    enum Enumerating { KEYS, VALUES } m_enumerating;
    unsigned m_index;
    UniRegistryGen &gen;
    const UniConfKey parent;
    UniConfKey current_key;
    const HKEY m_dontClose;
};

#endif // __UNICONFREGISTRY_H
