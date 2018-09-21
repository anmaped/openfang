## Flash the bootloader

Please follow the instructions [here](https://github.com/EliasKotlyar/Xiaomi-Dafang-Hacks/blob/master/hacks/flashinguboot.md).

## Flash rootfs using windows 10

diskpart or disk management wizard can be used to make partitions.

<img src="/doc/use_diskpart.png" width="600">

To flash the image in the first partition of the sd card use the tool DiskImage 1.6 ([link](http://www.roadkil.net/program.php/P12/Disk%20Image)).

<img src="/doc/towrite.png" width="300">


## Flash rootfs using linux

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


## Resize the partition after the first boot

To use the full space of the partition please resize it using the command
```
resize2fs /dev/root
```
It will take some time depending of the size of the partition.

Format the second partition to exFat or Fat32 using the command

```
mkfs.exfat /dev/partition
```
or
```
mkfs.fat -F 32 /dev/partition
```
