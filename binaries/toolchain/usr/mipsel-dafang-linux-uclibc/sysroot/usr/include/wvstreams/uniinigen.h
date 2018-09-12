/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * A generator for .ini files.
 */
#ifndef __UNICONFINI_H
#define __UNICONFINI_H

#include "unitempgen.h"
#include "wvlog.h"
#include <sys/stat.h>

class WvFile;

/**
 * Loads and saves ".ini"-style files similar to those used by
 * Windows, but adapted to represent keys and values using Tcl
 * style lists.
 * 
 * To mount, use the moniker prefix "ini:" followed by the
 * path of the .ini file.
 * 
 */
class UniIniGen : public UniTempGen
{
public:
    typedef wv::function<void()> SaveCallback;

private:
    WvString filename;
    int create_mode;
    WvLog log;
    struct stat old_st;
    SaveCallback save_cb;
    
public:
    /**
     * Creates a generator which can load/modify/save a .ini file.
     * "filename" is the local path of the .ini file
     */
    UniIniGen(WvStringParm filename, int _create_mode = 0666,
            SaveCallback _save_cb = SaveCallback());

    virtual ~UniIniGen();
    
    /***** Overridden members *****/

    virtual void commit();
    virtual bool refresh();
    virtual void set(const UniConfKey &key, WvStringParm value);

private:
#ifndef _WIN32
    // helper methods for commit
    bool commit_atomic(WvStringParm real_filename);
#endif
    
    void save(WvStream &file, UniConfValueTree &parent);
    bool refreshcomparator(const UniConfValueTree *a,
			   const UniConfValueTree *b);
};


#endif // __UNICONFINI_H
