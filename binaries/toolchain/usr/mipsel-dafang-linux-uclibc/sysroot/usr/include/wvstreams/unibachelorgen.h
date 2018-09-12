/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2005 Net Integration Technologies, Inc.
 *
 * A UniConf generator that refuses to commit() or refresh().  This is
 * useful in blocking propogation of these messages upstream.
 */

#ifndef _UNIBACHELORGEN_H
#define _UNIBACHELORGEN_H

#include "unifiltergen.h"

class UniBachelorGen : public UniFilterGen
{
public:
    UniBachelorGen(IUniConfGen *inner);
    UniBachelorGen(WvStringParm moniker);

    virtual void commit();
    virtual bool refresh();
};

#endif /* _UNIBACHELORGEN_H */
