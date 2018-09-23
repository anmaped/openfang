/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A tun device wrapper.
 */
#ifndef __WV_TUNDEV_H
#define __WV_TUNDEV_H

#include "wvfile.h"
#include "wvinterface.h"
#include "wvaddr.h"

/**
 * WvTunDev provides a convenient way of using Linux tunnel devices.
 * 
 * If you don't have the /dev/net/tun device, try doing:
 * mknod /dev/net/tun c 10 200.
 * 
 */
class WvTunDev : public WvFile
{
public:
    /**
     * Creates a tunnel device and its associated interface.
     *
     * "addr" is the initial ip address for the interface
     * "mtu" is the max transfer unit, default 1400
     */
    WvTunDev(const WvIPNet &addr, int mtu = 1400);

    /** Contains the name of the interface associated with the device. */
    WvString ifcname;

private:
    void init(const WvIPNet &addr, int mtu);
    
public:
    const char *wstype() const { return "WvTunDev"; }
};


#endif
