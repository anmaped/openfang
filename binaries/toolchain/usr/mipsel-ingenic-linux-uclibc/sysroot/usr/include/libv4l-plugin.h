/*
* Copyright (C) 2010 Nokia Corporation <multimedia@maemo.org>

* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __LIBV4L_PLUGIN_H
#define __LIBV4L_PLUGIN_H

#include <sys/types.h>

/* Structure libv4l_dev_ops holds the calls from libv4ls to video nodes.
   They can be normal open/close/ioctl etc. or any of them may be replaced
   with a callback by a loaded plugin.
*/

struct libv4l_dev_ops {
    void * (*init)(int fd);
    void (*close)(void *dev_ops_priv);
    int (*ioctl)(void *dev_ops_priv, int fd, unsigned long int request, void *arg);
    ssize_t (*read)(void *dev_ops_priv, int fd, void *buffer, size_t n);
    ssize_t (*write)(void *dev_ops_priv, int fd, const void *buffer, size_t n);
    /* For future plugin API extension, plugins implementing the current API
       must set these all to NULL, as future versions may check for these */
    void (*reserved1)(void);
    void (*reserved2)(void);
    void (*reserved3)(void);
    void (*reserved4)(void);
    void (*reserved5)(void);
    void (*reserved6)(void);
    void (*reserved7)(void);
};

#endif
