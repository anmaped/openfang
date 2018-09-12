#ifndef __UNIFILESYSTEMGEN_H
#define __UNIFILESYSTEMGEN_H

#include "uniconfgen.h"
#include <sys/types.h>

/**
 * Creates a UniConf tree that mirrors some point in the Linux filesystem,
 * with restrictions. The root of the point to be mirrored is a directory
 * name.  Additionally, the mode for creation of all files and directories
 * must be given.
 * 
 * UniConf keys are mapped to filesystem paths in the obvious way (delimit
 * and precede the list of names with "/", then append to the path for the
 * point being mirrored).
 * 
 * Keys corresponding to regular files have value equal to the content of
 * that file (plus a terminating NUL). Keys whose corresponding pathname
 * does not exist have value equal to the null string. Keys corresponding
 * to things other than regular files have value equal to the empty string,
 * unless their last segment is "." or "..", in which case they have value
 * equal to the null string.
 * 
 * Due to these definitions, the UniFileSystemGen violates the UniConfGen
 * semantics in that it is not possible for a key to have simultaneously a
 * non-empty value and children and it is not possible for a key named
 * "." or ".." to exist. Any set operation that by the UniConfGen semantics
 * would cause either of those things to be true will instead do nothing.
 * These shortcomings are permanent.
 * 
 * If an unrecoverable error occurs during set, it will fail, but will
 * possibly still have an effect. If an unrecoverable error occurs
 * during get, it will return the null string. If an unrecoverable error
 * occurs during iterator(), it will return a NULL pointer.
 * 
 * Presently, callbacks are never triggered.
 * 
 * Files containing embedded NUL characters don't currently work quite right
 * because WvString can't deal with them.  They'll stop at the first NUL.
 */
class UniFileSystemGen : public UniConfGen
{
public:
    UniFileSystemGen(WvStringParm _dir, mode_t _mode);
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
    virtual void setv(const UniConfPairList &pairs);
    virtual void flush_buffers() {}
    virtual Iter *iterator(const UniConfKey &key);
private:
    WvString dir;
    mode_t mode;
};

#endif
