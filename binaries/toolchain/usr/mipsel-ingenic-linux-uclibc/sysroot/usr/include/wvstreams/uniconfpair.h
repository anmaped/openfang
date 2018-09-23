/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * UniConf key-value pair storage abstraction.
 */
#ifndef __UNICONFPAIR_H
#define __UNICONFPAIR_H

#include "uniconfkey.h"
#include "wvstring.h"
#include "wvhashtable.h"


/** Represents a simple key-value pair. */
class UniConfPair
{
    UniConfKey xkey;  /*!< the name of this entry */
    WvString xvalue;  /*!< the value of this entry */

public:
    /**
     * Creates a UniConfPair.
     * "key" is the key
     * "value" is the value
     */
    UniConfPair(const UniConfKey &key, WvStringParm value)
        : xkey(key), xvalue(value) { }


    const UniConfKey &key() const
        { return xkey; }

    const WvString &value()
        { return xvalue; }

    void setvalue(WvStringParm value) { xvalue = value; }

    void setkey(UniConfKey &key) { xkey = key; }
};

DeclareWvList(UniConfPair);

#endif //__UNICONFPAIR_H
