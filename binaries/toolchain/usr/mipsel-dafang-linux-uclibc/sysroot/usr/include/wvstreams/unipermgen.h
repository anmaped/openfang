/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 */
#ifndef __UNIPERMGEN_H
#define __UNIPERMGEN_H

#include "unifiltergen.h"
#include "wvstringtable.h"

/**
 * UniPermGen wraps a tree encoding Unix-style permissions, and provides an
 * API for setting and checking them.  read permission allows you to read the
 * value of a key, write allows you to set it (duh).  exec allows you to get
 * subkeys.  You cannot iterate on a key unless you have exec permission.
 * (This is badly named, but it's inherited from Unix.)
 *
 * Permissions for 'key' are stored in the subkeys key/owner, key/group,
 * and key/user-read through key/world-write.  owner and group store arbitrary
 * text strings, and the remainder are boolean values.
 *
 * If you want to use monikers, UniPermGen can only be via UniSecureGen (see
 * unisecuregen.h) since it provides its own API beyond just UniConfGen.
 */
class UniPermGen : public UniFilterGen
{
public:
    UniPermGen(IUniConfGen *_gen);
    UniPermGen(WvStringParm moniker);

    enum Level { USER = 0, GROUP, WORLD };
    static WvString level2str(Level l);
    
    enum Type { READ = 0, WRITE, EXEC };
    static WvString type2str(Type t);

    struct Credentials
    {
        WvString user;
        mutable WvStringTable groups;
        // mutable because stupid WvHashTable has no const lookup methods
        
        Credentials() : groups(7) { }
    };

    /** get and set the owner for a path */
    void setowner(const UniConfKey &path, WvStringParm owner);
    WvString getowner(const UniConfKey &path);

    /** get and set the group for a path */
    void setgroup(const UniConfKey &path, WvStringParm group);
    WvString getgroup(const UniConfKey &path);

    /**
     * Return true if a user with the given credentials is allowed to
     * read/write/exec the given path.
     */
    bool getread(const UniConfKey &path, const Credentials &cred)
        { return getperm(path, cred, READ); }
    bool getwrite(const UniConfKey &path, const Credentials &cred)
        { return getperm(path, cred, WRITE); }
    bool getexec(const UniConfKey &path, const Credentials &cred)
        { return getperm(path, cred, EXEC); }

    bool getperm(const UniConfKey &path, const Credentials &cred, Type type);

    void setread(const UniConfKey &path, Level level, bool read)
        { setperm(path, level, READ, read); }
    void setwrite(const UniConfKey &path, Level level, bool write)
        { setperm(path, level, WRITE, write); }
    void setexec(const UniConfKey &path, Level level, bool exec)
        { setperm(path, level, EXEC, exec); }

    void setperm(const UniConfKey &path, Level level, Type type, bool val);

    /**
     * Set permissions for path using Unix style chmod (with the second form,
     * be sure to use octal)
     */
    void chmod(const UniConfKey &path, unsigned int owner, unsigned int group,
            unsigned int world);
    void chmod(const UniConfKey &path, unsigned int mode);

    virtual void flush_buffers() { }

private:
    bool getoneperm(const UniConfKey &path, Level level, Type type); 
};


#endif // __UNIPERMGEN_H
