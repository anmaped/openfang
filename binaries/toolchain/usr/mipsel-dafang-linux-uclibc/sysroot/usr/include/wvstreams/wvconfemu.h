/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2004 Net Integration Technologies, Inc.
 *
 * Basic WvConf emulation layer for UniConf.
 */
#ifndef __WVCONFEMU_H
#define __WVCONFEMU_H

#include "uniconfroot.h"
#include "wvstringtable.h"
#include "wvsorter.h"

#define WvConf WvConfEmu
#define WvConfigSection WvConfigSectionEmu
#define WvConfigSectionList WvConfigSectionListEmu
#define WvConfigEntry WvConfigEntryEmu
#define WvConfigEntryList WvConfigEntryListEmu


class WvConfEmu;
class WvConfigEntryEmu;
class WvConfigSectionEmu;
class WvAuthDaemon;
class WvAuthDaemonSvc;

typedef WvConfEmu WvConfigSectionListEmu;
typedef WvConfigSectionEmu WvConfigEntryListEmu;


class WvConfigEntryEmu
{
public:
    const WvString name;
    WvString value;
    WvConfigEntryEmu(WvStringParm _name, WvStringParm _value):
	name(_name), value(_value)
    {}
};


DeclareWvDict(WvConfigEntryEmu, WvString, name);


class WvConfigSectionEmu
{
private:
    const UniConf uniconf;
    WvConfigEntryEmuDict entries;
    WvStringTable &values;
public:
    const WvString name;
    WvConfigSectionEmu(const UniConf& _uniconf, WvStringParm _name,
		       WvStringTable *_values):
	uniconf(_uniconf), entries(42), values(*_values), name(_name)
    {}
    WvConfigEntryEmu *operator[] (WvStringParm s);
    const char *get(WvStringParm entry, const char *def_val = NULL);
    void set(WvStringParm entry, WvStringParm value);
    void quick_set(WvStringParm entry, WvStringParm value);

    bool isempty() const;

    class Iter;
    friend class Iter;

    typedef WvSorter<WvConfigEntryEmu, WvConfigSectionEmu, Iter> Sorter;
};


DeclareWvDict(WvConfigSectionEmu, WvString, name);


class WvConfigSectionEmu::Iter
{
private:
    WvConfigSectionEmu& sect;
    UniConf::RecursiveIter iter;
    WvLink link;
    WvConfigEntryEmu* entry;
public:
    Iter(WvConfigSectionEmu& _sect):
	sect(_sect), iter(_sect.uniconf), link(NULL, false), entry(NULL)
    { 
        assert(&_sect);
    }
    ~Iter();
    void rewind();
    WvLink *next();
    WvLink *cur();
    WvConfigEntryEmu* ptr() const;
    void* vptr() const;
    WvIterStuff(WvConfigEntryEmu);
};


// parameters are: userdata, section, entry, oldval, newval
typedef wv::function<void(void*, WvStringParm, WvStringParm, WvStringParm, WvStringParm)> WvConfCallback;


class WvConfEmu
{
private:
    struct CallbackInfo
    {
	WvConfCallback callback;
	void* userdata;
	WvString section;
	WvString key;
	void* cookie;
	CallbackInfo(WvConfCallback _callback, void* _userdata,
		     WvStringParm _section, WvStringParm _key,
		     void* _cookie):
	    callback(_callback), userdata(_userdata), section(_section),
	    key(_key), cookie(_cookie)
	{}
    };

    WvConfigSectionEmuDict sections;
    bool hold;
    bool dirty;
    WvList<CallbackInfo> callbacks;
    WvStringTable values;

    void notify(const UniConf &_uni, const UniConfKey &_key);
public:
    const UniConf uniconf;

    WvConfEmu(const UniConf &_uniconf);
    ~WvConfEmu();
    void zap();
    bool isclean() const;
    bool isok() const;
    void load_file(WvStringParm filename);
    void save(WvStringParm filename, int _create_mode = 0666);
    void save();
    void flush();

    WvConfigSectionEmu *operator[] (WvStringParm sect);

