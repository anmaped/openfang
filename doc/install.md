# Installation

The modification of Xiaomi Dafang's firmware is composed of three steps. The first one consists on flashing a new bootloader capable to load all the ROOTFS directories entirely from the SD card partition, the second one consists on partitioning the SD cards into two partitions and writing the new ROOTFS in one of the partitions, and the third one consists on resizing the ROOTFS filesystem.

- [Download and flash the bootloader](#dowload-bootloader)
- [Download and write the rootfs image](#download-rootfs)
- [Resize the written filesystem](#resize-the-rootfs-image)


## Dowload bootloader

:heavy_exclamation_mark:WARNING!! You may brick your device in this step. Be cautious.

The bootloader for Xiaomi Dafang 128M can be found in this [Link](https://github.com/Dafang-Hacks/uboot/raw/master/compiled_bootloader/dafang_128mb_v1.bin)

### Flash the bootloader

Please follow the instructions [here](https://github.com/EliasKotlyar/Xiaomi-Dafang-Hacks/blob/master/hacks/flashinguboot.md).

## Download rootfs

Xiaomi Dafang 128M [Link](https://github.com/anmaped/openfang/releases/download/rc1_01/rootfs.ext2-706829f.xz)

### Flash rootfs using windows 10

diskpart or disk management wizard can be used to make partitions.

<img src="/doc/use_diskpart.png" width="600">

To flash the image in the first partition of the sd card use the tool DiskImage 1.6 ([link](http://www.roadkil.net/program.php/P12/Disk%20Image)).

<img src="/doc/towrite.png" width="300">


### Flash rootfs using linux

```
fdisk /dev/sdb
```
Use `p` to see the list of partitions and `n` to create a new one.

After creating all the partitions we should see something like that

```
Command (m for help): p
Disk /dev/sdb: 58.4 GiB, 62730010624 bytes, 122519552 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000001

Device     Boot    Start       End   Sectors  Size Id Type
/dev/sdb1           2048  10242047  10240000  4.9G  7 HPFS/NTFS/exFAT
/dev/sdb2       10242048 122517503 112275456 53.6G  7 HPFS/NTFS/exFAT
```

The first partition is the boot partition and is where we should store the rootfs. For that use the command

```
dd if=/path/to/image/rootfs.ext3 of=/dev/sdb1
```
to flash the rootfs image.


### Resize the rootfs image

The rootfs image is smaller than the available partition where we have written the rootfs directories. To be able to use all the available space we allocated for the partition, we have to resize the filesystem using the command
```
resize2fs /dev/root
```
It will take a short while. Note that the time it takes depends of the size of the partition you are resizing.

To format the second partition to exFat or Fat32, we can use the command

```
mkfs.exfat /dev/partition
```
to format the device as an exfat filesystem or
```
mkfs.fat -F 32 /dev/partition
```
to create a fat32 filesystem.
