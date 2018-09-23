/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 */
#ifndef __WVBATCHSIGNAL_H
#define __WVBATCHSIGNAL_H

#include <uniconf.h>
#include <wvistreamlist.h>
#include <wvtr1.h>


class WvInvertedStream: public WvStream
{
public:
    WvInvertedStream(char *_id):
	WvStream()
    {
	WvIStreamList::globallist.append(this, false, _id);
    }
    ~WvInvertedStream()
    {
	WvIStreamList::globallist.unlink(this);
    }
};


/*
 * This class is a functor compatible with UniConfCallback,
 * IWvStreamCallback and WvStreamCallback, as well as supporting being
 * called with no parameters.
 *
 * It will turn any number of calls to any of these callbacks into a
 * single call to the callback you give to it, which will be sent on
 * the next run through the main loop.
 *
 * Think of it as an elevator button: pressing it a bunch of times has
 * no more effect than pressing it once, and while it doesn't do what
 * you tell it right away, it will do it soon enough.
 */
class WvBellPull
{
public:
    WvBellPull(WvCallback<> _cb):
	cb(_cb),
	bellpull(new WvInvertedStream("bellpull"))
    {
	bellpull->setcallback(
	    WvStreamCallback(this, &WvBellPull::bellpull_cb), NULL);
    }

    WvBellPull(const WvBellPull& _other):
	cb(_other.cb),
	bellpull(_other.bellpull)
    {
	bellpull->addRef();
    }

    ~WvBellPull()
    {
	bellpull->release();
    }
    void delay(time_t msec_timeout)
    {
	bellpull->alarm(msec_timeout);
    }
    void cancel()
    {
	bellpull->alarm(-1);
    }
    void operator()()
    {
	bellpull->alarm(0);
    }
    void operator()(IWvStream&)
    {
	bellpull->alarm(0);
    }
    void operator()(WvStream&, void*)
    {
	bellpull->alarm(0);
    }
    void operator()(const UniConf &, const UniConfKey &)
    {
	bellpull->alarm(0);
    }

private:
    WvCallback<> cb;
    WvInvertedStream *bellpull;

    void bellpull_cb(WvStream&, void*)
    {
	cb();
    }
};

#endif /* __WVBATCHSIGNAL_H */
