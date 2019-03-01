#!/bin/sh

TAG=rc05_01

set -e

CPW=$(pwd)

DIR=_build
[ -d $DIR ] || { printf '%s does not exist!\n' "$DIR"; mkdir $DIR; }

date=$(date +"%Y-%m-%d %H:%M")
#ID="($(git rev-parse HEAD)) $date"
ID="$(git describe --tags)"
SHORTID=$(git rev-parse --short HEAD)

echo "$ID" > fs/opt/version

cp fs $DIR/ -r;

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
cp "$CPW"/config/buildroot.config ./.config
cp "$CPW"/config/busybox.config ./package/busybox
cp "$CPW"/config/uClibc-ng.config ./package/uclibc

[ -d "dl" ] || { mkdir dl; }

cp ../../kernel-3.10.14.tar.xz dl/
cp ../../uboot-v2013.07.tar.xz dl/

WDIR=$CPW/$DIR/buildroot-$BUILDROOT_VERSION

# Patch buildroot if gcc >= 5
#
GCCVER=$(gcc -dumpversion | cut -d'.' -f1)
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

# copy custom openfang packages to buildroot directory
rm -r "$WDIR"/package/ffmpeg # use updated package version instead
#rm -r "$WDIR"/package/python # use updated package version instead
#rm -r "$WDIR"/package/uclibc # use updated package version instead
cp "$CPW"/buildroot/* . -rf

make

#
# compile different versions of uboot
#
[ -f "output/images/u-boot-lzo-with-spl.bin" ] && mv output/images/u-boot-lzo-with-spl.bin output/images/u-boot-lzo-with-spl_t20_128M.bin

# change uboot configuration
sed -i "s/BR2_TARGET_UBOOT_BOARDNAME=.*/BR2_TARGET_UBOOT_BOARDNAME=\"isvp_t20_sfcnor\"/g" .config

make uboot-dirclean
make uboot

[ -f "output/images/u-boot-lzo-with-spl.bin" ] && mv output/images/u-boot-lzo-with-spl.bin output/images/u-boot-lzo-with-spl_t20_64M.bin

# end uboot compilation


# constructs release with git hash label
echo "Compressing toolchain..."
tar -c -C "$WDIR"/output/host --transform s/./mipsel-ingenic-linux-uclibc/ --checkpoint=.1000 .  | xz --best > "$CPW"/toolchain-$SHORTID.tar.xz
echo "Compressing rootfs images..."
tar -c -C "$WDIR"/output/images --transform s/./openfang-images/ --checkpoint=.1000 . | xz --best > "$CPW"/images-$SHORTID.tar.xz
echo "Build completed successfully."


