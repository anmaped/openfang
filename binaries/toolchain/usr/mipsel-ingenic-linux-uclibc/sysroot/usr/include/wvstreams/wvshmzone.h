/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Shared memory zones.
 */
#ifndef __WVSHMZONE_H
#define __WVSHMZONE_H

#include "wverror.h"

/**
 * Represents a shared-memory zone via mmap().
 * 
 * If you create one of these, its buf element will be
 * shared across fork() and you can use it for various things
 * such as a circular queue, semaphore, etc.
 * 
 */
class WvShmZone : public WvErrorBase
{
public:
    /**
     * Creates a shared memory zone.
     *
     * "size" is the size of the zone in bytes
     */
    WvShmZone(size_t size);
    ~WvShmZone();
    
private:
    int fd;
    
public:
    int size;
    
    union {
	void *buf;
	char *cbuf;
	unsigned char *ucbuf;
    };
};


#endif // __WVSHMZONE_h
