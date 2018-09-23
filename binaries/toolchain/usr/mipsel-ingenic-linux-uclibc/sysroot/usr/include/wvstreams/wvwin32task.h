/* -*- Mode: C++ -*- */
#ifndef __WVWIN32TASK_H
#define __WVWIN32TASK_H

#include "wvstring.h"
#include "wvlinklist.h"

#include <windows.h>

#define WVTASK_MAGIC 0x123678

class WvTaskMan;

class WvTask
{
    friend class WvTaskMan;
    typedef void TaskFunc(void *userdata);
    
    static int taskcount, numtasks, numrunning;
    int magic_number, *stack_magic;
    WvString name;
    int tid;
    
    size_t stacksize;
    bool running, recycled;
    
    WvTaskMan &man;
    LPVOID mystate;	// used for resuming the task
    
    TaskFunc *func;
    void *userdata;

    static VOID CALLBACK MyFiberProc(PVOID lpParameter);
    WvTask(WvTaskMan &_man, size_t _stacksize = 64*1024);
    
public:
    virtual ~WvTask();
    
    void start(WvStringParm _name, TaskFunc *_func, void *_userdata);
    bool isrunning() const
        { return running; }
    void recycle();
    int get_tid() const { return tid; }
    WvString get_name() const { return name; }
};


DeclareWvList(WvTask);

/** Provides co-operative multitasking support among WvTask instances. */
class WvTaskMan
{
    friend class WvTask;

    static WvTaskMan *singleton;
    static int links;

    static int magic_number;
    static WvTaskList free_tasks;
    
    static void get_stack(WvTask &task, size_t size);
    static void stackmaster();
    static void _stackmaster();
    static void do_task();
    
    static WvTask *stack_target;
    
    static WvTask *current_task;
    static LPVOID toplevel;
    
    WvTaskMan();
    virtual ~WvTaskMan();
   
public:
    /// get/dereference the singleton global WvTaskMan
    static WvTaskMan *get();
    static void unlink();
  
    WvTask *start(WvStringParm name,
		  WvTask::TaskFunc *func, void *userdata,
		  size_t stacksize = 256*1024);
    
    // run() and yield() return the 'val' passed to run() when this task
    // was started.
    static int run(WvTask &task, int val = 1);
    static int yield(int val = 1);
    
    static WvTask *whoami()
        { return current_task; }
};

#endif // __WVWIN32TASK_H
