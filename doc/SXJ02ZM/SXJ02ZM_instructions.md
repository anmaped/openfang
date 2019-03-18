# Introduction and requirements
<img src="/doc/SXJ02ZM/img/xiaomi_mijia_2018.jpg" width="300">

The Xiaomi Mijia 1080p model v3 is the snow-white model which was released in 2018, and has a T20L SOC with 64MB RAM
Unfortunately there is no known Firmware-exploit which allows us to easily edit the firmware to gain ssh-access to the camera.
In order to get OpenFang on the camera, we need to program a custom bootloader to the camera by using a ch341a programmer.



### Requirements
You will need:
- USB CH341A programmer and Programmer Testing Clip SOP 8 <img src="/doc/SXJ02ZM/img/c341a_clamp.jpg" width="400">
- Soldering iron
- Flux
- Screwdrivers
- a piece of plastic foil from any plastic package



## 1. Case disassembly
- Remove the back of the camera, starting from the botton.
<img src="/doc/SXJ02ZM/img/opencase.jpg" width="300">



- After the back has been losen, carefully remove the speaker/mic connector before removing the back.
<img src="/doc/SXJ02ZM/img/speakercable.jpg" width="300">



- Unscrew the 3 black screws to separate the board from the front cover. Leave the middle screws in there!
<img src="/doc/SXJ02ZM/img/screws.jpg" width="300">



- After removing the screws, carefully remove the 2 frontpanel LED connectors.
<img src="/doc/SXJ02ZM/img/frontpanelcables.jpg" width="300">




## 2. (De)soldering and preparing to flash
### Desoldering
- Desolder the VCC leg (Leg 8) of the SOP8 chip.
<img src="/doc/SXJ02ZM/img/W25Q128JVSQ.jpg" width="300">
<img src="/doc/SXJ02ZM/img/desolderedleg.jpg" width="300">




- Put a piece of plastic foil between the desoldered leg and the PCB. We do this to avoid powering-up the whole PCB through the chip+programmer, as the chip would net be flashable like this.
<img src="/doc/SXJ02ZM/img/foilundersop.jpg" width="300">



### Conecting the SOP to the programmer
- Put the programmer clip tight on the SOP, having in mind that the red cable is always for PIN1. Make sure it sits tight on the SOP.
<img src="/doc/SXJ02ZM/img/clamponsop.jpg" width="300">
<img src="/doc/SXJ02ZM/img/clamponsop2.jpg" width="300">




- Connect the clip cable to the programmer as seen in here.
<img src="/doc/SXJ02ZM/img/c341a_position.jpg" width="300">


- Plug in the programer to the computer and start the flashing process.



## 3. Flashing the SOP (Windows / Mac / Linux)
### Downloading the needed files
- Download the latest release package from [Link](https://github.com/anmaped/openfang/releases) and extract it somewhere.
- Download the proper flashing software from the tools directory. It is recommended to flash the SOP under Linux / MacOS as I never had success in flashing it with windows! Feel free to report otherwise.



#### Flashing under MacOS / Linux
- Use ch341prog to first erase the SOP: ``` ./ch341prog -e ```
- Use ch341prog to write the custom bootloader to the SOP: ``` ./ch341prog -w u-boot-lzo-with-spl_t20_64M.bin ```  Caution: flash the 64M binary file!



#### Flashing under Windows
- Download the "CH341A programm v1.29.zip" from the tools directory, extract it somewhere and run it.
- you will get an error message, which you can ignore: 
<img src="/doc/SXJ02ZM/img/windows_flasher_1.png" width="300">


- After clicking on connect, you will be prompted to select your flash memory. Select any of the two. 
<img src="/doc/SXJ02ZM/img/windows_flasher_2.png" width="300">


- Click on erase at the top icons and wait for the process to finish. 
<img src="/doc/SXJ02ZM/img/windows_flasher_3.png" width="300">


- If the erasing has been completed, you will be prompted with a message. 
<img src="/doc/SXJ02ZM/img/windows_flasher_4.png" width="300">


- Click on read at the top icons and make sure that everything is set to FF. 
<img src="/doc/SXJ02ZM/img/windows_flasher_5.png" width="300">


- Click on File -> open and select the u-boot-lzo-with-spl_t20_64M.bin file. Do not select the 128m one. 
<img src="/doc/SXJ02ZM/img/windows_flasher_6.png" width="300">


- After the flashing, click on read at the top icons and see is something was written to the flash. 
<img src="/doc/SXJ02ZM/img/windows_flasher_7.png" width="300">



## 4. Preparing the SD-Card (Windows / Mac / Linux)
### Flash rootfs using Windows
- Download and install any partitioning software. My favourite freeware for this is Active Partition Manager.
- On active partiton manager erase all partitions of the sd-card.
- Create one NTFS-Partition with about 4GB (4096 mbytes)
- Select edit params of the partition and make sure you assign a drive letter to it.
- Use the rest of the unallocated space to create one exFat partition.
- Select edit params of the partition and make sure you assign a drive letter to it.
- Open DiskImage 1.6 ([link](http://www.roadkil.net/program.php/P12/Disk%20Image)) and flash the rootfs.ext2 imagefile onto the 4GB NTFS partition you created. <img src="/doc/img/towrite.png" width="300">



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
dd if=/path/to/image/rootfs.ext2 of=/dev/sdb1
```
to flash the rootfs image.



### Flash rootfs using MacOS
Follow the same instructions for Linux.



## 5. Configuring the cam
If everything went well so far, your cam's front LED should be flashing in different colors, and after 1-2 minutes stay orange. If this is the case, you can now connect your computer to the temporary created hotspot from the camera for the initial setup.
When you connect to the OpenFang access point, you can open a browser and call https://192.168.14.1 in the browser to access the panel. Use admin / admin in order to log on. If you get an error saying conection_refused, please proceed with this guide. Else please jump to "Resize the rootfs image".




### Create a certificate and boot the webserice in case of problems.
- Log on a SSH shell to 192.168.14.1 with username admin / admin.
- On the shell type ``` su ``` press return.
- Afterwards type ``` openssl req -new -x509 -keyout /etc/ssl/lighttpd.pem -out /etc/ssl/lighttpd.pem -days 365 -nodes -subj "/C=DE/ST=.../L=.../O=.../OU=.../CN=.../emailAddress=..." ``` press return.
- Type ``` cd /etc/init.d/ ``` press return.
- Finally type ``` ./lighttpd.sh start ``` press return.
- The webservice should now start and you should be able to log on.



### Resize the rootfs image
The rootfs image is smaller than the available partition where we have written the rootfs directories. To be able to use all the available space we allocated for the partition, we have to resize the filesystem.

- Log on a SSH shell to 192.168.14.1 with username admin / admin.
- On the shell type ``` su ``` press return.
- Type ``` resize2fs /dev/mmcblk0p1 ``` press return.
- It will take a short while. Note that the time it takes depends of the size of the partition you are resizing.



### Confifg the cam on the WebUi
- Go to Settings and select the MOdel of the cam (in this case the Mijia 2018)
- Go to wireless in the settings, select the mode and insert your home router's network credentials. Don't chose the type AP, as it will create an AP and this is not what we want.

