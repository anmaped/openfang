#!/bin/sh

set -e

VERSION=
TAG=
ID=

WORKING_DIR=/mnt/mmcblk0p2

update() {

	echo "Update is starting..."

	# check if there is enough memory available
	[ $(free | head -3 | tail -1 | awk '{printf $4}') -gt 64000 ] || { echo "No free ram available! Try to disable HD mode first..."; exit 1; }

	xz -cd images-$ID.tar.xz | pv -s $(printf "%.0f\n" $(xz -l images-$ID.tar.xz | tail -n 1 | awk '{print $5}'))m | tar xvf -

	[ -d "/mnt/update" ] || { mkdir /mnt/update; }

	mount -t ext3 -o loop openfang-images/rootfs.ext2 /mnt/update

	cp -a /mnt/update/. /

	umount /mnt/update/

	echo "Update Successful."
}

case "$1" in
version)
	echo "$VERSION"
	;;

--id=*)
	ID_NEW=$(echo "$1" | cut -d "=" -f 2)
	echo "$ID_NEW"
	[ -f "$WORKING_DIR/images-$ID_NEW.tar.xz" ] && {
		cd $WORKING_DIR
		update
		exit 0
	}
	echo "File not found!"
	;;

*)

	# check autoupdate version
	old=/opt/autoupdate.sh
	new=/opt/autoupdate.sh_new

	curl -L --insecure https://github.com/anmaped/openfang/releases/download/$TAG/autoupdate.sh -o $new

	date_b=$(cat /opt/version | cut -d ")" -f 2)

	NEW_VER=$(sh /opt/autoupdate.sh_new version)

	VERSION_=$(date -d "$VERSION" +%s)
	NEW_VER_=$(date -d "$NEW_VER" +%s)

	if [ "$VERSION_" -lt "$NEW_VER_" ]; then
		echo "***"
		echo "$VERSION ($VERSION_) will be updated to $NEW_VER ($NEW_VER_)"
		echo "***"
		. /opt/autoupdate.sh_new
		exit 0
	fi

	date_b_=$(date -d "$date_b" +%s)

	echo "***"
	echo "Current version is $date_b ($date_b_) and will be updated to $VERSION ($VERSION_)"
	echo "***"

	[ "$VERSION_" -le "$date_b_" ] && {
		echo "nothing to do."
		exit 0
	}

	# check if there is a second partition
	[ -d "$WORKING_DIR" ] || { echo "A second partition is not available to extract files!"; exit 1; }

	# check if there is 500mb free space available
	[ $(df -k "$WORKING_DIR" | tail -n 1 | awk '{printf $4}') -gt 500000 ] || { echo "No space available!"; exit 1; }

	cd $WORKING_DIR

	curl -L --insecure https://github.com/anmaped/openfang/releases/download/$TAG/images-$ID.tar.xz -o images-$ID.tar.xz

	# check if file exists
	[ -f "images-$ID.tar.xz" ] || { echo "Something wrong happen with the downloaded file."; exit 1; }

	update
	;;

esac
