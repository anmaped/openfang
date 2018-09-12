/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */
#ifndef __WVCALLBACKLIST_H
#define __WVCALLBACKLIST_H

#include <assert.h>
#include <map>


template<class InnerCallback>
class WvCallbackList
{
private:
    std::map<void*, InnerCallback> list;

    /* The idea of copying this gives me a headache. */
    WvCallbackList(const WvCallbackList &);
    WvCallbackList& operator=(const WvCallbackList&);

public:
    WvCallbackList()
    {
    }
    void add(const InnerCallback &cb, void *cookie)
    {
	assert(list.find(cookie) == list.end());
	list.insert(std::make_pair(cookie, cb));
    }
    void del(void *cookie)
    {
	typename std::map<void*, InnerCallback>::iterator it =
	    list.find(cookie);
	assert(it != list.end());
	list.erase(it);
    }
    bool isempty() const
    {
	return list.empty();
    }
    void operator()() const
    {
	typename std::map<void*, InnerCallback>::const_iterator it;

	for (it = list.begin(); it != list.end(); ++it)
	    it->second();
    }
    template<typename P1>
    void operator()(P1 &p1) const
    {
	typename std::map<void*, InnerCallback>::const_iterator it;

	for (it = list.begin(); it != list.end(); ++it)
	    it->second(p1);
    }
    template<typename P1,
	     typename P2>
    void operator()(P1 &p1, P2 &p2) const
    {
	typename std::map<void*, InnerCallback>::const_iterator it;

	for (it = list.begin(); it != list.end(); ++it)
	    it->second(p1, p2);
    }
    template<typename P1,
	     typename P2,
	     typename P3>
    void operator()(P1 &p1, P2 &p2, P3 &p3) const
    {
	typename std::map<void*, InnerCallback>::const_iterator it;

	for (it = list.begin(); it != list.end(); ++it)
	    it->second(p1, p2, p3);
    }
    template<typename P1,
	     typename P2,
	     typename P3,
	     typename P4>
    void operator()(P1 &p1, P2 &p2, P3 &p3, P4 &p4) const
    {
	typename std::map<void*, InnerCallback>::const_iterator it;

	for (it = list.begin(); it != list.end(); ++it)
	    it->second(p1, p2, p3, p4);
    }
    template<typename P1,
	     typename P2,
	     typename P3,
	     typename P4,
	     typename P5>
    void operator()(P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5) const
    {
	typename std::map<void*, InnerCallback>::const_iterator it;

	for (it = list.begin(); it != list.end(); ++it)
	    it->second(p1, p2, p3, p4, p5);
    }
    template<typename P1,
	     typename P2,
	     typename P3,
	     typename P4,
	     typename P5,
	     typename P6>
    void operator()(P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6) const
    {
	typename std::map<void*, InnerCallback>::const_iterator it;

	for (it = list.begin(); it != list.end(); ++it)
	    it->second(p1, p2, p3, p4, p5, p6);
    }
    template<typename P1,
	     typename P2,
	     typename P3,
	     typename P4,
	     typename P5,
	     typename P6,
	     typename P7>
    void operator()(P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6,
		    P7 &p7) const
    {
	typename std::map<void*, InnerCallback>::const_iterator it;

	for (it = list.begin(); it != list.end(); ++it)
	    it->second(p1, p2, p3, p4, p5, p6, p7);
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
		    P8 &p8) const
    {
	typename std::map<void*, InnerCallback>::const_iterator it;

	for (it = list.begin(); it != list.end(); ++it)
	    it->second(p1, p2, p3, p4, p5, p6, p7, p8);
    }
};


#endif /* __WVCALLBACKLIST_H */
