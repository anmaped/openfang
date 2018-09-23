/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2000-2003, Pierre Phaneuf
 * Copyright (C) 2002, Net Integration Technologies, Inc.
 * Copyright (C) 2002, Stéphane Lajoie
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

#ifndef __XPLC_IOBJECT_H__
#define __XPLC_IOBJECT_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

/** \file
 *
 * This file contains the %XPLC foundation interface.
 */

#include <xplc/uuid.h>

#ifndef UNSTABLE
/**
 * Used to mark an interface as unstable.  Add an UNSTABLE_INTERFACE
 * declaration to your object if you don't want people to use your
 * interface without knowing the interface might change from
 * underneath them.  They will then have to \#define UNSTABLE if they
 * want their program to compile.
 */
#define UNSTABLE_INTERFACE static bool UNSTABLE_INTERFACE = true;
#else
#define UNSTABLE_INTERFACE
#endif

class IWeakRef;

/** \interface IObject IObject.h xplc/IObject.h
 *
 * The basic interface which is included by all other %XPLC interfaces
 * and objects.  It provides a standard method to say you're using an
 * object, to say you're done, and to retrieve other %XPLC interfaces
 * from the object.
 * 
 * Usually you will not implement these functions yourself; use
 * the IMPLEMENT_IOBJECT macro instead.
 * 
 * \sa IMPLEMENT_IOBJECT, mutate(), get(), xplc_ptr, XPLC
 */
class IObject {
  UNSTABLE_INTERFACE
public:
  /**
   * Indicate you are using this object.  This increases the reference
   * count of the object by one.  Usually, when the reference count
   * reaches zero, the object is freed automatically.
   * 
   * This called a "strong reference", because they will prevent the
   * object from being destroyed. They should thus be used carefully,
   * as they control the lifetime of the object. For example, you do
   * not need to call addRef() on object passed as parameters, unless
   * you intend on keeping them.
   *
   * addRef() is often called automatically for you in %XPLC, but
   * you'll have to call release() by hand sometimes unless you use
   * xplc_ptr.
   */
  virtual unsigned int addRef() = 0;
    
  /**
   * Indicate that you are finished using this object.  This decreases
   * the reference count of the object by one.  Usually, when the
   * reference count reaches zero, the object is freed automatically.
   * 
   * You will usually need to manually release() any object given to
   * you by any other %XPLC function, unless you give the object to
   * someone else who will call release.
   * 
   * If you use xplc_ptr, it will do this for you.
   */
  virtual unsigned int release() = 0;
    
  /**
   * Returns the requested %XPLC interface. Will return NULL if the
   * interface is not supported.  The returned interface has been
   * addRef()ed, so you will need to release() it when done.  Note
   * that the interface returned may be a pointer to the same object
   * or a different one - that's none of your business. Asking for the
   * IObject interface should always return the same pointer for a
   * given logical object, so this can be used for comparison by
   * identity.
   * 
   * You should probably use the convenient mutate() and get()
   * functions instead of this, or use an xplc_ptr, which mutates the
   * object for you.
   */
  virtual IObject* getInterface(const UUID&) = 0;
    
  /**
   * Return a weak reference to this object.  A weak reference points
   * at the object, but does not control the lifetime of the object.
   * An object can thus still be deleted while someone holds a weak
   * reference.  You will still need to release() the weak reference
   * when you are done with it.
   *
   * \sa IWeakRef
   */
  virtual IWeakRef* getWeakRef() = 0;
};

/// IObject's IID
DEFINE_IID(IObject, {0x8ca76e98, 0xb653, 0x43d7,
  {0xb0, 0x56, 0x8b, 0x9d, 0xde, 0x9a, 0xbe, 0x9d}});

#endif /* __XPLC_IOBJECT_H__ */
