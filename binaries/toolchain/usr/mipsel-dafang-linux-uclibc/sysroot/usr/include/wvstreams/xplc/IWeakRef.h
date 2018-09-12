/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2003, Pierre Phaneuf
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

#ifndef __XPLC_IWEAKREF_H__
#define __XPLC_IWEAKREF_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IObject.h>

/** \interface IWeakRef IWeakRef.h xplc/IWeakRef.h
 *
 * Represents a weak reference to another object.
 */

class IWeakRef: public IObject {
  UNSTABLE_INTERFACE
public:
  /**
   * Obtains an addRef()'d strong reference to the referenced
   * object. If the object has been destroyed, this will return
   * NULL. Do not forget to release() the strong reference when you
   * are done with it.
   */
  virtual IObject* getObject() = 0;
};

DEFINE_IID(IWeakRef, {0x6a97f962, 0xeeec, 0x48e2,
  {0xb4, 0x68, 0x56, 0x60, 0x57, 0x3f, 0xd9, 0x24}});

#endif /* __XPLC_IWEAKREF_H__ */
