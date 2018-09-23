/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2002, Pierre Phaneuf
 * Copyright (C) 2002, Net Integration Technologies, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#ifndef __XPLC_IMONIKER_H__
#define __XPLC_IMONIKER_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IObject.h>

/** \interface IMoniker IMoniker.h xplc/IMoniker.h
 *
 * An interface for obtaining an IObject given a moniker string.
 *
 * A moniker can be used to obtain a particular object using a
 * human-readable string to describe it, rather than having to know
 * the object's UUID.  Human-readable strings are, unfortunately, not
 * guaranteed to be universally unique, so you might (theoretically)
 * not get the object you want.
 * 
 * You can retrieve the standard %XPLC moniker service from the service
 * manager using the XPLC_monikers UUID, which can then be used to
 * resolve monikers and register your own using the IMonikerService
 * interface it provides.
 *
 * Note that XPLC::get() and XPLC::create() are a convenient interface
 * to the %XPLC moniker system.
 */
class IMoniker: public IObject {
  UNSTABLE_INTERFACE
public:
  /**
   * Given a moniker string, return the IObject it refers to, or NULL if
   * no objects match.  The returned object is already addRef()'d.
   */
  virtual IObject* resolve(const char* moniker) = 0;
};

DEFINE_IID(IMoniker, {0x6c0bb432, 0x7c32, 0x4614,
  {0xa5, 0xab, 0xb2, 0x5d, 0x92, 0x23, 0xda, 0xa2}});

#endif /* __XPLC_IMONIKER_H__ */
