#!/bin/sh

set e

# check swap device argument
if [ -z "$1" ]
  then
    echo "No argument supplied"
fi


dd if=/dev/zero of="$1" bs=1024 count=262144 # 256Mb

chown root:root "$1"
chmod 0600 "$1"

mkswap "$1" # create swap file system

swapon "$1" # enable swap
