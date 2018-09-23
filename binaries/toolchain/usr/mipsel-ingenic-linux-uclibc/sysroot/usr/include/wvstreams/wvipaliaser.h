/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */ 
#ifndef __WVIPALIASER_H
#define __WVIPALIASER_H

#include "wvinterface.h"
#include "wvaddr.h"

/**
 * WvIPAliaser handles IP aliasing in the Linux kernel.  Multiple instances
 * of the object can be created, and they will share aliases between them.
 * Aliased addresses are only removed when all WvIPAliaser objects using
 * that address give it up.  (ie. the object is destroyed, or the Aliaser
 * is reconfigured without including that address)
 */
class WvIPAliaser
{
    struct Alias
    {
	int index, link_count;
	WvIPAddr ip;
	
	Alias(const WvIPAddr &_ip);
	~Alias();
    };

    DeclareWvList(Alias);
    
    static AliasList all_aliases;
    AliasList aliases;
    WvInterfaceDict interfaces;
    
    WvIPAliaser::Alias *ipsearch(WvIPAliaser::AliasList &l,
				 const WvIPAddr &ip);
    
public:
    WvIPAliaser();
    ~WvIPAliaser();
    
    void dump();
    
    /**
     * you must call start_edit() once, then add() any number of times, then
     * done_edit() once, to change your aliases.  The addresses add()ed
     * during the session become the _only_ ones that are aliases by this
     * WvIPAliaser instance.
     *
     * Why bother?  This way, WvIPAliaser can see all the desired aliases
     * when they are being changed, and only delete previously-added ones
     * if they are no longer used.  This is important, since if eg. Fast
     * Forward has an open connection through 1.2.3.4, and we want to add
     * 1.2.3.5 and delete 1.2.3.3, Fast Forward need not remember that it
     * was using 1.2.3.3 but 1.2.3.4 does not get deleted, even temporarily.
     *
     * If that was too confusing, just remember: call these functions in
     * the order they appear below, always.
     *
     * If add() returns true, then an interface was just created. If it returns
     * false, then that call made no changes.
     *
     * If done_edit() returns true, then one or more interfaces were just
     * destroyed. If it returns false, then that call made no changes.
     */
    void start_edit();
    bool add(const WvIPAddr &ip);
    bool done_edit();
};


#endif // __WVIPALIASER_H
