#ifndef __WVLOGSTREAM_H
#define __WVLOGSTREAM_H

#include "wvlogrcv.h"

/**
 * A WvLogRcv that sends its log messages to any supplied WvStream.
 * 
 * Takes ownership of the given stream, so it will be release()d when 
 * this object goes away.
 */
class WvLogStream: public WvLogRcv
{
public:
    WvLogStream(IWvStream *s,
		WvLog::LogLevel _max_level = WvLog::NUM_LOGLEVELS);
    virtual ~WvLogStream();

protected:
    IWvStream *cloned;
    
    virtual void _mid_line(const char *str, size_t len);
};

#endif // __WVLOGSTREAM_H
