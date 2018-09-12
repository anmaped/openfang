/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2004, Pierre Phaneuf
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

#ifndef __XPLC_IMODULEMANAGERFACTORY_H__
#define __XPLC_IMODULEMANAGERFACTORY_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IServiceHandler.h>

class IModuleManagerFactory: public IObject {
  UNSTABLE_INTERFACE
public:
  virtual IServiceHandler* createModuleManager(const char* directory) = 0;
};

DEFINE_IID(IModuleManagerFactory,{0x9c88d2d0, 0xc61f, 0x41b7,
  {0x9d, 0x4d, 0xd2, 0xc6, 0xc0, 0x97, 0x94, 0x55}});

#endif /* __XPLC_IMODULEMANAGERFACTORY_H__ */
