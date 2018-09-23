/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2004-2006 Net Integration Technologies, Inc.
 * 
 * Pathfinder Software:
 *   Copyright (C) 2007, Carillon Information Security Inc.
 *
 * This library is licensed under the LGPL, please read LICENSE for details.
 *
 * WvDBusMsg is intended to be an easy-to-use abstraction over the low-level 
 * D-Bus DBusMessage structure. It represents a message being passed around on 
 * the bus.
 */ 
#ifndef __WVDBUSMSG_H
#define __WVDBUSMSG_H

#include "wvstringlist.h"
#include "wvbuf.h"
#include <stdint.h>

struct DBusMessageIter;
struct DBusMessage;

class WvDBusMsg;
class WvDBusConn;


class WvDBusMsg
{
public:
    /**
     * Constructs a new WvDBus message. If destination is blank, no 
     * destination is set; this is appropriate when using D-BUS in a 
     * peer-to-peer context (no message bus).
     */
    WvDBusMsg(WvStringParm busname, WvStringParm objectname, 
              WvStringParm ifc, WvStringParm method);

    /**
     * Constructs a new WvDBus message, copying it out of an old one.
     */
    WvDBusMsg(WvDBusMsg &_msg);

    /**
     * Constructs a new WvDBus message from an existing low-level D-Bus 
     * message.
     */
    WvDBusMsg(DBusMessage *_msg);

    virtual ~WvDBusMsg();

    operator DBusMessage* () const;
    
    /**
     * Demarshals a new WvDBusMsg from a buffer containing its binary DBus
     * protocol representation.  You're responsible for freeing the object
     * when done.  Returns NULL if the object can't be extracted from the
     * buffer.
     * (Implementation in wvdbusmarshal.cc)
     */
    static WvDBusMsg *demarshal(WvBuf &buf);
    
    /**
     * Given a buffer containing what might be the header of a DBus message,
     * checks how many bytes need to be in the buffer in order for it to
     * contain a whole message.  If the return value is <= the number of
     * bytes already in the buffer, then demarshal() will succeed (or the
     * incoming message is corrupt).
     * (Implementation in wvdbusmarshal.cc)
     */
    static size_t demarshal_bytes_needed(WvBuf &buf);
    
    /**
     * Locks this message, encodes it in DBus binary protocol format, and
     * adds it to the given buffer.  This message becomes locked and can
     * no longer be append()ed to.  You can marshal it more than once,
     * however (but it will always have the same serial number!!)
     * (Implementation in wvdbusmarshal.cc)
     */
    void marshal(WvBuf &buf);
    
    WvString get_sender() const;
    WvString get_dest() const;
    WvString get_path() const;
    WvString get_interface() const;
    WvString get_member() const;
    WvString get_error() const;
    uint32_t get_serial() const;
    uint32_t get_replyserial() const;
    bool is_reply() const;
    operator WvString() const;
    
    void get_arglist(WvStringList &list) const;
    WvString get_argstr() const;

    /**
     * The following methods are designed to allow appending various
     * arguments to the message.
     */
    WvDBusMsg &append(const char *s);
    WvDBusMsg &append(bool b);
    WvDBusMsg &append(signed char c);
    WvDBusMsg &append(unsigned char c);
    WvDBusMsg &append(int16_t i);
    WvDBusMsg &append(uint16_t i);
    WvDBusMsg &append(int32_t i);
    WvDBusMsg &append(uint32_t i);
    WvDBusMsg &append(int64_t i);
    WvDBusMsg &append(uint64_t i);
    WvDBusMsg &append(double d);
    
    /**
     * Start a variant.
     */
    WvDBusMsg &variant_start(WvStringParm element_type);
    
    /**
     * End a variant.
     */
    WvDBusMsg &variant_end();
    
    /**
     * Start a struct.  Elements append()ed after this will be inside the
     * struct, and you should be careful that you append the right types in
     * the right order.  Finish using struct_end().
     */
    WvDBusMsg &struct_start(WvStringParm element_type);
    
    /**
     * End a struct started with struct_start().
     */
    WvDBusMsg &struct_end();

    /**
     * Start an array.  Elements append()ed after this will be inside the
     * array.  Finish using array_end().
     */
    WvDBusMsg &array_start(WvStringParm element_type);
    
    /**
     * End an array started with array_start().
     */
    WvDBusMsg &array_end();

