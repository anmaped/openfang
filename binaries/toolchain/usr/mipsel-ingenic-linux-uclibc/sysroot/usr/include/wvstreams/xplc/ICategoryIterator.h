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

#ifndef __XPLC_ICATEGORYITERATOR_H__
#define __XPLC_ICATEGORYITERATOR_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IObject.h>

/** \interface ICategoryIterator ICategoryIterator.h xplc/ICategoryIterator.h
 *
 * Allows iteration over a category.
 */

class ICategoryIterator: public IObject {
  UNSTABLE_INTERFACE
public:
  /** Returns the UUID of the category entry. */
  virtual const UUID& getUuid() = 0;
  /** Returns the string associated with the category entry. */
  virtual const char* getString() = 0;
  /** Advances to the next category entry. */
  virtual void next() = 0;
  /** Tests if the iterator is past the last item. The iterator is
   * invalid when this method returns true. */
  virtual bool done() = 0;
};

DEFINE_IID(ICategoryIterator, {0x87e48aae, 0xa1da, 0x4d9c,
  {0xa7, 0xc0, 0x7a, 0x5b, 0x88, 0xf4, 0x01, 0x7a}});

#endif /* __XPLC_ICATEGORYITERATOR_H__ */
