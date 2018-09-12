/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 *
 */
#ifndef __WVQTSTREAMCLONE_H
#define __WVQTSTREAMCLONE_H

#include <qobject.h>
#include <qintdict.h>
#include <qsocketnotifier.h>
#include <qtimer.h>
#include <qmetaobject.h>
#include "wvstreamclone.h"

/**
 * Wraps another WvStream and attaches it to the normal Qt event loop.
 * 
 * If you are using this object exclusively to manage all of your
 * streams, then you do not need to have a normal WvStreams
 * select()/callback() loop in your application at all.
 * 
 * However, should you leave the Qt event loop and wish to continue
 * using this WvStream, call qt_detach() first, then run a normal
 * WvStreams event loop.  If you do not do this, events may be
 * lost!  Later, you may resume the Qt event loop at any time
 * by leaving your WvStreams event loop and calling qt_attach().
 * 
 * Multiple instances of this object may coexist to wrap different
 * WvStream's or WvStreamList's.
 * 
 */
class WvQtStreamClone : public QObject, public WvStreamClone
{
    Q_OBJECT
    int msec_timeout;
    
    SelectInfo si;
    bool pending_callback;
    bool first_time;
    bool select_in_progress;
    int last_max_fd;
    QIntDict<QSocketNotifier> notify_readable;
    QIntDict<QSocketNotifier> notify_writable;
    QIntDict<QSocketNotifier> notify_exception;
    QTimer select_timer;

public:
    /**
     * WvQtStreamClone takes ownership of the stream you give it
     * just like WvStreamClone.  See comments there.
     *
     * Timeout sets the time between polls with select().
     * A value less than zero is regarded as an infinite timeout.
     */
    WvQtStreamClone(IWvStream *_cloned = NULL, int msec_timeout = -1);
    virtual ~WvQtStreamClone();

    // Call this to stop managing this stream via the Qt event loop.
    // Afterwards you may run a normal WvStream event loop based
    // on this object.
    void qt_detach();

    // Call this to resume managing this stream via the Qt event loop.
    // This is the default state when the object is constructed.
    void qt_attach();

    // Changes the timeout
    // You may need to adjust the timeout when using badly behaved streams
    void set_timeout(int msec_timeout);

private:
    // Called before the Qt event loop does its select()
    void pre_poll();
    // Called after the Qt event loop has finished its notifications
    void post_poll();

private slots:
    /** These things mark the beginning of a select pass **/
    // Qt event loop hook (happens before each iteration)
    void qt_begin_event_loop_hook();

    /** These things mark the end of a select pass **/
    // Qt select timeout expired
    void select_timer_expired();
    // Called when a file descriptor has been marked readable
    void fd_readable(int fd);
    // Called when a file descriptor has been marked writable
    void fd_writable(int fd);
    // Called when a file descriptor has been marked with an exception
    void fd_exception(int fd);

    // Needed or certain assertions fail ;)
    virtual void execute();

public:
    virtual void setclone(IWvStream *clone);
};

#endif // __WVQTSTREAMCLONE_H
