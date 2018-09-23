/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2000-2002, Pierre Phaneuf
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

#ifndef __XPLC_IFACTORY_H__
#define __XPLC_IFACTORY_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IObject.h>

/** \interface IFactory IFactory.h xplc/IFactory.h
 *
 * Common object creation interface.
 */

class IFactory: public IObject {
  UNSTABLE_INTERFACE
public:
  /**
   * Creates an object.
   *
   * The object returned by IFactory::createObject() is already
   * addRef()'d.
   */
  virtual IObject* createObject() = 0;
};

DEFINE_IID(IFactory, {0xcd386b27, 0x0ea1, 0x4e1b,
  {0xba, 0x08, 0xb8, 0x5e, 0xe4, 0xda, 0xad, 0x69}});

#endif /* __XPLC_IFACTORY_H__ */
