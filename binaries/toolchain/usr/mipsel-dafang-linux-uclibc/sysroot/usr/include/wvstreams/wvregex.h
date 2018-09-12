/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2004 Net Integration Technologies, Inc.
 * 
 * Regular expression support though libc
 */ 
#ifndef __WVREGEX_H
#define __WVREGEX_H

#include "wverror.h"
#include "wvstring.h"
#include <sys/types.h>
#include <regex.h>

#define __WVRE_REG(n) __wvre_r##n
#define __WVRE_DECL_FORM(n) WvString &__WVRE_REG(n) = WvRegex::__wvre_null_reg
#define WVREGEX_REGS_DECL \
    	    	__WVRE_DECL_FORM( 0), __WVRE_DECL_FORM( 1), \
    	    	__WVRE_DECL_FORM( 2), __WVRE_DECL_FORM( 3), \
		__WVRE_DECL_FORM( 4), __WVRE_DECL_FORM( 5), \
		__WVRE_DECL_FORM( 6), __WVRE_DECL_FORM( 7), \
		__WVRE_DECL_FORM( 8), __WVRE_DECL_FORM( 9), \
		__WVRE_DECL_FORM(10), __WVRE_DECL_FORM(11), \
		__WVRE_DECL_FORM(12), __WVRE_DECL_FORM(13), \
		__WVRE_DECL_FORM(14), __WVRE_DECL_FORM(15), \
		__WVRE_DECL_FORM(16), __WVRE_DECL_FORM(17), \
		__WVRE_DECL_FORM(18), __WVRE_DECL_FORM(19)
#define __WVRE_CALL_FORM(n) __WVRE_REG(n)
#define WVREGEX_REGS_CALL \
    	    	__WVRE_CALL_FORM( 0), __WVRE_CALL_FORM( 1), \
    	    	__WVRE_CALL_FORM( 2), __WVRE_CALL_FORM( 3), \
		__WVRE_CALL_FORM( 4), __WVRE_CALL_FORM( 5), \
		__WVRE_CALL_FORM( 6), __WVRE_CALL_FORM( 7), \
		__WVRE_CALL_FORM( 8), __WVRE_CALL_FORM( 9), \
		__WVRE_CALL_FORM(10), __WVRE_CALL_FORM(11), \
		__WVRE_CALL_FORM(12), __WVRE_CALL_FORM(13), \
		__WVRE_CALL_FORM(14), __WVRE_CALL_FORM(15), \
		__WVRE_CALL_FORM(16), __WVRE_CALL_FORM(17), \
		__WVRE_CALL_FORM(18), __WVRE_CALL_FORM(19)

/*!
@brief WvRegex -- Unified support for regular expressions.

Supports matching compiled regular expressions and capturing substrings
in registers.
!*/
class WvRegex: public WvErrorBase
{
private:
    bool have_preg;
    regex_t preg;
    
    bool match(WvStringParm string, int eflags,
    	    size_t nmatch, regmatch_t pmatch[]) const;

    virtual void seterr(int _errnum);

    bool _match(WvStringParm string, int eflags,
    	    int &match_start, int &match_end, WVREGEX_REGS_DECL) const
    {
    	regmatch_t pmatch[21];
    	int nmatch = 1;

#define __WVRE_COUNT_REGS(n) \
    	if ( &__WVRE_REG(n) != &__wvre_null_reg) ++nmatch

    	__WVRE_COUNT_REGS( 0); __WVRE_COUNT_REGS( 1);
    	__WVRE_COUNT_REGS( 2); __WVRE_COUNT_REGS( 3);
    	__WVRE_COUNT_REGS( 4); __WVRE_COUNT_REGS( 5);
    	__WVRE_COUNT_REGS( 6); __WVRE_COUNT_REGS( 7);
    	__WVRE_COUNT_REGS( 8); __WVRE_COUNT_REGS( 9);
    	__WVRE_COUNT_REGS(10); __WVRE_COUNT_REGS(11);
    	__WVRE_COUNT_REGS(12); __WVRE_COUNT_REGS(13);
    	__WVRE_COUNT_REGS(14); __WVRE_COUNT_REGS(15);
    	__WVRE_COUNT_REGS(16); __WVRE_COUNT_REGS(17);
    	__WVRE_COUNT_REGS(18); __WVRE_COUNT_REGS(19);

     	if (!match(string, eflags, nmatch, pmatch)) return false;

     	match_start = pmatch[0].rm_so;
     	match_end = pmatch[0].rm_eo;

#define __WVRE_STORE_REGS(n) \
     	if (&__WVRE_REG(n) != &__wvre_null_reg \
     	    	&& pmatch[n+1].rm_so != -1 && pmatch[n+1].rm_eo != -1) \
     	{ \
     	    int len = pmatch[n+1].rm_eo-pmatch[n+1].rm_so; \
     	    __WVRE_REG(n).setsize(len+1); \
     	    memcpy(__WVRE_REG(n).edit(), &string[pmatch[n+1].rm_so], len); \
     	    __WVRE_REG(n).edit()[len] = '\0'; \
     	}

    	__WVRE_STORE_REGS( 0); __WVRE_STORE_REGS( 1);
    	__WVRE_STORE_REGS( 2); __WVRE_STORE_REGS( 3);
    	__WVRE_STORE_REGS( 4); __WVRE_STORE_REGS( 5);
    	__WVRE_STORE_REGS( 6); __WVRE_STORE_REGS( 7);
    	__WVRE_STORE_REGS( 8); __WVRE_STORE_REGS( 9);
    	__WVRE_STORE_REGS(10); __WVRE_STORE_REGS(11);
    	__WVRE_STORE_REGS(12); __WVRE_STORE_REGS(13);
    	__WVRE_STORE_REGS(14); __WVRE_STORE_REGS(15);
    	__WVRE_STORE_REGS(16); __WVRE_STORE_REGS(17);
    	__WVRE_STORE_REGS(18); __WVRE_STORE_REGS(19);
     	     	
     	return true;
    }

public:
    //!
    //! Flags that affect interpretation of the regex; used in Regex() and
    //! set()
    //! 
    enum CFlags {
    	// Use (obsolete) basic regex syntax (like grep).  See regex(7).
    	BASIC = 0,
    	// Use extended regex syntax (like egrep).  See regex(7).
    	EXTENDED = REG_EXTENDED,
    	// Case insensitive
    	ICASE = REG_ICASE,
    	// Do not collect match start and end or registers; faster
    	NOSUB = REG_NOSUB,
    	// Match-any-character operators don't match a newline.  See regex(3)
    	NEWLINE = REG_NEWLINE
    };
    static const int default_cflags;

