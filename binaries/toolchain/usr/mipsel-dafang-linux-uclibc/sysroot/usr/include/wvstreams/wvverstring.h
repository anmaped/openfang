/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2004 Net Integration Technologies, Inc.
 *
 * Version number and string manipulations.
 *
 * The old version number was a 32-bit hexadecimal number, split into a 16-bit
 * major version and a 16-bit minor version.  For example, the old-style string
 * equivalent of 0x00012a00 would be "1.2a".
 *
 * The new-style version number is a 32-bit hexadecimal number, split into
 * an 8-bit major version, an 8-bit minor version, and a 16-bit revision
 * number.  The new-style string equivalent of 0x01020150 would be "1.02.0150".
 */
#ifndef __WVVERSTRING_H
#define __WVVERSTRING_H

/** Converts a version number to a C string.
 * This uses is_new_ver() to determine which style to use for the resulting
 * string.
 */
const char *ver_to_string(unsigned int ver);

/// Converts a version number to a C string using the old style.
const char *old_ver_to_string(unsigned int ver);

/// Converts a version number to a C string using the new sytle.
const char *new_ver_to_string(unsigned int ver);

/** Converts a C string to a version number.
 * This uses is_new_verstr() to determine which style to use for the resulting
 * version integer.
 */
unsigned int string_to_ver(const char *str);

/// Converts a C string to a version number using the old style.
unsigned int string_to_old_ver(const char *str);

/// Converts a C string to a version number using the new style.
unsigned int string_to_new_ver(const char *str);

/** Returns true if 'ver' is in the new style.
 * If the first two hex digits are non-zero, it is the old style; otherwise,
 * it's the new style.  This somewhat arbitrary distinction was made to ease
 * migration of some WvStreams projects.  Use the specific new and old
 * functions if you don't like this.
 */
bool is_new_ver(unsigned int ver);

/** Returns true if 'str' is in the new style.
 * If there are 2 periods ('.') in the string, it is the new style; otherwise,
 * it's the old style.
 */
bool is_new_verstr(const char *str);

/** Trims zeroes off the end of a version string.
 * Used by old-style functions only.
 */
char *trim_verstr(char *verstr);

#endif // __WVVERSTRING_H
