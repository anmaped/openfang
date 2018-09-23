/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2000-2003, Pierre Phaneuf
 * Copyright (C) 2002, Net Integration Technologies, Inc.
 * Copyright (C) 2004, Stéphane Lajoie
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

#ifndef __XPLC_UUIDOPS_H__
#define __XPLC_UUIDOPS_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

#include <xplc/uuid.h>

#if !defined _SYS_GUID_OPERATOR_EQ_ && !defined _NO_SYS_GUID_OPERATOR_EQ_
#define _SYS_GUID_OPERATOR_EQ_

/**
 * Equality operator for UUIDs.
 */
inline int operator==(const UUID& uuid1, const UUID& uuid2) {
    return
      (&uuid1 == &uuid2) ||
      ((static_cast<const u_int32_t*>(&uuid1.Data1)[0] == static_cast<const u_int32_t*>(&uuid2.Data1)[0]) &&
       (static_cast<const u_int32_t*>(&uuid1.Data1)[1] == static_cast<const u_int32_t*>(&uuid2.Data1)[1]) &&
       (static_cast<const u_int32_t*>(&uuid1.Data1)[2] == static_cast<const u_int32_t*>(&uuid2.Data1)[2]) &&
       (static_cast<const u_int32_t*>(&uuid1.Data1)[3] == static_cast<const u_int32_t*>(&uuid2.Data1)[3]));
}

/**
 * Inequality operator for UUIDs.
 */
inline int operator!=(const UUID& uuid1, const UUID& uuid2) {
    return
      (&uuid1 != &uuid2) &&
      ((static_cast<const u_int32_t*>(&uuid1.Data1)[0] != static_cast<const u_int32_t*>(&uuid2.Data1)[0]) ||
       (static_cast<const u_int32_t*>(&uuid1.Data1)[1] != static_cast<const u_int32_t*>(&uuid2.Data1)[1]) ||
       (static_cast<const u_int32_t*>(&uuid1.Data1)[2] != static_cast<const u_int32_t*>(&uuid2.Data1)[2]) ||
       (static_cast<const u_int32_t*>(&uuid1.Data1)[3] != static_cast<const u_int32_t*>(&uuid2.Data1)[3]));
}

#endif

/**
 * Converts a printable C string to a UUID.  The string is of the form
 * "9c318f06-52ec-4a62-b5fb-9279216e8586" (without the quotes), but
 * will also tolerate opening and closing braces before and after.
 * 
 * This is the form produced by the 'uuidgen' program and is used in
 * the Windows registry.
 */
const UUID UuidFromString(const char* str);

/**
 * Fill the given pointer with a printable string representing the
 * given UUID.  The string is of the form
 * "9c318f06-52ec-4a62-b5fb-9279216e8586" (without the quotes).
 * 
 * 'str' should point to at least 39 bytes of available memory.
 * Always returns 'str', which has been null-terminated.
 */
char* UuidToString(const UUID& uuid, char* str);

#endif /* __XPLC_UUIDOPS_H__ */
