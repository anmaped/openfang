/* -*- Mode: C++ -*- */
#ifndef WVSTREAMSDEBUGGER_H
#define WVSTREAMSDEBUGGER_H

#include <map>

#include "wverror.h"
#include "wvstringlist.h"
#include "wvtclstring.h"
#include "wvtr1.h"

class WvStreamsDebugger
{
public:

    // The callback type used to pass the results back to the application
    // that calls WvStreamsDebugger::run.  The application is allowed
    // to consume the WvStringList of results.
    typedef wv::function<void(WvStringParm, WvStringList&)> ResultCallback;

    // Debugging commands are implemented through the following three
    // callbacks:
    //   - InitCallback is optional and is used to allocate state
    //     for an instance of WvStreamsDebugger for the given command
    //   - RunCallback is required and is used to actually execute
    //     the command as a result of a call to WvStreamsDebugger::run
    //   - CleanupCallback is optional and is used to free state
    //     for an instance of WvStreamsDebugger for the given command
    typedef wv::function<void*(WvStringParm)> InitCallback;
    typedef wv::function<WvString(WvStringParm, WvStringList&,
				  ResultCallback, void*)> RunCallback;

    typedef wv::function<void(WvStringParm, void*)> CleanupCallback;

    // The WvStreamsDebugger::foreach function can be used to update
    // state in every instance of WvStreamsDebugger for a given command.
    typedef wv::function<void(WvStringParm, void*)> ForeachCallback;
    
private:

    struct Command
    {
        InitCallback init_cb;
        RunCallback run_cb;
        CleanupCallback cleanup_cb;
        
        Command(InitCallback _init_cb, RunCallback _run_cb,
                CleanupCallback _cleanup_cb)
        {
            init_cb = _init_cb;
            run_cb = _run_cb;
            cleanup_cb = _cleanup_cb;
        }
    };
    typedef std::map<WvString, Command> CommandMap;
    static CommandMap *commands;
    typedef std::map<WvString, void*> CommandDataMap;
    CommandDataMap command_data;
    
    void *get_command_data(WvStringParm cmd, Command *command);
    friend class WvStreamsDebuggerStaticInitCleanup;
    
public:

    WvStreamsDebugger();
    ~WvStreamsDebugger();

    WvString run(WvStringParm cmd, WvStringList &args,
            ResultCallback result_cb);
    
    static bool add_command(WvStringParm cmd,
            InitCallback init_cb,
            RunCallback run_cb,
            CleanupCallback cleanup_cb);
            
    static bool foreach(WvStringParm cmd, ForeachCallback foreach_cb);

private:

    static WvString help_run_cb(WvStringParm cmd,
            WvStringList &args,
            ResultCallback result_cb, void *);
};

#endif
