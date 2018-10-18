### How does the openfang distribution work?
The openfang distribution consists of two parts:
1. The custom U-Boot that alters the original firmware bootloader to boot from a microsd-card. This part does not contain any custom software, its just allows us to boot from the microsd-card. You only have to do this once.
2. The ROOTFS, which contains the custom software. You will have to install them onto your microsd-card after you completed step 1. You can modify this part easily by changing the files on the microsd-card. 

### Does the openfang contain an RTSP-Server?
Yes, you can watch it through VLC Player.

### Does the openfang connect to the Xiaomi Servers?
No. It does not connect to anything.

### Does the openfang remove the original Firmware?
No. You can still boot the original Software, if you remove the SD-Card.

### Can I run the openfang suite without a microsd?
No that is not possible.

### Can I have full-HD support?
Yes, you can. After installing openfang, you can easily enable it in uEnv.txt.

See [custom bootloader installation](/doc/install.md) for more details.

## What features does the openfang distribution currently have?
- Full working RTSP with H264/MJPEG. Based on https://github.com/mpromonet/v4l2rtspserver
- SSH-Server(dropbear) with username: admin password: admin (use passwd to change it later)
- Webserver(lighttpd) with username: admin password: admin
- Image-Snap(Get Jpeg Image) 
- Horizontal/vertical motor rotation / move to center
- Turn on/off blue/yellow/IR LEDs/IR-Cut
- Audio recording/playing:
```
Playing Audio:
$ ossplay /your/wave/file.wav volume

Recording Audio:
$ ossrecord test.wav 

Recording Audio in mp3:
$ ossrecord -g 1000 -S -b16 -s48 - | lame -m j - filename.mp3
```
- Curl
- Mosquito (MQTT + websockets)
- Anything else you can compile yourself. There is a toolchain available [here](https://github.com/anmaped/openfang/releases).


### What features does the openfang distribution will have?
- Local h264 recording
- OpenCV

