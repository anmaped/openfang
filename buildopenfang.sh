#!/bin/sh

CPW=$(pwd)

DIR=_build
[[ -d $DIR ]] || { printf '%s does not exist!\n' "$DIR"; mkdir $DIR; cp fs $DIR/ -r; }


cd $DIR

[[ -d "buildroot-2014.08" ]] || {
wget https://buildroot.org/downloads/buildroot-2014.08.tar.gz;
tar xvf buildroot-2014.08.tar.gz;
rm buildroot-2014.08.tar.gz;
}

cd buildroot-2014.08

# update config files
cp $CPW/config/.config_buildroot ./.config
cp $CPW/config/busybox.config ./package/busybox
cp $CPW/config/uClibc-ng.config ./package/uclibc

#cp $CPW/config/.config_kernel ./
mkdir dl
cp ../../kernel-3.10.14.tar.xz dl/

WDIR=$CPW/$DIR/buildroot-2014.08

# if you are using gcc 5 then
#
#  - copy patch-gcc_cp_cfns.h.patch into output/build/host-gcc-final-4.7.4 and patch it using the command:
# $ patch -p0 < patch-gcc_cp_cfns.h.patch
#
#  - copy automake.in.patch into output/build/host-automake-1.14.1 and patch it using the command:
# $ patch -p0 < automake.in.patch
#
# FOR ncurses-5.9 *USE* patch -p1 < ncurses-5.9-gcc-5.patch
#
#  - copy hashtable_itr.patch into output/build/host-mtd-1.5.1 and patch it using the command:
# $ patch -p1 < hashtable_itr.patch
#
# AUTO patch by buildroot if needed
#
GCCVER=$(gcc -dumpversion)
echo $GCCVER
if [ "$GCCVER" -ge "5" ]; then
  cp $CPW/patches/patch-gcc_cp_cfns.h.patch $WDIR/package/gcc/gcc-final
  cp $CPW/patches/automake.in.patch $WDIR/package/automake
  cp $CPW/patches/ncurses-5.9-gcc-5.patch $WDIR/package/ncurses
  cp $CPW/patches/hashtable_itr.patch $WDIR/package/mtd
  cp $CPW/patches/python2.7_gcc8__fix.patch $WDIR/package/python
  cp $CPW/patches/lzop-gcc6.patch $WDIR/package/lzop
fi

# copy the new package buildroot packages and gcc-5 patches
cp $CPW/buildroot/* . -r

make


