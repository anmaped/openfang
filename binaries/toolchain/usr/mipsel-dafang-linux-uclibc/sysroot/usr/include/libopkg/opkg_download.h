/* opkg_download.h - the opkg package management system

   Carl D. Worth

   Copyright (C) 2001 University of Southern California

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
*/

#ifndef OPKG_DOWNLOAD_H
#define OPKG_DOWNLOAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pkg.h"

typedef void (*opkg_download_progress_callback)(int percent, char *url);
typedef int (*curl_progress_func)(void *data, double t, double d, double ultotal, double ulnow);

int opkg_download(const char *src, const char *dest_file_name, curl_progress_func cb, void *data, const short hide_error);
int opkg_download_pkg(pkg_t *pkg, const char *dir);
/*
 * Downloads file from url, installs in package database, return package name.
 */
int opkg_prepare_url_for_install(const char *url, char **namep);

int opkg_verify_file (char *text_file, char *sig_file);

/* Curl cleanup function, does nothing unless opkg is configured with
 * '--enable-curl'.
 *
 * This function is callable regardless of whether curl support is enabled in
 * order to present a consistent API.
 */
void opkg_curl_cleanup(void);

#ifdef __cplusplus
}
#endif
#endif /* OPKG_DOWNLOAD_H */
