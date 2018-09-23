/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * Support for monikers, which are strings that you can pass to a magic
 * factory to get objects supporting a particular interface, without actually
 * knowing anything about the constructor for those objects.
 */
#ifndef __WVMONIKER_H
#define __WVMONIKER_H

#include "wvstring.h"
#include "wvxplc.h"

class WvMonikerRegistry;

typedef void *WvMonikerCreateFunc(WvStringParm parms, IObject *obj);

/**
 * WvMonikerBase is an auto-registration class for putting things into
 * a WvMonikerRegistry.  When a WvMonikerBase instance is created, it
 * registers a moniker prefix ("test:", "ssl:", "ini:", etc) and a factory
 * function that can be used to create an IObject using that prefix.
 * 
 * When the instance is destroyed, it auto-unregisters the moniker prefix
 * from the registry.
 * 
 * You can't actually create one of these, because it's not typesafe.  See
 * WvMoniker<T> instead.
 */
class WvMonikerBase
{
protected:
    WvMonikerBase(const UUID &iid, WvStringParm _id,
		  WvMonikerCreateFunc *func, const bool override = false);
    ~WvMonikerBase();
    
public:
    WvString id;
    WvMonikerRegistry *reg;
};


/**
 * A type-safe version of WvMonikerBase that lets you provide create functions
 * for object types other than IObject.  (The objects themselves have to
 * be derived from IObject, however.)
 * 
 * See WvMonikerBase for details.
 * 
 * Example:
 *    static IWvStream *createfunc(WvStringParm s, IObject *obj,
 *                                 void *userdata)
 *    {
 *        return new WvStream;
 *    }
 * 
 *    static WvMoniker<IWvStream> registration("ssl", createfunc);
 */
template <class T>
class WvMoniker : public WvMonikerBase
{
public:
    typedef T *CreateFunc(WvStringParm parms, IObject *obj);
    
    WvMoniker(WvStringParm _id, CreateFunc *_func, const bool override = false)
	: WvMonikerBase(XPLC_IID<T>::get(), _id, (WvMonikerCreateFunc *)_func,
			override)
    { 
	// this looks pointless, but it ensures that T* can be safely,
	// automatically downcast to IObject*.  That means T is really derived
	// from IObject, which is very important. The 'for' avoids a
	// warning.
	for(IObject *silly = (T *)NULL; silly; )
            ;
    };
};


/**
 * Create an object registered in a WvMonikerRegistry.  The iid specifies
 * which registry to look in, and s, obj, and userdata are the parameters to
 * supply to the object's factory.  Most factories need only 's', which is the
 * moniker itself.
 * 
 * Most people don't use this function.  See the templated, type-safe version
 * of wvcreate() below.
 */
void *wvcreate(const UUID &iid, WvStringParm s, IObject *obj);


/**
 * Create an object registered in a WvMonikerRegistry.  Exactly which
 * registry is determined by the template type T.
 * 
 * s, obj, and userdata are the parameters to supply to the object's
 * factory.  Most factories need only 's', which is the moniker itself.
 * 
 * Example:
 *    IWvStream *s = wvcreate<IWvStream>("tcp:localhost:25");
 *    IWvStream *s_ssl = wvcreate<IWvStream>("ssl:", s);
 */
template <class T>
inline T *wvcreate(WvStringParm s, IObject *obj = 0)
{
    return (T *)(wvcreate(XPLC_IID<T>::get(), s, obj));
}


#endif // __WVMONIKER_H
