/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2003 Net Integration Technologies, Inc.
 *
 */
#ifndef __WVDELAYEDCALLBACK_H
#define __WVDELAYEDCALLBACK_H

#include "wvistreamlist.h"
#include "wvtr1.h"

/**
 * A WvCallback wrapper that delays until the next tick of the WvIStreamList
 * main loop.
 *
 * There are restrictions on the type of the wrapped callback though:
 *   1. The return type must be void
 *   2. All parameter types must be copy-constructible value types
 * 
 * Example: setcallback(wv::delayed(mycallback));
 * 
 * FIXME: Because operator() makes a copy of the inner callback and thaw()
 * destroys the copy, nesting WvDelayedCallback doesn't work as you might
 * expect.  That is, don't do: wv::delayed(wv::delayed(mycallback)).
 * It creates a copy of the inner WvDelayedCallback, but that copy
 * gets frozen, then destroyed before it has a chance to thaw!  Anyway,
 * it's a stupid thing to do anyway, so don't.
 */
template<class Functor>
class WvDelayedCallback
{
private:
    Functor func;
    WvStream *stream;
    wv::function<void()> frozen;

public:
  WvDelayedCallback(const Functor& _func):
      func(_func), stream(new WvStream), frozen(0)
    {
        WvIStreamList::globallist.append(stream, true, "WvDelayedCallback");
    }
    WvDelayedCallback(const WvDelayedCallback &other):
	func(other.func), stream(new WvStream), frozen(0)
    {
        WvIStreamList::globallist.append(stream, true, "WvDelayedCallback");
    }
    ~WvDelayedCallback()
    {
        stream->close();
    }
    void operator()()
    {
        stream->setcallback(func);
        stream->alarm(0);
    }
    template<typename P1>
    void operator()(P1 &p1)
    {
	stream->setcallback(wv::bind(func, p1));
        stream->alarm(0);
    }
    template<typename P1,
	     typename P2>
    void operator()(P1 &p1, P2 &p2)
    {
	stream->setcallback(wv::bind(func, p1, p2));
        stream->alarm(0);
    }
    template<typename P1,
	     typename P2,
	     typename P3>
    void operator()(P1 &p1, P2 &p2, P3 &p3)
    {
	stream->setcallback(wv::bind(func, p1, p2, p3));
        stream->alarm(0);
    }
    template<typename P1,
	     typename P2,
	     typename P3,
	     typename P4>
    void operator()(P1 &p1, P2 &p2, P3 &p3, P4 &p4)
    {
	stream->setcallback(wv::bind(func, p1, p2, p3, p4));
        stream->alarm(0);
    }
    template<typename P1,
	     typename P2,
	     typename P3,
	     typename P4,
	     typename P5>
    void operator()(P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5)
    {
	stream->setcallback(wv::bind(func, p1, p2, p3, p4, p5));
        stream->alarm(0);
    }
    template<typename P1,
	     typename P2,
	     typename P3,
	     typename P4,
	     typename P5,
	     typename P6>
    void operator()(P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6)
    {
	stream->setcallback(wv::bind(func, p1, p2, p3, p4, p5, p6));
        stream->alarm(0);
    }
    template<typename P1,
	     typename P2,
	     typename P3,
	     typename P4,
	     typename P5,
	     typename P6,
	     typename P7>
    void operator()(P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6, P7 &p7)
    {
	stream->setcallback(wv::bind(func, p1, p2, p3, p4, p5, p6, p7));
        stream->alarm(0);
    }
    template<typename P1,
	     typename P2,
	     typename P3,
	     typename P4,
	     typename P5,
	     typename P6,
	     typename P7,
	     typename P8>
    void operator()(P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6, P7 &p7,
		    P8 &p8)
    {
	stream->setcallback(wv::bind(func, p1, p2, p3, p4, p5, p6, p7, p8));
        stream->alarm(0);
    }
};


/*
 * We put the following in the wv:: namespace so that they match wv::bind
 * and wv::function from wvtr1.h.
 */
namespace wv
{
    /**
     * A convenience function for constructing WvDelayedCallback objects 
     * from wv::functions without explicitly specifying the type.  
     * Example: 
     *    typedef wv::function<void(int, int)> Func;
     *    Func f = wv::delayed(wv::bind(mycallback, 5, _1));
     *    f(10);
     */
    template <typename T>
    inline T delayed(T cb)
    {
	return WvDelayedCallback<T>(cb);
    }
    
    /**
     * A convenience function for constructing WvDelayedCallback objects 
     * from function pointers without explicitly specifying the type.  
     * Example: 
     *    typedef wv::function<void(int)> Func;
     *    Func f = wv::delayed(mycallback);
     *    f(10);
     */
    template <typename T>
    inline wv::function<T> delayed(T *cb)
    {
	return WvDelayedCallback< wv::function<T> >(cb);
    }
}

#endif
