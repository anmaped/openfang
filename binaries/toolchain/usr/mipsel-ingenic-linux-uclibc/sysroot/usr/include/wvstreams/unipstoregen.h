/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2003 Net Integration Technologies, Inc.
 *
 * A generator that exposes Windows protected storage.
 *
 * When linking statically, use the following #pragma to ensure this 
 * generator gets registered:
 * #pragma comment(linker, "/include:?UniPStoreGenMoniker@@3V?$WvMoniker@VUniConfGen@@@@A")
 */

#ifndef __UNICONFPSTORE_H
#define __UNICONFPSTORE_H

#include "uniconfgen.h"
#include "wvlog.h"
#include "windows.h"

#include "pstorec.tlh"

#define PST_KEY_CURRENT_USER 0x00000000 // Specifies that the storage is maintained in the current user section of the registry.
#define PST_KEY_LOCAL_MACHINE 0x00000001 // Specifies that the storage is maintained in the local machine section of the registry.

#define PST_E_OK 0x00000000L // The operation was successful.
#define PST_E_TYPE_EXISTS 0x800C0004L // The data item already exists in the protected storage.
#define PST_E_UNKNOWN_TYPE 0x800C0005L 
#define PST_E_NOT_FOUND 0x800C0010L 

#define PST_PF_ALWAYS_SHOW 0x00000001 	// Requests that the provider show the prompt dialog to the user even if not required for this access.
#define PST_PF_NEVER_SHOW 0x00000002 	// Do not show the prompt dialog to the user.

#define PST_CF_DEFAULT 0x00000000 	// Allows user to choose confirmation style.
#define PST_CF_NONE 0x00000001 	// Forces silent item creation.

/**
 * A generator that exposes Windows protected storage.
 *
 * To mount, use the moniker 
 * "pstore:PST_KEY_CURRENT_USER:TYPENAME:TYPEGUID:SUBTYPE:SUBTYPEGUID"
 *
 */
class UniPStoreGen : public UniConfGen
{
private:
    WvLog m_log;
    HMODULE m_hPstoreDLL;
    PSTORECLib::IPStorePtr m_spPStore; 
    GUID m_type, m_subtype;
    DWORD m_key;

    HRESULT create_types(WvString type_name, WvString subtype_name);

protected:
    virtual void flush_buffers() { }

public:
    UniPStoreGen(WvString _base);
    virtual ~UniPStoreGen();

    /***** Overridden methods *****/
    
    virtual bool isok();
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual bool exists(const UniConfKey &key);
    virtual bool haschildren(const UniConfKey &key);
    virtual Iter *iterator(const UniConfKey &key);
};


#endif // __UNICONFPSTORE_H
