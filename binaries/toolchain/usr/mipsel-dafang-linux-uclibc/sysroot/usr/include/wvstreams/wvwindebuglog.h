/* -*- Mode: C++ -*- */
#include "wvlogrcv.h"

class WvWinDebugLog :
    public WvLogRcv
{
    virtual void _mid_line(const char *str, size_t len);
    virtual ~WvWinDebugLog();
};
