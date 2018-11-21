#!/bin/sh

TAG=rc04_01

set -e

CPW=$(pwd)

DIR=_build
[ -d $DIR ] || { printf '%s does not exist!\n' "$DIR"; mkdir $DIR; }

date=$(date +"%Y-%m-%d %H:%M")
ID="($(git rev-parse HEAD)) $date"
SHORTID=$(git rev-parse --short HEAD)

echo "$ID" > fs/opt/version

cp --preserve=context fs $DIR/ -r;

sed -i "s/VERSION=.*/VERSION=\"$date\"/g" $DIR/fs/opt/autoupdate.sh
sed -i "s/TAG=.*/TAG=\"$TAG\"/g" $DIR/fs/opt/autoupdate.sh
sed -i "s/ID=.*/ID=\"$SHORTID\"/g" $DIR/fs/opt/autoupdate.sh

[ "$1" = "stamp" ] && { exit 0; }

cd $DIR

BUILDROOT_VERSION=2016.02

[ -d "buildroot-$BUILDROOT_VERSION" ] || {
wget https://buildroot.org/downloads/buildroot-$BUILDROOT_VERSION.tar.gz;
tar xvf buildroot-$BUILDROOT_VERSION.tar.gz;
rm buildroot-$BUILDROOT_VERSION.tar.gz;
cd buildroot-$BUILDROOT_VERSION
patch -p1 < "$CPW"/patches/add_fp_no_fused_madd.patch
cd ..
}

cd buildroot-$BUILDROOT_VERSION

# update config files
cp --preserve=context "$CPW"/config/buildroot.config ./.config
cp --preserve=context "$CPW"/config/busybox.config ./package/busybox
cp --preserve=context "$CPW"/config/uClibc-ng.config ./package/uclibc

[ -d "dl" ] || { mkdir dl; }

cp --preserve=context ../../kernel-3.10.14.tar.xz dl/
cp --preserve=context ../../uboot-v2013.07.tar.xz dl/

WDIR=$CPW/$DIR/buildroot-$BUILDROOT_VERSION

# Patch buildroot if gcc >= 5
#
GCCVER=$(gcc -dumpversion)
echo "GCC version: $GCCVER"
if [ "$GCCVER" -ge "5" ]; then
  cp "$CPW"/patches/automake.in.patch "$WDIR"/package/automake
  cp "$CPW"/patches/python/python2.7_gcc8__fix.patch "$WDIR"/package/python
  cp "$CPW"/patches/lzop-gcc6.patch "$WDIR"/package/lzop
fi

# copy python patches to address host-python build failing
# when host has openssl 1.1.0 headers installed
cp -f "$CPW"/patches/python/111-optional-ssl.patch "$WDIR"/package/python
cp "$CPW"/patches/python/019-force-internal-hash-if-ssl-disabled.patch "$WDIR"/package/python

# copy custom opendafang packages to buildroot directory
rm -r "$WDIR"/package/ffmpeg # use updated package version instead
cp --preserve=context "$CPW"/buildroot/* . -rf

cp "$CPW"/v4l2rtspserver-v0.0.8.tar.gz "$WDIR"/dl/

make

# constructs release with git hash label
echo "Compressing toolchain..."
tar -c -C "$WDIR"/output/host --transform s/./mipsel-ingenic-linux-uclibc/ --checkpoint=.1000 .  | xz --best > "$CPW"/toolchain-$SHORTID.tar.xz
echo "Compressing rootfs images..."
tar -c -C "$WDIR"/output/images --transform s/./openfang-images/ --checkpoint=.1000 . | xz --best > "$CPW"/images-$SHORTID.tar.xz
echo "Build completed successfully."


