## Openfang Mqtt configuration & Home Assistant Integration

Mqtt and some binary files posted in this project have been ported from Dafang Hack and this is the steps explaining how to do it.

### Library requirements
we need some new links to current library until someone do not recompile the programs again.(in Openfang there are links like libm.so.1 but we need links terminated with 0)

```links
ln -s /lib/libm-1.0.12.so /lib/libm.so.0 
ln -s /lib/libuClibc-1.0.12.so /lib/libc.so.0
ln -s /lib/libpthread-1.0.12.so /lib/libpthread.so.0
ln -s /lib/librt-1.0.12.so /lib/librt.so.0
```

### Bin's ported
We ported the following files from Dafang in bin folder with 755 permisions:
```files
bin/busybox


chmod 755 bin/*
```

mosquitto_sub, mosquitto_pub and getImages were already in Openfang.

### MQTT config file
We ported this file from config/mqtt.conf.dist and we need to rename to config/mqtt.conf. Changing the paths and our configuration.

export LD_LIBRARY_PATH='/thirdlib:/system/lib:/system/sdcard/lib' for export LD_LIBRARY_PATH='/thirdlib:/lib:/opt/lib'
	
 Variables: (USER,PASS, HOST, PORT, LOCATION, DEVICE)    

### Autostart files
We need to create mqtt-control and mqtt-status files as copy of other autostart files and give it 755 permisions:
```autostart
/config/autostart/mqtt-control
/config/autostart/mqtt-status

chmod 755 config/autostart/mqtt-*
```

### Controlscripts files
We need to port mqtt-control.sh and mqtt-status.sh files and give it 755 permisions:
```controlscripts
/controlscripts/mqtt-control.sh
/controlscripts/mqtt-status.sh

chmod 755 config/autostart/mqtt-*
```
then we need to change all entries "/system/sdcard" for "/opt"

### Scripts files
We need to port mqtt-control.sh, mqtt-status.sh and mqtt-status-interval.sh files and give it 755 permisions:
```scripts
/scripts/mqtt-control.sh
/scripts/mqtt-status.sh
/scripts/mqtt-status-interval.sh

chmod 755 config/autostart/mqtt-*
```
then we need to change all entries "/system/sdcard" for "/opt"
in mqtt-status-interval.sh we need to replace "/system/sdcard/bin/setconf" for "/usr/bin/setconf"

### Common_functions.sh correction
in comon_functions.sh
	replace all "/system/sdcard" for "/opt"
