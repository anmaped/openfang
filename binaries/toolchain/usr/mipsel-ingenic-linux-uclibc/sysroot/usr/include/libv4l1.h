/*
#             (C) 2008 Hans de Goede <hdegoede@redhat.com>

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335  USA
*/

#ifndef __LIBV4L1_H
#define __LIBV4L1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <libv4l1-videodev.h>

#if HAVE_VISIBILITY
#define LIBV4L_PUBLIC __attribute__ ((visibility("default")))
#else
#define LIBV4L_PUBLIC
#endif

/* Point this to a FILE opened for writing when you want to log error and
   status messages to a file, when NULL errors will get send to stderr */
LIBV4L_PUBLIC extern FILE *v4l1_log_file;

/* Just like your regular open/close/etc, except that when opening a v4l2
   capture only device, full v4l1 emulation is done including emulating the
   often not implemented in v4l2 drivers CGMBUF ioctl and v4l1 style mmap call
   in userspace.

   Format conversion is done if necessary when capturing. That is if you
   (try to) set a capture format which is not supported by the cam, but is
   supported by libv4lconvert then SPICT will succeed and on SYNC / read the
   data will be converted for you and returned in the request format.

   Note that currently libv4l1 depends on the kernel v4l1 compatibility layer
   for: 1) Devices which are not capture only, 2) Emulation of many basic
   v4l1 ioctl's which require no driver specific handling.

   Note that no functionality is added to v4l1 devices, so if for example an
   obscure v4l1 device is opened which only supports some weird capture format
   then libv4l1 will not be of any help (in this case it would be best to get
   the driver converted to v4l2, as v4l2 has been designed to include weird
   capture formats, like hw specific bayer compression methods).
*/

LIBV4L_PUBLIC int v4l1_open(const char *file, int oflag, ...);
LIBV4L_PUBLIC int v4l1_close(int fd);
LIBV4L_PUBLIC int v4l1_dup(int fd);
LIBV4L_PUBLIC int v4l1_ioctl(int fd, unsigned long int request, ...);
LIBV4L_PUBLIC ssize_t v4l1_read(int fd, void *buffer, size_t n);
LIBV4L_PUBLIC void *v4l1_mmap(void *start, size_t length, int prot, int flags,
		int fd, int64_t offset);
LIBV4L_PUBLIC int v4l1_munmap(void *_start, size_t length);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
