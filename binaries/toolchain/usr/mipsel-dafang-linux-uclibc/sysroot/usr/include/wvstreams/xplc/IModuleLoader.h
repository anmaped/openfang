/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2002-2004, Pierre Phaneuf
 * Copyright (C) 2002-2004, Net Integration Technologies, Inc.
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

#ifndef __XPLC_IMODULELOADER_H__
#define __XPLC_IMODULELOADER_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IModule.h>

/** \interface IModuleLoader IModuleLoader.h xplc/IModuleLoader.h
 *
 * The interface to control the module loader.
 */

class IModuleLoader: public IObject {
  UNSTABLE_INTERFACE
public:
  virtual IModule* loadModule(const char* modulename) = 0;
};

DEFINE_IID(IModuleLoader, {0xa4143690, 0xfc54, 0x43f4,
  {0x9d, 0x7e, 0x50, 0x3c, 0x71, 0xff, 0xa5, 0x01}});

#endif /* __XPLC_IMODULELOADER_H__ */
