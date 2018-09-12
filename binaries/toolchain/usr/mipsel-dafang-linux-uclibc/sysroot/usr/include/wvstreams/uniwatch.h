/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * A class that does add_callback when created and del_callback when
 * destroyed, thus making it harder to do one or the other incorrectly.
 * 
 * Because it's an object itself, it uses its own 'this' pointer as the
 * 'cookie', so you don't have to come up with one.
 */
#ifndef __UNIWATCH_H
#define __UNIWATCH_H

#include "uniconf.h"

class UniWatch
{
    UniConf cfg;
    UniConfCallback cb;
    bool recurse;
    
public:
    // standard "add_callback" version
    UniWatch(const UniConf &_cfg, const UniConfCallback &_cb,
	     bool _recurse = true);
    
    // special "add_setbool" version
    UniWatch(const UniConf &_cfg, bool *b, bool _recurse = true);
    
    ~UniWatch();
};

DeclareWvList2(UniWatchListBase, UniWatch);

class UniWatchList : public UniWatchListBase
{
public:
    void add(const UniConf &_cfg, const UniConfCallback &_cb,
	     bool _recurse = true)
        { append(new UniWatch(_cfg, _cb, _recurse), true); }
    void add(const UniConf &_cfg, bool *b, bool _recurse = true)
        { append(new UniWatch(_cfg, b, _recurse), true); }
};


#endif // __UNIWATCH_H
