#!/bin/sh

CPW=$(pwd)

DIR=__build
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

# copy the new package v4l2rtspserver
# here

#make


cd ..
# compile v4l2rtspserver
git clone https://github.com/mpromonet/v4l2rtspserver.git


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

