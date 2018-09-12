/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 * A Qt object that invokes its callback whenever it receives
 * an event.  This is useful for deferring processing to the
 * Qt event loop.  Use it to avoid problems resulting from the
 * non-reentrant nature of WvStream::execute().
 */
#ifndef __WVQTHOOK_H
#define __WVQTHOOK_H

#include <qobject.h>
#include <qevent.h>
#include "wvtr1.h"

class WvQtHook;
// parameters are: WvQtHook &, int type, void *data
typedef wv::function<void(WvQtHook&, int, void*)> WvQtHookCallback;

class WvQtHook : public QObject
{
    Q_OBJECT
    WvQtHookCallback callback;

public:
    WvQtHook(WvQtHookCallback _callback = NULL);

    // sets the callback function to be invoked
    void setcallback(WvQtHookCallback _callback);
    
    // posts an event to the Qt event loop to be sent to the
    // attached callback later
    void post(int type = 0, void *data = NULL);

    // sends an event to the attached callback now
    void send(int type = 0, void *data = NULL);

    // internal
    virtual bool event(QEvent *event);
};

#endif // __WVQTHOOK_H
