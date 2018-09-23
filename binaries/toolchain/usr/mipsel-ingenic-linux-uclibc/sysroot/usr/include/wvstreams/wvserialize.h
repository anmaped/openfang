/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * Code to serialize objects into WvBufs, and more code to read WvBufs and
 * construct objects from them.
 */
#ifndef __WVSERIALIZE_H
#define __WVSERIALIZE_H

#include "wvbuf.h"
#include "wvstringlist.h"

#ifndef _WIN32
# if HAVE_INTTYPES_H
#  include <inttypes.h>
# else
#  if HAVE_STDINT_H
#   include <stdint.h>
#  endif
# endif
#include <netinet/in.h>
#else
#if _MSC_VER
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif
#include <winsock2.h>
#endif

/**
 * Encode an object as an array of bytes and put it into a WvBuf.  This
 * function just calls an overloaded _wv_serialize() function.  There was
 * really no need for a template here at all, except for symmetry with
 * wv_deserialize() which does need one.
 */
template <typename T>
inline void wv_serialize(WvBuf &buf, const T &t)
{
    _wv_serialize(buf, t);
}


/**
 * This function shouldn't be necessary at all, but using it makes totally
 * insane assembler errors go away (gcc 2.95.4, glibc 2.3.1).
 */
inline int32_t _wv_htonl(int32_t i)
{
    return htonl(i);
}
inline int16_t _wv_htons(int16_t i)
{
    return htons(i);
}

/**
 * Helper functions to convert 64 bit ints to and from host byteorder
 */
inline uint64_t ntohll(uint64_t n)
{
#ifdef WORDS_BIGENDIAN
    return n;
#else
    return (((uint64_t)ntohl(n)) << 32) | ntohl(n >> 32);
#endif
}

inline uint64_t htonll(uint64_t n)
{
#ifdef WORDS_BIGENDIAN
    return n;
#else
    return (((uint64_t)htonl(n)) << 32) | htonl(n >> 32);
#endif
}

/**
 * A helper function that serializes different types of integers.  Since
 * it's inlined, the "if" is actually executed at compile time, so don't
 * worry.
 * 
 * The clever part: it doesn't really matter what size an 'int' or a 'long'
 * is, as long as it's one of the sizes supported by this function.  If an
 * int is 32 bits, we'll use the 32-bit serializer... and so on.
 */
template <typename T>
void wv_serialize_scalar(WvBuf &buf, const T t)
{
    if (sizeof(T) == 8)
    {
        int64_t i = htonll(t);
        buf.put(&i, 8);
    }
    else if (sizeof(T) == 4)
    {
	int32_t i = _wv_htonl(t);
	buf.put(&i, 4);
    }
    else if (sizeof(T) == 2)
    {
	int32_t i = _wv_htons(t);
	buf.put(&i, 2);
    }
    else if (sizeof(T) == 1)
	buf.put(&t, 1);
    else
	assert(0);
}

inline void _wv_serialize(WvBuf &buf, long long i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, unsigned long long i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, long i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, unsigned long i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, int i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, unsigned int i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, short i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, unsigned short i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, bool i)
    { wv_serialize_scalar(buf, i); }

/** Note: char != signed char for purposes of function overloading! */
inline void _wv_serialize(WvBuf &buf, char i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, signed char i)
    { wv_serialize_scalar(buf, i); }
inline void _wv_serialize(WvBuf &buf, unsigned char i)
    { wv_serialize_scalar(buf, i); }


/**
 * Serialize a WvString. The string serializer is guaranteed to not insert
 * any nuls (character 0) into the output stream except for the
 * string-terminating one, which is always present.  This makes
 * deserialization easy.
 */
inline void _wv_serialize(WvBuf &buf, WvStringParm s)
{
    if (!s.isnull())
	buf.putstr(s);
    buf.put("", 1); // terminating nul
}

