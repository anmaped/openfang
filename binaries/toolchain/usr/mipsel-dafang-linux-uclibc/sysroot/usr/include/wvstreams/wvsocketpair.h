#ifndef __WVSOCKETPAIR_H
#define __WVSOCKETPAIR_H

#ifndef _WIN32
#include <sys/socket.h>
#endif

/**
 * Like socketpair(), but works in win32 and doesn't ask you about the
 * protocol (since that's irrelevant, really).  'type' is like the type
 * parameter to socketpair(), and should be either SOCK_DGRAM or SOCK_STREAM.
 * 
 * The return value and errno codes are the same as for socketpair().
 */
int wvsocketpair(int type, int socks[2]);

#endif // __WVSOCKETPAIR_H
