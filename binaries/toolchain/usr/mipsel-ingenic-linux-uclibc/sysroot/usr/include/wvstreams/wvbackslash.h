/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 2002 Net Integration Technologies, Inc.
 *
 * C-style backslash escaping and unescaping of strings.
 */
#ifndef __WVBACKSLASH_H
#define __WVBACKSLASH_H

#include "wvencoder.h"

/**
 * An encoder that performs C-style backslash escaping of strings.
 * 
 * Use this to escape control characters, unprintable characters,
 * and optionally quotes or any other special printable characters
 * into sequences of the form \\n, \\xFF, \\", etc...
 * 
 * Supports reset().
 * 
 */
class WvBackslashEncoder : public WvEncoder
{
    WvString nasties;

public:
    /**
     * Creates a C-style backslash encoder.
     *   nasties - the set of printable characters to escape
     *             in addition to the non-printable ones
     *             (should always contain at least backslash)
     */
    WvBackslashEncoder(WvStringParm _nasties = "\\\"");
    virtual ~WvBackslashEncoder() { }

protected:
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf, bool flush);
    virtual bool _reset();
};


/**
 * An encoder that performs C-style backslash unescaping of strings.
 * 
 * Recognizes the following sequences preceeded by backslash:
 * 
 *  - a: substitutes alarm bell (ascii 7)
 *  - b: substitutes backspace (ascii 8)
 *  - f: substitutes formfeed (ascii 12)
 *  - n: substitutes newline (ascii 10)
 *  - r: substitutes carriage return (ascii 13)
 *  - t: substitutes tab (ascii 9)
 *  - v: substitutes vertical tab (ascii 11)
 *  - 0: substitutes null (ascii 0)
 *  - 0xyz: substitutes character with octal encoding xyz
 *  - xxy: substitutes character with hex encoding xy
 *  - newline: substitutes space (line continuation sequence)
 *  - \\: substitutes backslash
 *  - otherwise substitutes the next character (strips the backslash)
 * 
 * 
 * Supports reset().
 * 
 */
class WvBackslashDecoder : public WvEncoder
{
    enum State
        { Initial, Escape, Hex1, Hex2, Octal1, Octal2, Octal3 };
    State state;
    WvInPlaceBuf tmpbuf;
    int value;

public:
    /** Creates a C-style backslash decoder. */
    WvBackslashDecoder();
    virtual ~WvBackslashDecoder() { }

protected:
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf, bool flush);
    virtual bool _reset();

private:
    bool flushtmpbuf(WvBuf &outbuf);
};

#endif // __WVBACKSLASH_H
