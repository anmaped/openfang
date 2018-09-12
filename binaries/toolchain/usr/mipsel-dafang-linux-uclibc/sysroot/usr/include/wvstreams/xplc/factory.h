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
 *
 * As a special exception, you may use this file as part of a free
 * software library without restriction.  Specifically, if other files
 * instantiate templates or use macros or inline functions from this
 * file, or you compile this file and link it with other files to
 * produce an executable, this file does not by itself cause the
 * resulting executable to be covered by the GNU Lesser General Public
 * License.  This exception does not however invalidate any other
 * reasons why the executable file might be covered by the GNU Lesser
 * General Public License.
 */

#ifndef __XPLC_FACTORY_H__
#define __XPLC_FACTORY_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/IFactory.h>
#include <xplc/utils.h>

typedef IObject*(*FactoryFunc)();

/**
 * Generic factory class. Implements IFactory by calling the function
 * pointer that its constructor takes.
 */
class GenericFactory: public IFactory {
  IMPLEMENT_IOBJECT(GenericFactory);
private:
  FactoryFunc factory;
public:
  /**
   * Set up the generic factory. The generic factory will use the
   * function that the specified pointer indicates to answer to
   * IFactory::createObject requests.
   */
  GenericFactory(FactoryFunc aFactory);
  /* IFactory */
  virtual IObject* createObject();
};

#endif /* __XPLC_FACTORY_H__ */
