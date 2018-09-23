/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
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

#ifndef __XPLC_ICATEGORY_H__
#define __XPLC_ICATEGORY_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IFactory.h>
#include <xplc/ICategoryIterator.h>

/**
 * Represents a category.
 */

class ICategory: public IObject {
  UNSTABLE_INTERFACE
public:
  /** Gets an iterator for the category. */
  virtual ICategoryIterator* getIterator() = 0;
};

DEFINE_IID(ICategory, {0x90abfe8d, 0x50a9, 0x44d8,
  {0x96, 0x03, 0x29, 0x9c, 0x8b, 0x21, 0x9e, 0x5d}});

#endif /* __XPLC_ICATEGORYMANAGER_H__ */
