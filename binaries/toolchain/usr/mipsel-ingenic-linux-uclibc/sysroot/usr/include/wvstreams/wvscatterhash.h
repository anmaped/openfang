/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2003 Net Integration Technologies, Inc.
 *
 * A hash table container.
 */
#ifndef __WVSCATTERHASH_H
#define __WVSCATTERHASH_H

#include "wvhash.h"
#include "wvsorter.h"
#include "wvxplc.h"   // for deletev.  ick.
#include <sys/types.h>

#define REBUILD_LOAD_FACTOR 0.45
#define RESIZE_LOAD_FACTOR 0.4

#define IS_OCCUPIED(x) ((x) >> 1)
#define IS_AUTO_FREE(x) ((x) == 3)
#define IS_DELETED(x) ((x) == 1)

class WvScatterHashBase
{
public:
    WvScatterHashBase(unsigned _numslots);
    virtual ~WvScatterHashBase() { deletev xslots; deletev xstatus; }

    static const unsigned null_idx = (unsigned)-1;
    static const unsigned prime_numbers[];

    size_t count() const { return num; }
    bool isempty() const { return !num; }
    size_t slowcount() const;
  
    /******* IterBase ******/
    class IterBase
    {
    public:
        IterBase(WvScatterHashBase &_table) : table(&_table) { }

        IterBase(const IterBase &other)
            : table(other.table), index(other.index) { }

        void rewind() { index = 0; }
	bool cur()
	    { return index <= table->numslots; }
	void *vptr()
	    { return get(); }
	
        bool next()
        {
            if (!table)
                return false;

	    /* FIXME: Couldn't this be a *little* clearer? */
            while (++index <= table->numslots &&
                   !IS_OCCUPIED(table->xstatus[index-1])) { }

	    return index <= table->numslots;
        }

        bool get_autofree() const
	{
            return IS_AUTO_FREE(table->xstatus[index-1]);
	}

        void set_autofree(bool autofree)
	{
            table->xstatus[index-1] = autofree ? 3 : 2;
	}

    protected:
        void *get() const { return table->xslots[index-1]; }

        WvScatterHashBase *table;
        unsigned index;
    };


protected:
    virtual unsigned do_hash(const void *data) = 0;
    virtual void do_delete(void *data) = 0;

    friend class IterBase;
    
    typedef void *Slot;
    typedef unsigned char Status;

    Slot *xslots;
    Status *xstatus;
    int prime_index;
    unsigned numslots;

    unsigned genfind(const void *data, unsigned hash) const;
    Slot genfind_or_null(const void *data, unsigned hash) const;
    void _add(void *data, bool autofree);
    void _add(void *data, unsigned hash, bool autofree);
    void _remove(const void *data, unsigned hash);
    void _zap();
    void _set_autofree(const void *data, unsigned hash, bool autofree);
    bool _get_autofree(const void *data, unsigned hash);

    virtual bool compare(const void *key, const void *elem) const = 0;


private:
    void rebuild();
    unsigned second_hash(unsigned hash) const
        { return (hash % (numslots - 1)) + 1; }
    unsigned curhash(unsigned hash, unsigned hash2, unsigned attempt) const
        //{ return (hash + attempt * attempt) % numslots; }
        { return (hash + attempt*hash2) % numslots; }

    size_t used;
    size_t num;
};


template <
    class T,                                            // element type
    class K,                                            // key type
    class Accessor,                                     // element to key
    template <class> class Comparator = OpEqComp        // comparison func
>
class WvScatterHash : public WvScatterHashBase
{
protected:
    typedef Comparator<K> MyComparator;

    virtual bool compare(const void *key, const void *elem) const
        { return MyComparator::compare((const K *)key,
                Accessor::get_key((const T *)elem)); }

    unsigned hash(const T *data)
        { return WvHash(*Accessor::get_key(data)); }

    virtual unsigned do_hash(const void *data)
        { return hash((const T *)data); }

    virtual void do_delete(void *data)
        { delete (T *)data; }    

public:
    WvScatterHash(unsigned _numslots = 0) : WvScatterHashBase(_numslots) { }
    virtual ~WvScatterHash() { _zap(); }

    T *operator[] (const K &key) const
        { return (T *)(genfind_or_null(&key, WvHash(key))); }

    void add(const T *data, bool autofree = false)
        { _add((void *)data, hash(data), autofree); }

    void remove(const T *data)
        { _remove(Accessor::get_key(data), hash(data)); }

    void set_autofree(const K &key, bool autofree)
    {
	_set_autofree(key, WvHash(key), autofree);
    }

    void set_autofree(const T *data, bool autofree)
    {
	_set_autofree(Accessor::get_key(data), hash(data), autofree);
    }

    bool get_autofree(const K &key)
    {
	return _get_autofree(key, WvHash(key));
    }

    bool get_autofree(const T *data)
    {
	return _get_autofree(Accessor::get_key(data), hash(data));
    }

    void zap()
        { _zap(); }


    class Iter : public WvScatterHashBase::IterBase
    {
    public:
        Iter(WvScatterHash &_table) : IterBase(_table) { }
        Iter(const Iter &other) : IterBase(other) { }

        unsigned char *getstatus() { return &xstatus[index-1]; }

        T *ptr() const
            { return (T *)(get()); }

        WvIterStuff(T);
    };

    typedef class WvSorter<T, WvScatterHashBase, WvScatterHashBase::IterBase>
	Sorter;
};


#define DeclareWvScatterDict2(_classname_,  _type_, _ftype_, _field_)     \
        __WvScatterDict_base(_classname_, _type_, _ftype_, &obj->_field_)

#define DeclareWvScatterDict(_type_, _ftype_, _field_)                    \
        DeclareWvScatterDict2(_type_##Dict, _type_, _ftype_, _field_)

#define DeclareWvScatterTable2(_classname_, _type_)                       \
        __WvScatterDict_base(_classname_, _type_, _type_, obj)

#define DeclareWvScatterTable(_type_)                                     \
        DeclareWvScatterTable2(_type_##Table, _type_)


#define __WvScatterDict_base(_classname_, _type_, _ftype_, _field_)       \
    template <class T, class K>                                           \
    struct _classname_##Accessor                                          \
    {                                                                     \
        static const K *get_key(const T *obj)                             \
            { return _field_; }                                           \
    };                                                                    \
                                                                          \
    typedef WvScatterHash<_type_, _ftype_,                                \
             _classname_##Accessor<_type_, _ftype_> > _classname_


#endif //_WVSCATTERHASH_H
