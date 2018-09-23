/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * FIXME: Suspiciously similar to wvstreamlist, and with tons of duplicated
 * code.  Blech.
 */ 
#ifndef __WVISTREAMLIST_H
#define __WVISTREAMLIST_H

#include "wvstream.h"

/** Create the WvStreamListBase class - a simple linked list of WvStreams */
DeclareWvList2(WvIStreamListBase, IWvStream);

/**
 * WvStreamList holds a list of WvStream objects -- and its select() and
 * callback() functions know how to handle multiple simultaneous streams.
 */
class WvIStreamList : public WvStream, public WvIStreamListBase
{
public:
    WvIStreamList();
    virtual ~WvIStreamList();
    virtual bool isok() const;
    virtual void pre_select(SelectInfo &si);
    virtual bool post_select(SelectInfo &si);
    virtual void execute();
    
    void unlink(IWvStream *data)
        { sure_thing.unlink(data); WvIStreamListBase::unlink(data); }

    void add_after(WvLink *after, IWvStream *data, bool autofree,
		   const char *id)
    {
	WvIStreamListBase::add_after(after, data, autofree, id);
    }
    void add(IWvStream *data, bool autofree, const char *id)
    {
	WvIStreamListBase::add(data, autofree, id);
    }
    void prepend(IWvStream *data, bool autofree, const char *id)
    {
	WvIStreamListBase::prepend(data, autofree, id);
    }

public: 
    bool auto_prune; // remove !isok() streams from the list automatically?
    static WvIStreamList globallist;
    
protected:
    WvIStreamListBase sure_thing;

private:
    // Create some undefined overrides to prevent accidentally using a
    // WvString as an id; these functions will keep a long-term reference to
    // the string, so you should probably use a string constant.
    void add_after(WvLink *after, IWvStream *data, bool autofree, WvString id);
    void add(IWvStream *data, bool autofree, WvString id);
    void prepend(IWvStream *data, bool autofree, WvString id);

private:
    bool in_select;
    bool dead_stream;

#ifndef _WIN32
    static void onfork(pid_t p);
#endif

public:
    void append(IWvStream *s, bool auto_free, const char *id)
    {
        if (s->wsname() == NULL)
            s->set_wsname(id);
        WvIStreamListBase::append(s, auto_free, id);
    }
    void append(IWvStream *s, bool auto_free, WVSTRING_FORMAT_DECL)
    {
        if (s->wsname() == NULL)
            s->set_wsname(WvString(WVSTRING_FORMAT_CALL));
        WvIStreamListBase::append(s, auto_free, s->wsname());
    }

public:
    const char *wstype() const { return "WvIStreamList"; }
    
private:
    static void add_debugger_commands();
private:
    static WvString debugger_globallist_run_cb(WvStringParm cmd,
        WvStringList &args,
        WvStreamsDebugger::ResultCallback result_cb, void *);
};

#endif // __WVISTREAMLIST_H
