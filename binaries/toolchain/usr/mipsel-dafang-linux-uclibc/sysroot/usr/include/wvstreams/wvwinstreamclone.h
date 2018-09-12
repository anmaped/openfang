/* -*- Mode: C++ -*- */
#pragma once
#include "wvstreamclone.h"
#include <map>
#include <vector>
#define WIN32_LEAN_AND_MEAN
//#define NOMINMAX
#include <windows.h>

#define WM_SELECT (WM_USER)
#define WM_DONESELECT (WM_USER+1)

class WvWinStreamClone :
    public WvStreamClone
{
public:
    WvWinStreamClone(WvStream *_cloned);
    ~WvWinStreamClone();
    static DWORD Initialize();

private:
    // types
    typedef std::map<SOCKET, long> SocketEventsMap;
    typedef std::map<HWND, WvWinStreamClone *> WndStreamMap;
    typedef std::vector<HWND> WndVector;

    // class members
    static ATOM s_aClass;
    static WndVector s_wndpool;
    static WndStreamMap s_wndmap;
    const static UINT_PTR TIMER_ID = 12345;
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    // instance members
    SelectInfo m_si;
    int m_msec_timeout;
    HWND m_hWnd;
    bool m_pending_callback;
    bool m_select_in_progress;

    void pre_poll();
    void post_poll();
    void select_set(SocketEventsMap &sockmap, fd_set *set, long event );
    void select_callback(SOCKET socket, int event, int error);
    HWND alloc_wnd();
    void free_wnd(HWND w);

public:
    void setclone(IWvStream *newclone);
};


