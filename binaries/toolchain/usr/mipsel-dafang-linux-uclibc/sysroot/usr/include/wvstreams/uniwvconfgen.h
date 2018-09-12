/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 * 
 * A generator to make a UniConf object out of a WvConf.
 */ 

#ifndef __UNICONFWVGEN_H
#define __UNICONFWVGEN_H

#include "uniconfgen.h"

class WvConf;
/**
 * A UniConf generator for backwards compatibility with WvConf
 */
class UniWvConfGen : public UniConfGen
{
private:
    UniConfKey *tempkey;
    WvString tempvalue;

    void notify(void *userdata, WvStringParm section, WvStringParm entry,
		WvStringParm oldval, WvStringParm newval);

protected:
    WvConf *cfg;

    class WvConfIter;

public:
    UniWvConfGen(WvConf *_cfg);
    ~UniWvConfGen();

    /***** Overridden members *****/

    virtual void flush_buffers() { }
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual bool haschildren(const UniConfKey &key);
    virtual Iter *iterator(const UniConfKey &key);
};

#endif //__UNICONFWVGEN_H