/** The template wv_serialize doesn't work for const char arrays. */
inline void wv_serialize(WvBuf &buf, const char *t)
{
    _wv_serialize(buf, t);
}


/**
 * Serialize a WvBuf.  This is handier than it sounds, because then
 * WvGdbmHash's value can be a WvBuf.
 */
inline void _wv_serialize(WvBuf &buf, const WvBuf &inbuf)
{
    wv_serialize(buf, inbuf.used());
    buf.put(const_cast<WvBuf *>(&inbuf)->peek(0, inbuf.used()), inbuf.used());
}


/**
 * Serialize a list of serializable things.
 * 
 * Oh boy - I think I'm having a bit too much fun.
 */
template <typename T>
void _wv_serialize(WvBuf &buf, const WvList<T> &list)
{
    // save the number of elements
    _wv_serialize(buf, (size_t)list.count());
    
    // save the elements
    typename WvList<T>::Iter i(list);
    for (i.rewind(); i.next(); )
	_wv_serialize(buf, *i);
}



/** Deserialize an object.  See wv_deserialize(). */
template <typename T>
    T _wv_deserialize(WvBuf &buf);


/**
 * Deserialize a complex templated object.  See wv_deserialize().
 * 
 * This class is needed because partial template specialization only works
 * on classes, not on functions.  So in order to define a generic deserializer
 * for, say, WvList<T>, we have to have a class with a member function.  Sigh.
 */
template <typename T>
class WvDeserialize
{
public:
    static T go(WvBuf &buf)
	{ return _wv_deserialize<T>(buf); }
};


/**
 * If there's a deserializer for type "T", this will make a default
 * deserializer for type "T *"; that is, it'll allocate the new object
 * dynamically and you'll have to free it after.
 * 
 * This helps when you want to assume *all* deserializers return pointers
 * that you need to delete later.
 * 
 * FIXME: this class takes precedence over *specialized* _wv_deserialize()
 * functions for pointers!  Pointer-based deserializers need to be classes
 * too until this is resolved.
 */
// note: this has to be a class because we use partial template
// specialization, which doesn't work on functions.
template <typename T>
class WvDeserialize<T *>
{
public:
    static T *go(WvBuf &buf)
        { return new T(_wv_deserialize<T>(buf)); }
};



/**
 * Deserialize an object: read bytes from a buffer, and return an object
 * constructed from that.
 * 
 * Note that there is no default deserializer.  You have to specialize this
 * template for every data type you might want to deserialize.  We do define
 * some for a few standard C types.
 * 
 * Implementation note:
 * If you define a deserializer for your own type, name it _wv_deserialize()
 * (with the underscore).  If you're unlucky, you may need to define a
 * WvDeserialize class instead.
 * 
 * Note that if you have a data structure, you probably want to
 * wv_deserialize<MyType *>(buf) instead of wv_deserialize<MyType>(buf) to
 * avoid extra copies.  You'll have to define _wv_deserialize() appropriately,
 * of course.  Pointer-based _wv_deserialize() functions allocate memory,
 * so you'll have to 'delete' the returned object yourself.
 */
template <typename T>
inline T wv_deserialize(WvBuf &buf)
{
    return WvDeserialize<T>::go(buf);
}


/**
 * These functions shouldn't be necessary at all, but using it makes totally
 * insane assembler errors go away (gcc 2.95.4, glibc 2.3.1).
 */
inline int32_t _wv_ntohl(int32_t i)
{
    return ntohl(i);
}
inline int16_t _wv_ntohs(int16_t i)
{
    return ntohs(i);
}


/**
 * A helper function that deserializes different types of integers.  Since
 * it's inlined, the "if" is actually executed at compile time, so don't
 * worry.
 */
