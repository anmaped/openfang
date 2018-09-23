/* -*- Mode: C++ -*-
 * Worldvisions Tunnel Vision Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * WvPushDir -- A simple class to check the existance of a dir 
 *  and to properly return the formatted path of the diir
 */
#ifndef __WVPUSHDIR_H
#define __WVPUSHDIR_H

#include "wverror.h"

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <unistd.h>

class WvPushDir : public WvError
{
    DIR *dir_handle;
    char *old_dir;

public:
    void* operator new(size_t) 
        { abort(); }

    WvPushDir(WvStringParm new_dir)
    {
#ifdef MACOS
       old_dir = static_cast<char *>(calloc(PATH_MAX, sizeof(char *)));
       getcwd(old_dir, PATH_MAX);;
#else
       old_dir = get_current_dir_name();
#endif
       dir_handle = opendir(old_dir);
       if (chdir(new_dir) == -1)
          errnum = errno;
    }

    ~WvPushDir()
    { 
        chdir(old_dir); 
        closedir(dir_handle);
        free(old_dir);
    }
};

#endif /// __WVPUSHDIR_H
