/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A version of WvLogConsole that colorizes output on terminals
 * which support ANSI color sequences
 */
#ifndef __WVCOLORLOGCONSOLE_H
#define __WVCOLORLOGCONSOLE_H

#include "wvlogrcv.h"

/**
 * WvLogRcv adds some intelligence to WvLogRcvBase, to keep
 * track of line-prefix-printing and other formatting information.
 */
class WvColorLogConsole: public WvLogConsole
{
    bool colorize;

public:
    WvColorLogConsole(int _fd,
            WvLog::LogLevel _max_level = WvLog::NUM_LOGLEVELS);
    virtual ~WvColorLogConsole();

    static bool is_tty(int fd);
    static bool can_colorize(int fd, const char *TERM);

    static const char *color_start_seq(WvLog::LogLevel log_level);
    static const char *clear_to_eol_seq(WvLog::LogLevel log_level);
    static const char *color_end_seq(WvLog::LogLevel log_level);

protected:
    virtual void _begin_line();
    virtual void _mid_line(const char *str, size_t len);
    virtual void _end_line();
};

#endif // __WVCOLORLOGCONSOLE_H
