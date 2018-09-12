/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2002-2003, Net Integration Technologies, Inc.
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

#ifndef __XPLC_ICATEGORYMANAGER_H__
#define __XPLC_ICATEGORYMANAGER_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IServiceHandler.h>
#include <xplc/ICategory.h>

/** \interface ICategoryManager ICategoryManager.h xplc/ICategoryManager.h
 *
 * Let you register categories and obtain information about them.
 */

class ICategoryManager: public IObject {
  UNSTABLE_INTERFACE
public:
  /**
   * Register a component with a category. Both are specified by
   * UUIDs.
   */
  virtual void registerComponent(const UUID& category,
                                 const UUID& component,
                                 const char* extrastring) = 0;
  /**
   * Get a category object for the specified category.
   */
  virtual ICategory* getCategory(const UUID&) = 0;
};

DEFINE_IID(ICategoryManager, {0xb5f218a5, 0xb50a, 0x4e8c,
  {0x9e, 0x0e, 0x69, 0x2e, 0x17, 0xf0, 0xe2, 0x99}});

#endif /* __XPLC_ICATEGORYMANAGER_H__ */
