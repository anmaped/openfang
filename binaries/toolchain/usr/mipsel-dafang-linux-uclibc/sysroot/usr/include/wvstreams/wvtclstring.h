/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * FIXME:
 *   It would be possible to represent arbitrary binary blobs using this
 *   technique, but we'd have to avoid using null-terminated strings in a few
 *   places, particularly in the input to wvtcl_escape().
 * 
 *   We could even make encoded binary blobs printable (although that's not
 *   _strictly_ necessary in all cases) by encoding non-printable characters
 *   using \x## notation, if wvtcl_escape() or wvtcl_unescape() supported it.
 */
/** \file
 * Functions to handle "tcl-style" strings and lists.
 * 
 * Using wvtcl_encode(), you can encode _any_ list of strings into a single
 * string, then reliably split the single string back into the list using
 * wvtcl_decode().
 * 
 * You can create recursive lists of lists by simply running wvtcl_encode()
 * on a list of strings returned from wvtcl_encode().
 * 
 * Example list encodings (all of the following lists have exactly 3 elements):
 *     foo blah weasels
 *     e1 elem2 {element 3}
 *     x1 {} "element 3"
 *     w x y\ z
 * 
 * Example list of lists:
 *     foo\ blah\ weasels {e1 elem2 {element 3}} {w x y\ z}
 */

#ifndef __WVTCLSTRING_H
#define __WVTCLSTRING_H

#include "wvbuf.h"
class WvStringMask;

// the default set of "nasties", ie. characters that need to be escaped if
// they occur somewhere in a string.
#define WVTCL_NASTY_SPACES_STR    " \t\n\r"
extern const WvStringMask WVTCL_NASTY_SPACES;

// Another default set of nasties, but only splitting on newlines
#define WVTCL_NASTY_NEWLINES_STR  "\n\r"
extern const WvStringMask WVTCL_NASTY_NEWLINES;

// {, }, \, and " are always considered "nasty."
#define WVTCL_ALWAYS_NASTY_CASE '{': case '}': case '\\': case '"'


// the default set of split characters, ie. characters that separate elements
// in a list.  If these characters appear unescaped and not between {} or ""
// in a list, they signify the end of the current element.
#define WVTCL_SPLITCHARS_STR " \t\n\r"
extern const WvStringMask WVTCL_SPLITCHARS;


/**
 * tcl-escape a string.  There are three ways to do this:
 *   1) Strings that need no escaping are unchanged.
 *   2) Strings containing characters in 'nasties' are usually encoded just
 *         by enclosing the unmodified string in braces.
 *         (For example, "foo blah" becomes "{foo blah}")
 *   3) Strings containing nasties _and_ unmatched braces are encoded using
 *         backslash notation.  (For example, " foo} " becomes "\ foo\}\ "
 */
WvString wvtcl_escape(WvStringParm s,
		      const WvStringMask &nasties = WVTCL_NASTY_SPACES);


/**
 * tcl-unescape a string.  This is generally the reverse of wvtcl_escape,
 * except we can reverse any backslashified or embraced string, even if it
 * doesn't follow the "simplest encoding" rules used by wvtcl_escape.  We
 * can also handle strings in double-quotes, ie. '"foo"' becomes 'foo'.
 */
WvString wvtcl_unescape(WvStringParm s);


/**
 * encode a tcl-style list.  This is easily done by tcl-escaping each
 * string in 'l', then appending the escaped strings together, separated by
 * the first char in splitchars.
 */
WvString wvtcl_encode(WvList<WvString> &l,
		      const WvStringMask &nasties = WVTCL_NASTY_SPACES,
		      const WvStringMask &splitchars = WVTCL_SPLITCHARS);

/**
 * Get a single tcl word from an input buffer, and return the rest of the
 * buffer untouched.  If no word can be created from the buffer, return
 * a null string and leave the buffer unmodified.
 */
WvString wvtcl_getword(WvBuf &buf,
		       const WvStringMask &splitchars = WVTCL_SPLITCHARS,
		       bool do_unescape = true);

/**
 * split a tcl-style list.  There are some special "convenience" features
 * here, which allow users to create lists more flexibly than wvtcl_encode
 * would do.
 * 
 * Elements of the list are separated by any number of any characters from
 * the 'splitchars' list.
 * 
 * Quotes are allowed around elements: '"foo"' becomes 'foo'.  These work
 * mostly like braces, except the string is assumed to be backslashified.
 * That is, '"\ "' becomes ' ', whereas '{\ }' becomes '\ ' (ie. the backslash
 * wouldn't be removed).
 * 
 * Zero-length elements must be represented by {}
 * 
 */
void wvtcl_decode(WvList<WvString> &l, WvStringParm _s,
		  const WvStringMask &splitchars = WVTCL_SPLITCHARS,
		  bool do_unescape = true);

#endif // __WVTCLSTRING_H
