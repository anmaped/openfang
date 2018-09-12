/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */
#ifndef __WVPROTOSTREAM_H
#define __WVPROTOSTREAM_H

#include "wvstreamclone.h"

class WvLog;

/**
 * WvProtoStream is a framework that makes it easy to communicate using
 * common command-response driven protocols.  This is supposed to be
 * flexible enough to handle FTP, HTTP, SMTP, tunnelv, Weaver rcmd, and
 * many others.
 */
class WvProtoStream : public WvStreamClone
{
public:
    class Token
    {
    public:
	WvString data;
	size_t length;
	
	Token();
	Token(const unsigned char *_data, size_t _length);
	void fill(const unsigned char *_data, size_t _length);
	~Token();
    };
    
    DeclareWvList(Token);

    WvDynBuf tokbuf;
    bool log_enable;

    WvProtoStream(WvStream *_cloned, WvLog *_debuglog = NULL);
    virtual ~WvProtoStream();
    
    /** override uwrite() so we can log all output */
    virtual size_t uwrite(const void *buffer, size_t size);

    // Routines to convert an input line into a set of Tokens.
    virtual Token *next_token();
    WvString next_token_str();
    WvString token_remaining();
    virtual TokenList *tokenize();
    size_t list_to_array(TokenList *tl, Token **array);
    Token *tokline(const char *line);
    
    /** Convert token strings to enum values */
    int tokanal(const Token &t, const char **lookup,
		bool case_sensitive = false);
    
    // finite state machine
    int state;
    virtual void do_state(Token &t1);
    virtual void switch_state(int newstate);
    
    /** pass input through to the state machine, one line at a time */
    virtual void execute();
    
protected:
    WvLog *logp;
    
public:
    const char *wstype() const { return "WvProtoStream"; }
};


#endif // __WVPROTOSTREAM_H
