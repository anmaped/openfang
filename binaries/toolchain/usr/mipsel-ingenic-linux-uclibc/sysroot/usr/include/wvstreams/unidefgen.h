/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * UniDefGen is a UniConfGen for retrieving data with defaults
 * 
 * Usable with the moniker default:
 */

#ifndef __UNIDEFGEN_H
#define __UNIDEFGEN_H

#include "unifiltergen.h"

/*
 * The defaults are stored and accessed by using a * in the keyname. The *
 * can represent either a segment of the path, or can can be left as a key
 * in a path so that any search to that path returns a result.
 *
 * (note: odd spacing is only to avoid putting * and / directly together)
 *
 * For example, you could set /twister/expression/ * /reality =
 * "/tmp/twister" and then a search for /twister/expression/bob/reality
 * would return the result.
 *
 * There is no limitation on where the * can be placed or the number of *s.
 * For example: /twister/ * / * / reality / *
 * would be an acceptable (though somewhat insane) use. This would make it
 * so a search to /twister/(whatever)/(whatever)/reality/(whatever) would
 * always return a key.
 *
 * If a more absolute path exists, then it will be returned instead of the
 * defaults. Precedence is given to matches existing closer to the end of
 * the key.
 *
 * If the key is set to '*n', it will return the n'th element from the end of
 * the absolute path that was passed in.  For instance, if
 * /twister/ * / * / reality is set to *1, then a search for
 * /twister/expression/bob/reality will return 'bob'.  If it is set to *2, the
 * search will return 'expression'.  If it were set to *3 (or *0), the result is
 * undefined.
 */
class UniDefGen : public UniFilterGen
{
    UniConfKey finddefault(const UniConfKey &key, char *p, char *q);
    WvString replacewildcard(const UniConfKey &key,
			     const UniConfKey &defkey, WvStringParm in);

public:
    UniDefGen(IUniConfGen *gen) : UniFilterGen(gen) { }

    /***** Overridden members *****/

    virtual bool keymap(const UniConfKey &unmapped_key, UniConfKey &mapped_key);
    virtual void flush_buffers() { }
    virtual WvString get(const UniConfKey &key);
    virtual void set(const UniConfKey &key, WvStringParm value);
};

#endif // __UNIDEFGEN_H
