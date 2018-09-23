#ifndef __WVLINKERHACK_H
#define __WVLINKERHACK_H

/*
 * Don't call this directly.  It's used by WV_LINK_TO().
 */
extern int **__wv_link_func(int **to);

/**
 * Create a target that WV_LINK_TO can point at when it wants to make sure
 * your object file is linked into the final binary.
 * 
 * Do not put the name in quotes.
 * 
 * Example (in uniinigen.cc): WV_LINK(UniIniGen);
 */
#define WV_LINK(name) volatile int __wv_link_##name

/**
 * Link to a target created by WV_LINK, ensuring that the file which created
 * the WV_LINK is included in your final binary.  This is useful for ensuring
 * particular monikers will always be available when you link statically,
 * even though you don't refer to any particular classes implementing that
 * moniker directly.
 * 
 * Do not put the name in quotes.
 * 
 * Example (somewhere other than uniinigen.cc): WV_LINK_TO(UniIniGen);
 */
#define WV_LINK_TO(name) \
	extern volatile int __wv_link_##name; \
        namespace { volatile int __wv_local_a_##name = __wv_link_##name; }

#endif // __WVLINKERHACK_H
