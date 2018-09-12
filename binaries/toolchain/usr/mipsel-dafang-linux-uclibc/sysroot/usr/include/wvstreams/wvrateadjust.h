/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * WvRateAdjust is a WvEncoder that makes sure data comes out of it at a
 * given average rate.  It adds duplicate samples or removes extra samples
 * from the input in order to achieve this.
 * 
 * The current version doesn't do anything fancy like linear or spline
 * interpolation, since it's being built mainly for minor rate adjustments
 * like 8000 Hz to 8020 Hz, and interpolation probably doesn't help too 
 * much there.  It's also nicer to not have to worry explicitly about the
 * number format, which you would have to do if you were doing actual math.
 * 
 * You can also use this as a slightly (not terribly) inefficient rate
 * *estimator* for the input stream.
 * 
 * NOTE: Time is of the essence of this encoder.
 */ 
#ifndef __WVRATEADJUST_H
#define __WVRATEADJUST_H

#include "wvencoder.h"
#include "wvtimeutils.h"

class WvRateAdjust : public WvEncoder
{
public:
    WvRateAdjust *match_rate;
 
    int sampsize, irate_n, irate_d, orate_n, orate_d;
    WvTime epoch; // the time when sampling started
    
    // the token bucket holds the "remainder" of our fake integer division
    // operation.  With this, we can make sure we always average out to
    // exactly the right rate.
    // 
    // Basically:
    //    bucket = ((orate*insamps) - (irate*outsamps)) * irate_d * orate_d
    // 
    // ...but with some care thrown in to prevent overflows.
    int bucket;
    
    WvRateAdjust(int _sampsize, int _irate_base, int _orate);
    WvRateAdjust(int _sampsize, int _irate_base, WvRateAdjust *_match_rate);
    
    int getirate()
        { return irate_n / irate_d; }
    int getorate()
        { return orate_n / orate_d; }
    
protected:
    void init(int _sampsize, int _irate_base);
    
    virtual bool _encode(WvBuf &inbuf, WvBuf &outbuf, bool flush);
};

#endif // __WVRATEADJUST_H
