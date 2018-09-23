/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2004, Net Integration Technologies, Inc.
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

#ifndef __XPLC_DELETE_H__
#define __XPLC_DELETE_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

/** \file
 *
 * Include this file right after IObject.h (or instead of) to detect
 * accidental uses of "delete" on an %XPLC interface. Use "xplcdelete"
 * instead of "delete" when you know what you are doing.
 */

#include <new>
#include <memory>

#ifdef __XPLC_IOBJECT_H__
#error "<xplc/delete.h> has to be included before <xplc/IObject.h>."
#endif

#include <xplc/IObject.h>

/**
 * \defgroup DeleteDetector This is the "delete detector".
 */

/// \ingroup DeleteDetector
class CheckIObject {};
class CheckIObjectOk {};
/// \ingroup DeleteDetector
class CheckIObjectOkVector {};

template<class T>
class ConversionIObject
{
public:
  //@{
  typedef char Yes;
  /// \ingroup DeleteDetector
  struct No { char dummy[2]; };
  static T* from;
  static Yes test(const IObject*);
  static No test(...);
  //@}
};

template<bool>
struct XPLC_CTAssert;
template<>
/// \ingroup DeleteDetector
struct XPLC_CTAssert<true> {};

template<class T>
inline void operator&&(CheckIObject, const T* obj) {
  static_cast<void>(sizeof(XPLC_CTAssert<(sizeof(ConversionIObject<T>::test(ConversionIObject<T>::from)) != sizeof(typename ConversionIObject<T>::Yes))>));
  delete obj;
}

template<class T>
inline void operator&&(CheckIObjectOk, const T* obj) {
  delete obj;
}

template<class T>
inline void operator&&(CheckIObjectOkVector, const T* obj) {
  delete[] obj;
}


/**
 * Undefine xplcdelete. <xplc/utils.h> defines xplcdelete, we should
 * undo this, in case it has been included before.
 */
#undef xplcdelete

/**
 * Macro used to indicate a valid use of the delete keyword with an
 * XPLC interface. In some cases, you really need to use delete on an
 * object that derives from IObject. In those cases, use "xplcdelete"
 * instead of "delete".
 */
#define xplcdelete CheckIObjectOk() &&

/**
 * Overriding the delete keyword. This replaces the delete keyword
 * with an invocation of the operator&& using a specific marker class
 * as the left operand, allowing it to be templated on the right
 * operand.
 */
#define delete CheckIObject() &&

/**
 * Remplacement for delete[]. Because we cannot capture usage of
 * delete[] using macros, we have to add a replacement for it,
 * unfortunately.
 */
#define deletev CheckIObjectOkVector() &&

#endif /* __XPLC_DELETE_H__ */
