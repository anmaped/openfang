/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A very simple word wrapping encoder.
 */
#ifndef __WVWORDWRAP_H
#define __WVWORDWRAP_H
 
#include "wvencoder.h"
 
/**
 * Trivial word wrapper.
 * Recognizes newlines in input stream as end of line.
 * Words are output until no more will fit, in which case a newline
 * is output and the word is presented on the next line.  Wrapped
 * word delimiter characters are discarded such that a wrapped word
 * will always be placed at the beginning of a line.
 */
class WvWordWrapEncoder : public WvEncoder
{
    const int maxwidth;
    char *line;
    int width;     // current visual position
    int curindex;  // current index in line array
    int wordindex; // index of beginning of word in line array
    bool inword;   // if true, we're in a word

public:
    WvWordWrapEncoder(int maxwidth);
    virtual ~WvWordWrapEncoder();

protected:
    // on flush, outputs a partial line with remaining chars
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf, bool flush);
    virtual bool _reset(); // supported

private:
    void flushline(WvBuf &outbuf);
};

#endif // __WVWORDWRAP_H