template <typename T>
inline T wv_deserialize_scalar(WvBuf &buf)
{
    if (buf.used() < sizeof(T))
	return 0;
    
    if (sizeof(T) == 8)
        return (T) ntohll(*(int64_t *)buf.get(8));
    else if (sizeof(T) == 4)
	return (T) _wv_ntohl(*(int32_t *)buf.get(4));
    else if (sizeof(T) == 2)
	return (T) _wv_ntohs(*(int16_t *)buf.get(2));
    else if (sizeof(T) == 1)
	return (T) *(int8_t *)buf.get(1);
    else
	assert(0);
}

template <typename T>
inline T xwv_deserialize_scalar(WvBuf &buf)
{
    return 0;
}

template <>
    inline long long _wv_deserialize<long long>(WvBuf &buf)
    { return wv_deserialize_scalar<long long>(buf); }
template <> 
    inline unsigned long long _wv_deserialize<unsigned long long>(WvBuf &buf)
    { return wv_deserialize_scalar<unsigned long long>(buf); }
template <>
    inline long _wv_deserialize<long>(WvBuf &buf)
    { return wv_deserialize_scalar<long>(buf); }
template <> 
    inline unsigned long _wv_deserialize<unsigned long>(WvBuf &buf)
    { return wv_deserialize_scalar<unsigned long>(buf); }
template <>
    inline int _wv_deserialize<int>(WvBuf &buf)
    { return wv_deserialize_scalar<int>(buf); }
template <> 
    inline unsigned int _wv_deserialize<unsigned int>(WvBuf &buf)
    { return wv_deserialize_scalar<unsigned int>(buf); }
template <>
    inline short _wv_deserialize<short>(WvBuf &buf)
    { return wv_deserialize_scalar<short>(buf); }
template <> 
    inline unsigned short _wv_deserialize<unsigned short>(WvBuf &buf)
    { return wv_deserialize_scalar<unsigned short>(buf); }
template <> 
    inline bool _wv_deserialize<bool>(WvBuf &buf)
    { return wv_deserialize_scalar<bool>(buf); }
template <>
    inline char _wv_deserialize<char>(WvBuf &buf)
    { return wv_deserialize_scalar<char>(buf); }
template <> 
    inline signed char _wv_deserialize<signed char>(WvBuf &buf)
    { return wv_deserialize_scalar<signed char>(buf); }
template <> 
    inline unsigned char _wv_deserialize<unsigned char>(WvBuf &buf)
    { return wv_deserialize_scalar<unsigned char>(buf); }

/**
 * Deserialize a WvString.  Stops at (and includes) the terminating nul
 * (zero) character.  Serialized WvStrings are guaranteed not to contain nul
 * except as the last character.
 */
template <>
WvString _wv_deserialize<WvString>(WvBuf &buf);


/** Deserialize a WvBuf. */
// FIXME: it should be possible to do this without using a class!
template <>
class WvDeserialize<WvBuf *>
{
public:
    static WvBuf *go(WvBuf &buf)
    {
	size_t len = wv_deserialize<size_t>(buf);
	WvBuf *outbuf = new WvInPlaceBuf(new char[len], 0, len, true);
	outbuf->merge(buf, len);
	return outbuf;
    }
};


/** Deserialize a list of serializable things. */
template <typename T>
class WvDeserialize<WvList<T> *>
{
public:
    static WvList<T> *go(WvBuf &buf)
    {
	WvList<T> *list = new WvList<T>;
	size_t nelems = wv_deserialize<size_t>(buf);
	
	for (size_t count = 0; count < nelems; count++)
	{
	    T t = wv_deserialize<T>(buf);
	    list->append(new T(t), true);
	}
	
	return list;
    }
};

template <>
class WvDeserialize<WvStringList*>
{
public:
    static WvStringList *go(WvBuf &buf)
    {
        WvStringList *list = new WvStringList();
        size_t nelems = wv_deserialize<size_t>(buf);

        for (size_t count = 0; count < nelems; count++)
        {
            WvString str = wv_deserialize<WvString>(buf);
            list->append(str);
        }

        return list;
    }
};

#endif // __WVSERIALIZE_H
