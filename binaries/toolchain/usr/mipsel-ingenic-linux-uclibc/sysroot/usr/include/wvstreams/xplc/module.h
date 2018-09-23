/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * XPLC - Cross-Platform Lightweight Components
 * Copyright (C) 2002, Pierre Phaneuf
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

#ifndef __XPLC_MODULE_H__
#define __XPLC_MODULE_H__

#if defined(__GNUC__) && __GNUC__ > 3
# pragma GCC system_header
#endif

/** \file
 *
 * Structures and definitions related to the XPLC module ABI
 * (Application Binary Interface).
 */

#include <xplc/IObject.h>

#ifdef UNSTABLE
#include <limits.h>
#endif

/**
 * XPLC module magic number. Used to ensure that we are dealing with a
 * valid XPLC module.
 */
#define XPLC_MODULE_MAGIC 0x58504c43UL

/**
 * The current XPLC module ABI version.
 */
//@{
#ifdef UNSTABLE
#define XPLC_MODULE_VERSION_MAJOR UINT_MAX
#define XPLC_MODULE_VERSION_MINOR 0
#else
#define XPLC_MODULE_VERSION_MAJOR 0
#define XPLC_MODULE_VERSION_MINOR 0
#endif
//@}

/**
 * Defines attributes required for exported symbols.
 */
#ifdef WIN32
#define ENTRYPOINT extern "C" __declspec(dllexport)
#else
#define ENTRYPOINT extern "C"
#endif

/**
 * Entry for a component. Modules have an array of these, where the
 * function pointed at by getObject will be used to obtain an
 * interface pointer when the requested UUID matches the one in uuid.
 */
struct XPLC_ComponentEntry {
  //@{
  const UUID& uuid;
  IObject* (*getObject)();
  //@}
};

/**
 * Entry for a category registration. Modules have an array of these,
 * used to indicate the category information for the module.
 */
struct XPLC_CategoryEntry {
  //@{
  const UUID& category;
  const UUID& uuid;
  const char* const string;
  //@}
};

/**
 * Information for an XPLC module.
 */
struct XPLC_ModuleInfo {
  /**
   * XPLC module magic number. This is to ensure that it is in fact a
   * valid XPLC module that has been loaded.
   */
  unsigned long magic;
  /**
   * The XPLC module ABI version that this module conforms to. This
   * should always be the first member of the XPLC_ModuleInfo
   * structure, as the meaning of the following members depend on it.
   */
  unsigned int version_major;
  /**
   * The XPLC module ABI sub-version that this module conforms
   * to. This is used for optional and backward-compatible changes in
   * the module ABI.
   */
  unsigned int version_minor;

  /**
   * Description string for the module.
   */
  const char* description;

  /**
   * List of components supported by the module. This is a pointer to
   * the list of components that will be made available by this
   * module.
   */
  const XPLC_ComponentEntry* const components;

  /**
   * List of category registrations for the module.
   */
  const XPLC_CategoryEntry* const categories;
};

/**
 * Definition of the XPLC module information structure. This structure
 * should be initialized appropriately in a loadable XPLC module.
 */
ENTRYPOINT const XPLC_ModuleInfo XPLC_Module;

#endif /* __XPLC_MODULE_H__ */
