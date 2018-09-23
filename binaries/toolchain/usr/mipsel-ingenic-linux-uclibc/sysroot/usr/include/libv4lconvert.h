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

#ifndef __LIBV4LCONVERT_H
#define __LIBV4LCONVERT_H

/* These headers are not needed by us, but by linux/videodev2.h,
   which is broken on some systems and doesn't include them itself :( */

#ifdef linux
#include <sys/time.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#endif

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#endif

/* end broken header workaround includes */

#include <linux/videodev2.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if HAVE_VISIBILITY
#define LIBV4L_PUBLIC __attribute__ ((visibility("default")))
#else
#define LIBV4L_PUBLIC
#endif

struct libv4l_dev_ops;
struct v4lconvert_data;

LIBV4L_PUBLIC const struct libv4l_dev_ops *v4lconvert_get_default_dev_ops();

LIBV4L_PUBLIC struct v4lconvert_data *v4lconvert_create(int fd);
LIBV4L_PUBLIC struct v4lconvert_data *v4lconvert_create_with_dev_ops(int fd,
		void *dev_ops_priv, const struct libv4l_dev_ops *dev_ops);
LIBV4L_PUBLIC void v4lconvert_destroy(struct v4lconvert_data *data);

/* When doing flipping / rotating / video-processing, only supported
   destination formats can be used (as flipping / rotating / video-processing
   is not supported on other formats). This function can be used to query
   if that is the case. */
LIBV4L_PUBLIC int v4lconvert_supported_dst_fmt_only(
		struct v4lconvert_data *data);

/* With regards to dest_fmt just like VIDIOC_TRY_FMT, except that the try
   format will succeed and return the requested V4L2_PIX_FMT_foo in dest_fmt if
   the cam has a format from which v4lconvert can convert to dest_fmt.
   The real format to which the cam should be set is returned through src_fmt
   when not NULL.
   Note that just like the real VIDIOC_TRY_FMT this function will change the
   dest_fmt when not supported. This includes changing it to a supported
   destination format when trying a native format of the camera and
   v4lconvert_supported_dst_fmt_only() returns true. */
LIBV4L_PUBLIC int v4lconvert_try_format(struct v4lconvert_data *data,
		struct v4l2_format *dest_fmt, /* in / out */
		struct v4l2_format *src_fmt); /* out */

/* Like VIDIOC_ENUM_FMT, but the emulated formats are added at the end of the
   list, except if flipping / processing is active for the device, then only
   supported destination formats are listed */
LIBV4L_PUBLIC int v4lconvert_enum_fmt(struct v4lconvert_data *data,
		struct v4l2_fmtdesc *fmt);

/* Is conversion necessary or can the app use the data directly? */
LIBV4L_PUBLIC int v4lconvert_needs_conversion(struct v4lconvert_data *data,
		const struct v4l2_format *src_fmt,   /* in */
		const struct v4l2_format *dest_fmt); /* in */

/* This function does the following conversions:
    - format conversion
    - cropping
   if enabled:
    - processing (auto whitebalance, auto gain, gamma correction)
    - horizontal/vertical flipping
    - 90 degree (clockwise) rotation

   NOTE: the last 3 steps are enabled/disabled depending on
    - the internal device list
    - the state of the (software emulated) image controls

   Therefore this function should
    - not be used when getting the frames from libv4l
    - be called only once per frame
   Otherwise this may result in unintended double conversions !

   Returns the amount of bytes written to dest and -1 on error */
LIBV4L_PUBLIC int v4lconvert_convert(struct v4lconvert_data *data,
		const struct v4l2_format *src_fmt,  /* in */
		const struct v4l2_format *dest_fmt, /* in */
		unsigned char *src, int src_size, unsigned char *dest, int dest_size);

/* get a string describing the last error */
LIBV4L_PUBLIC const char *v4lconvert_get_error_message(struct v4lconvert_data *data);

/* Just like VIDIOC_ENUM_FRAMESIZE, except that the framesizes of emulated
   formats can be enumerated as well. */
LIBV4L_PUBLIC int v4lconvert_enum_framesizes(struct v4lconvert_data *data,
		struct v4l2_frmsizeenum *frmsize);

/* Just like VIDIOC_ENUM_FRAMEINTERVALS, except that the intervals of emulated
   formats can be enumerated as well. */
LIBV4L_PUBLIC int v4lconvert_enum_frameintervals(struct v4lconvert_data *data,
		struct v4l2_frmivalenum *frmival);

/* Pass calls to query, get and set video controls to the libv4lcontrol class */
LIBV4L_PUBLIC int v4lconvert_vidioc_queryctrl(struct v4lconvert_data *data,
		void *arg);
LIBV4L_PUBLIC int v4lconvert_vidioc_g_ctrl(struct v4lconvert_data *data,
		void *arg);
LIBV4L_PUBLIC int v4lconvert_vidioc_s_ctrl(struct v4lconvert_data *data,
		void *arg);

/* Is the passed in pixelformat supported as destination format? */
LIBV4L_PUBLIC int v4lconvert_supported_dst_format(unsigned int pixelformat);

/* Get/set the no fps libv4lconvert uses to decide if a compressed format
   must be used as src fmt to stay within the bus bandwidth */
LIBV4L_PUBLIC int v4lconvert_get_fps(struct v4lconvert_data *data);
LIBV4L_PUBLIC void v4lconvert_set_fps(struct v4lconvert_data *data, int fps);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
