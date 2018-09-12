/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * FIXME: I was too lazy to templatize this properly, so we only support
 * WvCallback<void*,void*>.  It should be possible to work with any kind
 * of return value and parameter, although it makes sense to limit things
 * to just one parameter (since it currently has to be returned by yield()
 * somehow).
 */

#ifndef __WVCONT_H
#define __WVCONT_H

#include "wvlinklist.h"
#include "wvstreamsdebugger.h"
#include "wvtr1.h"

typedef wv::function<void*(void*)> WvContCallback;

/**
 * WvCont provides "continuations", which are apparently also known as
 * semi-coroutines.  You can wrap any WvCallback<void*,void*> in a WvCont
 * and make it a "continuable" callback - that is, you can yield() from it
 * and return a value.  Next time someone calls your callback, it will be
 * as if yield() has returned (and the parameter to your function is returned
 * from yield()).
 */
class WvCont
{
    struct Data;
    friend struct Data;
    typedef WvList<Data> DataList;
    
private:
    /**
     * When we copy a WvCont, we increase the reference count of the 'data'
     * member rather than copying it.  That makes it so every copy of a given
     * callback object still refers to the same WvTask.
     */
    Data *data;
    static DataList *data_list;

    static Data *curdata;
    static int taskdepth;
    
    static void bouncer(void *userdata);
    
    /**
     * Actually call the callback inside its task, and enforce a call stack.
     * Doesn't do anything with arguments.  Returns the return value.
     */
    void *call()
	{ return _call(data); }
    
    /**
     * Call the callback inside its task, but don't assume this WvCont will
     * still be around when we come back.
     */
    static void *_call(Data *data);

    /**
     * Construct a WvCont given a pre-existing Data structure.  This is
     * basically equivalent to using the copy constructor.
     */
    WvCont(Data *data);

public:
    /**
     * Construct a WvCont using an existing WvCallback.  The WvCont object
     * can be used in place of that callback, and stored in a callback of
     * the same data type.
     */
    WvCont(const WvContCallback &cb, unsigned long stacksize = 64*1024);
    
    /** Copy constructor. */
    WvCont(const WvCont &cb);
    
    /** Destructor. */
    ~WvCont();
    
    /**
     * call the callback, making p1 the return value of yield() or the
     * parameter to the function, and returning Ret, the argument of yield()
     * or the return value of the function.
     */
    void *operator() (void *p1 = 0);
    
    // the following are static because a function doesn't really know
    // which WvCont it belongs to, and only one WvCont can be the "current"
    // one globally in an application anyway.
    // 
    // Unfortunately this prevents us from assert()ing that you're in the
    // context you think you are.
    
    /**
     * Get a copy of the current WvCont.
     */
    static WvCont current();

    /**
     * "return" from the current callback, giving value 'ret' to the person
     * who called us.  Next time this callback is called, it's as if yield()
     * had returned, and the parameter to the callback is the value of
     * yield().
     */
    static void *yield(void *ret = 0);
    
    /**
     * Tell us if the current context is "okay", that is, not trying to
     * die. If !isok(), you shouldn't yield(), because the caller is just
     * going to keep calling you until you die.  Return as soon as you can.
     */
    static bool isok();

    
    /**
     * A templated function that allows you to pass a WvCont wherever a
     * C-style function pointer of the form
     *      R func(T, void *userdata)
     * is taken.  It's your job to make sure the 'userdata' provided is
     * a pointer to the right WvCont.
     * 
     * Example:
     *     typedef bool MyFunc(Obj *obj, void *userdata);
     *     WvCont cont;
     *     MyFunc *func = &WvCont::c_bouncer<bool,Obj *>;
     *     bool b = func(new Obj, &cont);
     */
    template <typename R, typename T>
	static R c_bouncer(T t, void *_cont)
	{
	    WvCont &cont = *(WvCont *)_cont;
	    return (R)cont((T)t);
	}


    /**
     * A templated function that allows you to pass a WvCont wherever a
     * C-style function pointer of the form
     *      R func(void *userdata)
     * is taken.  It's your job to make sure the 'userdata' provided is
     * a pointer to the right WvCont.
     * 
     * Example:
     *     typedef bool MyFunc(void *userdata);
     *     WvCont cont;
     *     MyFunc *func = &WvCont::c_bouncer<bool>;
     *     bool b = func(&cont);
     */
    template <typename R>
     static R c_bouncer(void *_cont)
	{
	    WvCont &cont = *(WvCont *)_cont;
	    return (R)cont(0);
	}

private:
    static WvString debugger_conts_run_cb(WvStringParm cmd, WvStringList &args,
        WvStreamsDebugger::ResultCallback result_cb, void *);
};

#endif // __WVCONT_H