    /**
     * Start a variant-array.  Elements append()ed after this will be inside
     * the array.  Finish using varray_end().
     * 
     * A variant-array is like an array, but is enclosed automatically inside
     * a variant object.  This is useful for arrays of arrays, where each
     * inner array is of a different type.
     */
    WvDBusMsg &varray_start(WvStringParm element_type);
    
    /**
     * End an array started with array_start().
     */
    WvDBusMsg &varray_end();

    /**
     * Generate a message that will be a reply to this one.
     */
    WvDBusMsg reply();
    
    /**
     * Return true if this message is an error response.
     */
    bool iserror() const;
    
    /**
     * A shortcut for sending this message on the given connection.
     * Equivalent to conn.send(*this).
     */
    void send(WvDBusConn &conn);
    
    class Iter
    {
    public:
	DBusMessageIter *const first, *const it;
	mutable WvString s;
	bool rewound;
	
	Iter(const WvDBusMsg &_msg);
	Iter(const WvDBusMsg::Iter &_it);
	Iter(const DBusMessageIter &_first);
	~Iter();

        /**
         * Rewinds the iterator to make it point to an imaginary element
         * preceeding the first element of the list.
         */
	void rewind();
	
	/**
	 * Returns the data type of the current element.  Not usually needed,
	 * as the iterator converts elements automatically between most types.
	 */
	int type() const;
	
	/**
	 * Returns a sub-iterator for walking through recursive types, such
	 * as arrays, structs, and variants.
	 * 
	 * You don't strictly need to call this for variants; get_str() and
	 * friends will do the right thing.
	 */
	Iter open() const;
	
        /**
         * Moves the iterator along the list to point to the next element.
         * 
         * If the iterator had just been rewound, it now points to the
         * first element of the list.
         */
	bool next();
	
	/**
	 * Same as next(), but returns *this instead so you can convert the
	 * new item to the right value type.  Note: this doesn't do error
	 * checking to see if the parameter exists.
	 */
	Iter &getnext()
	    { next(); return *this; }

        /**
	 * Returns: true if the current link is valid
         */
	bool cur() const;
	
	/**
	 * Fill a WvStringList with a string for each element of the iter.
	 */
	void get_all(WvStringList &list);
	
	/**
	 * Return a WvString representation of all elements in a single
	 * string.
	 */
	WvString get_all();
	
	/**
	 * Get the current element as a string (possible for all types).
	 */
	WvString get_str() const;
	
	/**
	 * Get the current element as an int64_t
	 * (possible for all integer types)
	 */
	int64_t get_int() const;
	operator int64_t() const { return get_int(); }
	operator int32_t() const { return get_int(); }
	operator int16_t() const { return get_int(); }
	operator int8_t() const { return get_int(); }
	operator bool() const { return get_int() != 0; }
	
	/**
	 * Get the current element as a uint64_t
	 * (possible for all integer types)
	 */
	uint64_t get_uint() const;
	operator uint64_t() const { return get_uint(); }
	operator uint32_t() const { return get_uint(); }
	operator uint16_t() const { return get_uint(); }
	operator uint8_t() const { return get_uint(); }

        /**
         * Get the current element as a double
         * (possible for all integer and floating point types)
         */
        double get_double() const;
        operator double() const { return get_double(); }
        operator float() const { return get_double(); }
	
	/**
	 * Returns a pointer to the WvString at the iterator's current
	 * location.  Needed so that WvIterStuff() will work.
	 */
	WvString *ptr() const;
	operator WvString() const { return *ptr(); }
 	
	WvIterStuff(WvString);
    };

protected:
    mutable DBusMessage *msg;
    WvList<DBusMessageIter> itlist;
};


class WvDBusSignal : public WvDBusMsg
{
public:
    WvDBusSignal(WvStringParm objectname, WvStringParm ifc, 
                 WvStringParm name);
};


class WvDBusError : public WvDBusMsg
{
    DBusMessage *setup1(WvDBusMsg &in_reply_to,
		       WvStringParm errname, WvStringParm message);
    void setup2();
public:
    WvDBusError(WvDBusMsg &in_reply_to,
		WvStringParm errname, WvStringParm message)
	: WvDBusMsg(setup1(in_reply_to, errname, message))
    {
	setup2();
    }
    
    WvDBusError(WvDBusMsg &in_reply_to,
		WvStringParm errname, WVSTRING_FORMAT_DECL)
	: WvDBusMsg(setup1(in_reply_to, errname,
			   WvString(WVSTRING_FORMAT_CALL)))
    {
	setup2();
    }
};

#endif // __WVDBUSMSG_H
