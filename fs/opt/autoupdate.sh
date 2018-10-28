#!/bin/bash

set -e

VERSION=
TAG=
ID=

case "$1" in
version)
  echo "$VERSION";
  ;;

*)

# check autoupdate version
old=/opt/autoupdate.sh
new=/opt/autoupdate.sh_new

curl -L --insecure https://github.com/anmaped/openfang/releases/download/$TAG/autoupdate.sh -o $new

date_b=$(cat /opt/version  | cut -d ")" -f 2)

NEW_VER=`sh /opt/autoupdate.sh_new version`

VERSION_=`date -d "$VERSION" +%s`
NEW_VER=`date -d "$NEW_VER" +%s`
echo $VERSION_
echo $NEW_VER

if [[ "$VERSION_" -lt "$NEW_VER" ]] ;
then
    echo "Updating..."
    . /opt/autoupdate.sh_new
    exit 0
fi

date_b=`date -d "$date_b" +%s`
echo $VERSION_
echo $date_b

[[ "$VERSION_" -le "$date_b" ]] && { echo "nothing to do."; exit 0; }

cd /mnt/mmcblk0p2

curl -L --insecure https://github.com/anmaped/openfang/releases/download/$TAG/images-$ID.tar.xz -o images-$ID.tar.xz

xz -cd images-$ID.tar.xz | tar xvf -

cp /bin/dd .
cp /sbin/reboot .

./dd if=openfang-images/rootfs.ext2 of=/dev/mmcblk0p1

./reboot

esac

