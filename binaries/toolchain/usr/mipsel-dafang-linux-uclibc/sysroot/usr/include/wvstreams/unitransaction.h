/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2005 Net Integration Technologies, Inc.
 *
 * Allows one to wrap a UniConf tree with a transaction model.  Use
 * UniTransaction::commit() to commit, and
 * UniTransaction::refresh() to rollback.
 */
#ifndef _UNITRANSACTION_H
#define _UNITRANSACTION_H

#include "unibachelorgen.h"
#include "uniconfroot.h"
#include "unitransactiongen.h"
#include "uniunwrapgen.h"

/**
 * Wraps an existing UniConf tree with a transaction generator.
 */
class UniTransaction : public UniConfRoot
{
    friend class UniConf;
    friend class UniConf::Iter;
    friend class UniConf::RecursiveIter;

public:
    UniTransaction(const UniConf &base)
	: UniConfRoot(new UniTransactionGen(new UniBachelorGen(
		      new UniUnwrapGen(base))), false)
    {
    }
    
    // C++ would auto-generate a "copy constructor" for this function, but
    // what we really want is just to wrap a new transaction around the
    // base, just like any other UniConf object.
    UniTransaction(const UniTransaction &base)
	: UniConfRoot(new UniTransactionGen(new UniBachelorGen(
		      new UniUnwrapGen(base))), false)
    {
    }
};

#endif /* _UNITRANSACTION_H */