    void add_callback(WvConfCallback callback, void *userdata,
		      WvStringParm section, WvStringParm key, void *cookie);
    void del_callback(WvStringParm section, WvStringParm key, void *cookie);

    void add_setbool(bool *b, WvStringParm _section, WvStringParm _key);
    void del_setbool(bool *b, WvStringParm _section, WvStringParm _key);

    // The addname callback will add the key "ent" in "sect" to the "list" 
    // whenever "ent" changes. If ent is empty, add any key in the sect to
    // the list when one is added/deleted/changed
    void add_addname(WvStringList *list, WvStringParm sect, WvStringParm ent);
    void del_addname(WvStringList *list, WvStringParm sect, WvStringParm ent);

    WvString getraw(WvString wvconfstr, int &parse_error);
    int getint(WvStringParm section, WvStringParm entry, int def_val);
    const char *get(WvStringParm section, WvStringParm entry,
		    const char *def_val = NULL);
    int fuzzy_getint(WvStringList &sect, WvStringParm entry,
		  int def_val);
    const char *fuzzy_get(WvStringList &sect, WvStringParm entry,
			  const char *def_val = NULL);

    void setraw(WvString wvconfstr, const char *&value, int &parse_error);
    void setint(WvStringParm section, WvStringParm entry, int value);
    void set(WvStringParm section, WvStringParm entry,
	     const char *value);

    void maybesetint(WvStringParm section, WvStringParm entry,
		     int value);
    void maybeset(WvStringParm section, WvStringParm entry,
		  const char *value);

    void delete_section(WvStringParm section);

    // Gets a user's password and decrypts it.  This isn't defined in wvconf.cc.
    WvString get_passwd(WvStringParm sect, WvStringParm user);
    WvString get_passwd(WvStringParm user)
        { return get_passwd("Users", user); }
    WvString get_passwd2(WvString pwenc);

    // Check the password passed in.  This isn't defined in wvconf.cc
    // We use this function to check passwords since we may not know what
    // the password actually is!
    // If s is not null and has continue_select enabled, check_passwd will
    // pause if the password is incorrect (the pause length depends on how
    // many password failures have occurred recently).
    bool check_passwd(WvStringParm sect, WvStringParm user,
		      WvStringParm passwd, WvStream *s);
    bool check_passwd(WvStringParm user, WvStringParm passwd, WvStream *s)
    {
	return check_passwd("Users", user, passwd, s);
    }

    // Check if the user exists.  This isn't defined in wvconf.cc
    bool user_exists(WvStringParm sect, WvStringParm user);
    bool user_exists(WvStringParm user)
    {
	return user_exists("Users", user);
    }

    // Encrypts and sets a user's password.  This isn't defined in wvconf.cc.
    void set_passwd(WvStringParm sect, WvStringParm user, WvStringParm passwd);
    void set_passwd(WvStringParm user, WvStringParm passwd)
        { set_passwd("Users", user, passwd); }
    WvString set_passwd2(WvStringParm passwd);

    // Converts all passwords to unencrypted format.  Not defined in wvconf.cc.
    void convert_to_old_pw();

    static int check_for_bool_string(const char *s);

    class Iter;
    friend class Iter;
    
private:
/* The following is an ugly hack, but since WvConf is being
 * deprecated, we don't care.
 * 
 * It seems that check_passwd() and user_exists() need to talk to a
 * WvAuthDaemon.  However, making them virtual functions would break since
 * everyone else has to implement them.  So we'll its pointer and accessors
 * here.
 */
private:
    WvAuthDaemon *wvauthd;	// Authentication Daemon
public:
    friend class WvAuthDaemonSvc;
};


class WvConfEmu::Iter
{
    WvConfEmu& conf;
    UniConf::Iter iter;
    WvLink link;
public:
    Iter(WvConfEmu& _conf):
	conf(_conf), iter(conf.uniconf), link(NULL, false)
    {}
    void rewind();
    WvLink *next();
    WvConfigSectionEmu* ptr() const;
    WvIterStuff(WvConfigSectionEmu);
};

#endif // __WVCONFEMU_H
