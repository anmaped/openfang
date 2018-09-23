/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 *
 * A UniConf generator that executes callbacks to generate the value of keys
 */
#ifndef __UNICALLBACKGEN_H
#define __UNICALLBACKGEN_H

#include <map>

#include "unitempgen.h"
#include "wvstream.h"
#include "wvtr1.h"

typedef wv::function<WvString(const UniConfKey&)>
        UniCallbackGenGetCallback;
typedef wv::function<void(const UniConfKey&, WvStringParm)>
        UniCallbackGenSetCallback;

/**
 * A UniConf generator that executes callbacks to generate the value of keys
 *
 * To make the callback fire and set the value of the key,
 * call set(key, whatever).  Calling get(key) returns the most recent
 * generated value of the key.
 */
class UniCallbackGen: public UniTempGen
{
    typedef std::map<UniConfKey, UniCallbackGenGetCallback> GetCallbackMap;
    GetCallbackMap get_callbacks;
    typedef std::map<UniConfKey, UniCallbackGenSetCallback> SetCallbackMap;
    SetCallbackMap set_callbacks;

public:
    
    bool update_before_get;
    bool update_after_set;

    UniCallbackGen():
        update_before_get(false),
        update_after_set(true) {}
    virtual ~UniCallbackGen() {}

    virtual void setgetcallback(const UniConfKey &key,
            UniCallbackGenGetCallback get_callback)
    {
        if (get_callback)
            get_callbacks[key] = get_callback;
        else
            get_callbacks.erase(key);
    }
    virtual void setsetcallback(const UniConfKey &key,
            UniCallbackGenSetCallback set_callback)
    {
        if (set_callback)
            set_callbacks[key] = set_callback;
        else
            set_callbacks.erase(key);
    }

    virtual void update(const UniConfKey &key, 
            WvStringParm value = WvString::null)
    {
	GetCallbackMap::iterator it = get_callbacks.find(key);
        if (it != get_callbacks.end())
            UniTempGen::set(key, it->second(key));
        else
	    UniTempGen::set(key, value);
    }

    /***** Overridden members *****/
    virtual WvString get(const UniConfKey &key)
    {
        if (update_before_get) update(key);

        return UniTempGen::get(key);
    }
    virtual void set(const UniConfKey &key, WvStringParm value)
    {
	SetCallbackMap::iterator it = set_callbacks.find(key);
	if (it != set_callbacks.end())
            it->second(key, value);

        if (update_after_set)
	    update(key, value);
    }
};


#endif // __UNICALLBACKGEN_H
