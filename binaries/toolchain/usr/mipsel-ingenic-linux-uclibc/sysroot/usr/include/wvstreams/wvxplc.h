/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * XPLC setup bits.
 */ 
#ifndef __WVXPLC_H
#define __WVXPLC_H

#ifndef UNSTABLE
#define UNSTABLE
#endif

#ifdef _WIN32
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "wvwin32-sanitize.h"
#else
#include <unistd.h> // not strictly necessary, but EVERYBODY uses this...
#include <sys/time.h>
#include "wvautoconf.h"
#endif

#ifndef ENABLE_DELETE_DETECTOR
#include <xplc/IObject.h>
#define deletev delete[]
#else
#include <string>
#include <xplc/delete.h>
#endif

#include <xplc/xplc.h>
#include <xplc/ptr.h>
#include <xplc/uuidops.h>

#define WVRELEASE(ptr) do { if (ptr) ptr->release(); ptr = 0; } while (0)
#define WVDELETE(ptr) do { delete ptr; ptr = 0; } while (0)

#endif // __WVXPLC_H
