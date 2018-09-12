/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2003 Net Integration Technologies, Inc.
 *
 * Part of an automated testing framework.  You can declare a "test function"
 * using WVTEST_MAIN, and call WVPASS and WVFAIL from there.  These produce
 * formatted data on stdout that can be read by external testrunner scripts.
 * 
 * More than one WVTEST_MAIN is allowed in a single program, and they all
 * get run.
 */ 
#ifndef __WVTEST_H
#define __WVTEST_H

#include <time.h>

class WvTest
{
    typedef void MainFunc();
    const char *descr, *idstr;
    MainFunc *main;
    int slowness;
    WvTest *next;
    static WvTest *first, *last;
    static int fails, runs;
    static time_t start_time;
    static bool run_twice;
    
    static void alarm_handler(int sig);
   
    static void print_result(bool start, const char *file, int line, 
            const char *condstr, bool result);
public:
    WvTest(const char *_descr, const char *_idstr, MainFunc *_main, int _slow);
    static int run_all(const char * const *prefixes = NULL);
    static void start(const char *file, int line, const char *condstr);
    static void check(bool cond);
    static inline bool start_check(const char *file, int line,
				   const char *condstr, bool cond)
        { start(file, line, condstr); check(cond); return cond; }
    static bool start_check_eq(const char *file, int line,
			       const char *a, const char *b, bool expect_pass);
    static bool start_check_eq(const char *file, int line, int a, int b,
                               bool expect_pass);
    static bool start_check_lt(const char *file, int line,
                               const char *a, const char *b);
    static bool start_check_lt(const char *file, int line, int a, int b);
};


#define WVPASS(cond) \
    WvTest::start_check(__FILE__, __LINE__, #cond, (cond))
#define WVPASSEQ(a, b) \
    WvTest::start_check_eq(__FILE__, __LINE__, (a), (b), true)
#define WVPASSLT(a, b) \
    WvTest::start_check_lt(__FILE__, __LINE__, (a), (b))
#define WVFAIL(cond) \
    WvTest::start_check(__FILE__, __LINE__, "NOT(" #cond ")", !(cond))
#define WVFAILEQ(a, b) \
    WvTest::start_check_eq(__FILE__, __LINE__, (a), (b), false)

#define WVTEST_MAIN3(descr, ff, ll, slowness) \
    static void _wvtest_main_##ll(); \
    static WvTest _wvtest_##ll(descr, ff, _wvtest_main_##ll, slowness); \
    static void _wvtest_main_##ll()
#define WVTEST_MAIN2(descr, ff, ll, slowness) \
    WVTEST_MAIN3(descr, ff, ll, slowness)
#define WVTEST_MAIN(descr) WVTEST_MAIN2(descr, __FILE__, __LINE__, 0)
#define WVTEST_SLOW_MAIN(descr) WVTEST_MAIN2(descr, __FILE__, __LINE__, 1)


#endif // __WVTEST_H
