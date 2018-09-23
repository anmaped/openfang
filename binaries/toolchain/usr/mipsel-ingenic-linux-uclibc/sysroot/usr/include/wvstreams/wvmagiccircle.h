/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A circular queue that can be accessed across fork().
 */
#ifndef __WVMAGICCIRCLE_H
#define __WVMAGICCIRCLE_H

#include "wvshmzone.h"


/** A circular queue that can be accessed across fork(). */
class WvMagicCircle : public WvErrorBase
{
public:
    /**
     * Creates a shared memory circular queue.
     *
     * "size" is the number of elements to store
     */
    WvMagicCircle(size_t size);
    ~WvMagicCircle();
    
protected:
    WvShmZone shm;
    
    volatile int &head, &tail;
    int size;
    char *circle;
    
public:
    size_t used();
    size_t left()
	{ return size - used() - 1; }
    size_t put(const void *data, size_t len);
    size_t get(void *data, size_t len);
    size_t skip(size_t len);
    
public:
    const char *wstype() const { return "WvMagicCircle"; }
};


#endif // __WVMAGICCIRCLE_H
