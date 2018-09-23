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

#ifndef __XPLC_IMONIKERSERVICE_H__
#define __XPLC_IMONIKERSERVICE_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IMoniker.h>

/** \interface IMonikerService IMonikerService.h xplc/IMonikerService.h
 *
 * An interface for registering objects so they can be retrieved using
 * a moniker.
 * 
 * You can obtain the %XPLC standard moniker service by getting the
 * XPLC_monikers UUID from the service manager.
 */
class IMonikerService: public IMoniker {
  UNSTABLE_INTERFACE
public:
  /**
   * Register an object to be retrieved with a moniker.
   */
  virtual void registerObject(const char* prefix, const UUID& uuid) = 0;
};

DEFINE_IID(IMonikerService, {0x0ee2cc09, 0xd7d5, 0x44ee,
  {0xbc, 0x63, 0xf2, 0xb9, 0x03, 0x7d, 0xb9, 0x82}});

#endif /* __XPLC_IMONIKERSERVICE_H__ */
