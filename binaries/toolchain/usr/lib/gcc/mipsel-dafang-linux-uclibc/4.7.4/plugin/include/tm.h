#ifndef GCC_TM_H
#define GCC_TM_H
#define TARGET_CPU_DEFAULT (((MASK_SPLIT_ADDRESSES)|MASK_EXPLICIT_RELOCS)|MASK_RELAX_PIC_CALLS)
#ifndef TARGET_ENDIAN_DEFAULT
# define TARGET_ENDIAN_DEFAULT 0
#endif
#ifndef LIBC_GLIBC
# define LIBC_GLIBC 1
#endif
#ifndef LIBC_UCLIBC
# define LIBC_UCLIBC 2
#endif
#ifndef LIBC_BIONIC
# define LIBC_BIONIC 3
#endif
#ifndef LIBC_MUSL
# define LIBC_MUSL 4
#endif
#ifndef DEFAULT_LIBC
# define DEFAULT_LIBC LIBC_UCLIBC
#endif
#ifndef ANDROID_DEFAULT
# define ANDROID_DEFAULT 0
#endif
#ifdef IN_GCC
# include "options.h"
# include "insn-constants.h"
# include "config/dbxelf.h"
# include "config/elfos.h"
# include "config/gnu-user.h"
# include "config/linux.h"
# include "config/glibc-stdint.h"
# include "config/mips/mips.h"
# include "config/linux-android.h"
# include "config/mips/gnu-user.h"
# include "config/mips/linux.h"
# include "config/initfini-array.h"
#endif
#if defined IN_GCC && !defined GENERATOR_FILE && !defined USED_FOR_TARGET
# include "insn-flags.h"
#endif
# include "defaults.h"
#endif /* GCC_TM_H */