    //!
    //! Flags that affect matching of regex.  Used in match() and
    //! continuable_match()
    //!
    enum EFlags
    {
    	// Matching begining of line always fails (unless NEWLINE cflag is set)
    	NOTBOL = REG_NOTBOL,
    	// Matching end of line always fails (unless NEWLINE cflag is set)
    	NOTEOL = REG_NOTEOL
    };
    static const int default_eflags;

    //!
    //! Internal use only
    //!
    static WvString __wvre_null_reg;

    //!
    //! Construct an empty regex object.  Matches will always fail until set()
    //! is called with a valid regex.
    //!
    WvRegex() : have_preg(false) {}
    //!
    //! Construct a regex object, compiling the given regex
    //!
    //! \param regex The new regular expression to match
    //! \param cflags CFlags used to compile the regular expression;
    //!               the defaults are case sensitive, extended RE.
    //!
    WvRegex(WvStringParm regex, int cflags = default_cflags) : have_preg(false)
    	{ set(regex, cflags); }
    ~WvRegex();
    
    //!
    //! Replace the current regex to match with a new one.
    //!
    //! \param regex The new regular expression to match
    //! \param cflags CFlags used to compile the regular expression;
    //!               the defaults are case sensitive, extended RE.
    //!
    bool set(WvStringParm regex, int cflags = default_cflags);
    
    //!
    //! Match a given string against the compiled regular expression
    //!
    //! \param string The string to match
    //! \param (remaining) WvString registers to capture substring matches
    //!     	as specified in the RE
    //!
    //! \code
    //! extern WvString line;
    //! WvString match;
    //! if (re.match(line, match))
    //!	    wvout->print("Matching substring is '%s'\n", match);
    //! \endcode
    //!
    bool match(WvStringParm string, WVREGEX_REGS_DECL) const
    {
    	int match_start, match_end;
    	return _match(string, default_eflags,
    	    	match_start, match_end, WVREGEX_REGS_CALL); 
    }
    //!
    //! Match a given string against the compiled regular expression
    //!
    //! \param string The string to match
    //! \param eflags EFlags that affect matching 
    //! \param (remaining) WvString registers to capture substring matches
    //!     	as specified in the RE
    //!
    bool match(WvStringParm string, int eflags, WVREGEX_REGS_DECL) const
    {
    	int match_start, match_end;
    	return _match(string, eflags,
    	    	match_start, match_end, WVREGEX_REGS_CALL); 
    }
    
    //!
    //! Match a given string against the compiled regular expression, 
    //! capturing the start and end positions of the matching string.
    //!
    //! \param string The string to match
    //! \param match_start If the match succeeds, the starting index of the
    //!	    	match in string
    //! \param match_end If the match succeeds, the index of the character in
    //!	    	string following the last character of the match
    //! \param (remaining) WvString registers to capture substring matches
    //!     	as specified in the RE
    //!
    //! \code
    //! extern WvString line;
    //! int start = 0;
    //! WvString match;
    //! int match_start, match_end;
    //! while (re.continuable_match(&line[start],
    //!	    	match_start, match_end, match))
    //! {
    //!	    wvout->print("Matching substring is '%s'@[%s,%s)\n",
    //!	    	    match, match_start, match_end);
    //!	    start += match_end;
    //! }
    //! \endcode
    //!
    bool continuable_match(WvStringParm string,
            int &match_start, int &match_end,
    	    WVREGEX_REGS_DECL) const
    {
    	return _match(string, default_eflags,
    	    	match_start, match_end, WVREGEX_REGS_CALL); 
    }
    //!
    //! Match a given string against the compiled regular expression, 
    //! capturing the start and end positions of the matching string.
    //!
    //! \param string The string to match
    //! \param eflags EFlags that affect matching 
    //! \param match_start If the match succeeds, the starting index of the
    //!	    	match in string
    //! \param match_end If the match succeeds, the index of the character in
    //!	    	string following the last character of the match
    //! \param (remaining) WvString registers to capture substring matches
    //!     	as specified in the RE
    //!
    bool continuable_match(WvStringParm string, int eflags,
            int &match_start, int &match_end,
    	    WVREGEX_REGS_DECL) const
    {
    	return _match(string, eflags,
    	    	match_start, match_end, WVREGEX_REGS_CALL); 
    }
};

#endif // __WVREGEX_H