```common_functions
	correct line 93 status=$(getgpioled $(nvram get 2860 yellow_pin)) for status=$(getgpioled $(nvram get 2860 yellow_led_pin))
 ```
 
 ### Home Assistant
 Autodiscovery creates some deveices that we don't have implemented and I disabled it. To create configuration add the following lines acording your configuration:
 
 in configuration.yaml
 ```configuration.yaml
 camera:
   - platform: ffmpeg
    name: <openfang1> ffmpeg
    input: -rtsp_transport tcp -i rtsp://<your_ip>:8554/unicast
    
 binary_sensor:
  - platform: mqtt
    name: "openfang1_motion_sensor"
    state_topic: "Home/openfang1/motion_sensor"
 
 sensor:
  - platform: mqtt
    name: "openfang1"
    state_topic: "Home/openfang1"
    unit_of_measurement: "%"
    
  - platform: mqtt
    name: "openfang1_light_sensor"
    state_topic: "Home/openfang1/brightness"
    unit_of_measurement: "%"
  
  switch:
  - platform: mqtt
    name: "openfang1_blue_led"
    state_topic: "Home/openfang1/leds/blue"
    command_topic: "Home/openfang1/leds/blue/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false

  - platform: mqtt
    name: "openfang1_yellow_led"
    state_topic: "Home/openfang1/leds/yellow"
    command_topic: "Home/openfang1/leds/yellow/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false
    
  - platform: mqtt
    name: "openfang1_ir_led"
    state_topic: "Home/openfang1/leds/ir"
    command_topic: "Home/openfang1/leds/ir/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false
    
  - platform: mqtt
    name: "openfang1_ir_cut"
    state_topic: "Home/openfang1/ir_cut"
    command_topic: "Home/openfang1/ir_cut/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false
    
  - platform: mqtt
    name: "openfang1_night_mode"
    state_topic: "Home/openfang1/night_mode"
    command_topic: "Home/openfang1/night_mode/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false

  - platform: mqtt
    name: "openfang1_night_mode_auto"
    state_topic: "Home/openfang1/night_mode/auto"
    command_topic: "Home/openfang1/night_mode/auto/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false

  - platform: mqtt
    name: "openfang1_motion_detection"
    state_topic: "Home/openfang1/motion/detection"
    command_topic: "Home/openfang1/motion/detection/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false

  - platform: mqtt
    name: "openfang1_motion_tracking"
    state_topic: "Home/openfang1/motion/tracking"
    command_topic: "Home/openfang1/motion/tracking/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false
    
  - platform: mqtt
    name: "openfang1_remount_sdcard"
    state_topic: "Home/openfang1/remount_sdcard"
    command_topic: "Home/openfang1/remount_sdcard/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false  
    
  - platform: mqtt
    name: "openfang1_reboot"
    state_topic: "Home/openfang1/reboot"
    command_topic: "Home/openfang1/reboot/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false
    
  - platform: mqtt
    name: "openfang1_snapshot"
    state_topic: "Home/openfang1/snapshot"
    command_topic: "Home/openfang1/snapshot/set"
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: false  
    
  - platform: mqtt
    name: "openfang1 RTSP"
    state_topic: "Home/openfang1/status"
    command_topic: "Home/openfang1/set"
    payload_on: "h264_noseg_start"
    payload_off: "rtsp_stop"
    optimistic: false
 
  ```
  In customize:
  ```customize.yaml
 homeassistant:
  customize:
    sensor.openfang1:
      friendly_name: openfang1      
      icon: mdi-clipboard-text
    switch.openfang1_blue_led:
      friendly_name: openfang1 Blue LED      
      icon: mdi:led-on
    switch.openfang1_yellow_led:
      friendly_name: openfang1 Yellow LED      
      icon: mdi:led-on      
    switch.openfang1_ir_led:
      friendly_name: openfang1 Ir LED      
      icon: mdi:led-on
    switch.openfang1_ir_cut:
      friendly_name: openfang1 Ir CUT      
      icon: mdi:image-filter-black-white
    sensor.openfang1_light_sensor:
      friendly_name: openfang1 Light Sensor      
      icon: mdi:brightness-5
    switch.openfang1_night_mode:
      friendly_name: openfang1 Night Mode      
      icon: mdi:weather-night 
    switch.openfang1_night_mode_auto:
      friendly_name: openfang1 Auto-Night Mode      
      icon: mdi:weather-night  
    switch.openfang1_motion_detection:
      friendly_name: openfang1 Motion Detection      
      icon: mdi:run
    binary_sensor.openfang1_motion_sensor:
      friendly_name: openfang1 Motion Sensor
      icon: mdi:run
    switch.openfang1_remount_sdcard:
      friendly_name: openfang1 Remount SD-Card      
    switch.openfang1_reboot:
      friendly_name: openfang1 Reboot      
    switch.openfang1_snapshot:
      friendly_name: openfang1 Snapshot  
   ```
   
  In lovelace:
  ```lovelace.yaml
 cards:
  - type: picture-glance
    entities: []
    Title: openfang1
    camera_image: camera.openfang1_ffmpeg
  
  - type: entities
    title: Switchs
    show_header_toggle: false
    entities:
    - switch.openfang1_blue_led
    - switch.openfang1_yellow_led 
    - switch.openfang1_ir_led
    - type: divider
    - switch.openfang1_ir_cut
    - type: divider 
    - sensor.openfang1_light_sensor
    - sensor.openfang1
    - type: divider
    - switch.openfang1_night_mode
    - switch.openfang1_night_mode_auto
    - type: divider
    - switch.openfang1_motion_detection
    - binary_sensor.openfang1_motion_sensor
    - type: divider    
    - switch.openfang1_rtsp 
    - switch.openfang1_h264_rtsp_server
    - switch.openfang1_mjpeg_rtsp_server
    - type: divider
    - switch.openfang1_motion_tracking
    - switch.openfang1_motion_send_mail
    - switch.openfang1_motion_send_telegram 
    - type: divider
    - switch.openfang1_remount_sdcard
    - switch.openfang1_reboot
    - switch.openfang1_snapshot
    - type: divider    
    - camera.openfang1_motion_snapshot    
  ```

At the end we upladed some of this mqtt files at this github but take special attention in device name and your configuration.
 
