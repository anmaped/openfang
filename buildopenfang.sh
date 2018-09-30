#!/bin/sh

CPW=$(pwd)

DIR=_build
[[ -d $DIR ]] || { printf '%s does not exist!\n' "$DIR"; mkdir $DIR; cp fs $DIR/ -r; }

ID="(`git rev-parse HEAD`) `date +"%Y-%m-%d %H:%M"`"
SHORTID=`git rev-parse --short HEAD`

echo $ID > fs/opt/version

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
cp ../../uboot-v2013.07.tar.xz dl/

WDIR=$CPW/$DIR/buildroot-2014.08

# Patch buildroot if gcc >= 5
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

# copy custom opendafang packages to buildroot directory
cp $CPW/buildroot/* . -r

make

# constructs release
tar c $WDIR/output/host | xz --best > $CPW/toolchain-$SHORTID.tar.xz
tar c $WDIR/output/images | xz --best > $CPW/images-$SHORTID.tar.xz


