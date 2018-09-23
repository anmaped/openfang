/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2000-2003, Pierre Phaneuf
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

#ifndef __XPLC_XPLC_H__
#define __XPLC_XPLC_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

/** \file
 *
 * The XPLC helper class for the C++ binding.
 */

#include <xplc/core.h>
#include <xplc/utils.h>
#include <xplc/ptr.h>

/** \class XPLC xplc.h xplc/xplc.h
 *
 * The XPLC helper class.  This class is part of the XPLC C++ binding
 * in order to provide a more natural C++ feel to the use of XPLC.
 */

class XPLC {
private:
  xplc_ptr<IServiceManager> servmgr;
public:
  XPLC(): servmgr(XPLC_getServiceManager()) {}
  /**
   * Create an XPLC object using an existing service manager
   * reference.
   */
  XPLC(IServiceManager* _servmgr): servmgr(do_addRef(_servmgr)) {}

  /**
   * Adds a directory to the module loader path.
   */
  void addModuleDirectory(const char* directory);

  /**
   * Obtain an XPLC object. Obtains an object with the provided UUID
   * from the service manager.
   */
  IObject* get(const UUID& uuid) {
    return servmgr->getObject(uuid);
  }
  /**
   * Templated variant of XPLC::get() that will do a getInterface()
   * for you.
   */
  template<class Interface>
  Interface* get(const UUID& uuid) {
    return mutate<Interface>(servmgr->getObject(uuid));
  }

  /**
   * Object creation helper.  Obtains an object with the provided UUID
   * from the service manager, tries to get the IFactory interface
   * from the object and calls its createObject() method.
   */
  IObject* create(const UUID& cid);
  /**
   * Templated variant of XPLC::create() that will do a getInterface()
   * for you.
   */
  template<class Interface>
  Interface* create(const UUID& cid) {
    return mutate<Interface>(create(cid));
  }

  /**
   * Object creation helper that operates from a moniker. Works like
   * XPLC::create(const UUID&), but finds the object using a moniker
   * instead.
   */
  IObject* create(const char*);
  /**
   * Templated variant of XPLC::create(const char*) that will do a
   * getInterface() for you.
   */
  template<class Interface>
  Interface* create(const char* aMoniker) {
    return mutate<Interface>(create(aMoniker));
  }
};

#endif /* __XPLC_XPLC_H__ */
