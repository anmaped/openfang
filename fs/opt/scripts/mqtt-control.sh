#!/bin/sh

. /opt/config/mqtt.conf
. /opt/scripts/common_functions.sh

killall mosquitto_sub 2> /dev/null
killall mosquitto_sub.bin 2> /dev/null

/usr/bin/mosquitto_sub -v -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/# -t "${LOCATION}/set" ${MOSQUITTOOPTS} | while read -r line ; do
  echo $line
  case $line in
#    "${LOCATION}/set announce")
#      /opt/scripts/mqtt-autodiscovery.sh
#      ;;
#    "${TOPIC}/set help")
#      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/help ${MOSQUITTOOPTS} -m "possible commands: configured topic + Yellow_LED/set on/off, configured topic + Blue_LED/set on/off, configured topic + set with the following commands: status, $(grep \)$ /opt/www/cgi-bin/action.cgi | grep -v '[=*]' | sed -e "s/ //g" | grep -v -E '(osd|setldr|settz|showlog)' | sed -e "s/)//g")"
#    ;;

    "${TOPIC}/set status")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/ ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(/opt/scripts/mqtt-status.sh)"
    ;;

    "${TOPIC}/leds/blue")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/blue ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(blue_led status)"
    ;;

    "${TOPIC}/leds/blue/set ON")
      blue_led on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/blue ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(blue_led status)"
    ;;

    "${TOPIC}/leds/blue/set OFF")
      blue_led off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/blue ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS}  -m "$(blue_led status)"
    ;;

    "${TOPIC}/leds/yellow")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/yellow ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(yellow_led status)"
    ;;

    "${TOPIC}/leds/yellow/set ON")
      yellow_led on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/yellow ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(yellow_led status)"
    ;;

    "${TOPIC}/leds/yellow/set OFF")
      yellow_led off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/yellow ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(yellow_led status)"
    ;;

    "${TOPIC}/leds/ir")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/ir ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(ir_led status)"
    ;;

    "${TOPIC}/leds/ir/set ON")
      ir_led on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/ir ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(ir_led status)"
    ;;

    "${TOPIC}/leds/ir/set OFF")
      ir_led off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/ir ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(ir_led status)"
    ;;

    "${TOPIC}/ir_cut")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/ir_cut ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(ir_cut status)"
    ;;

    "${TOPIC}/ir_cut/set ON")
      ir_cut on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/ir_cut ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(ir_cut status)"
    ;;

    "${TOPIC}/ir_cut/set OFF")
      ir_cut off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/ir_cut ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(ir_cut status)"
    ;;

    "${TOPIC}/brightness")
      if [ "$SENDLDR" != "false" ]; then
        /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/brightness ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(ldr status)"
      fi
    ;;

    "${TOPIC}/night_mode")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/night_mode ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(night_mode status)"
    ;;

    "${TOPIC}/night_mode/set ON")
      night_mode on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/night_mode ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(night_mode status)"
    ;;

    "${TOPIC}/night_mode/set OFF")
      night_mode off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/night_mode ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(night_mode status)"
    ;;

    "${TOPIC}/night_mode/auto")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/night_mode/auto ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(auto_night_mode status)"
    ;;

    "${TOPIC}/night_mode/auto/set ON")
      auto_night_mode on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/night_mode/auto ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(auto_night_mode status)"
    ;;

    "${TOPIC}/night_mode/auto/set OFF")
      auto_night_mode off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/night_mode/auto ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(auto_night_mode status)"
    ;;
##############################################################################################################################################################
##################### VALIDATED
##############################################################################################################################################################

    "${TOPIC}/motion/detection")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/detection ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_detection status)"
    ;;

    "${TOPIC}/motion/detection/set ON")
      motion_detection on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/detection ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_detection status)"
    ;;

    "${TOPIC}/motion/detection/set OFF")
      motion_detection off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/detection ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_detection status)"
    ;;

    "${TOPIC}/motion/tracking")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/tracking ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_tracking status)"
    ;;

    "${TOPIC}/motion/tracking/set ON")
      motion_tracking on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/tracking ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_tracking status)"
    ;;

    "${TOPIC}/motion/tracking/set OFF")
      motion_tracking off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/tracking ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_tracking status)"
    ;;

    "${TOPIC}/remount_sdcard/set ON")
      remount_sdcard
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/remount_sdcard ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "Remounting the SD Card"
    ;;

    "${TOPIC}/reboot/set ON")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/reboot ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "Rebooting the System"
      reboot_system
    ;;

    "${TOPIC}/snapshot/set ON")
      snapshot
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/snapshot ${MOSQUITTOOPTS} ${MOSQUITTOPUBOPTS} -f "$filename"
    ;;
	
    "${TOPIC}/motion/send_mail")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/send_mail ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_send_mail status)"
    ;;

    "${TOPIC}/motion/send_mail/set ON")
      motion_send_mail on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/send_mail ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_send_mail status)"
    ;;

    "${TOPIC}/motion/send_mail/set OFF")
      motion_send_mail off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/send_mail ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_send_mail status)"
    ;;

   "${TOPIC}/motion/send_telegram")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/send_telegram ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_send_telegram status)"
    ;;

    "${TOPIC}/motion/send_telegram/set ON")
      motion_send_telegram on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/send_telegram ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_send_telegram status)"
    ;;

    "${TOPIC}/motion/send_telegram/set OFF")
      motion_send_telegram off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/send_telegram ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motion_send_telegram status)"
    ;;

    "${TOPIC}/motors/vertical/set up")
      motor up
	  MOTORSTATE=$(motor status vertical)
	  if [ `/opt/bin/setconf -g f` -eq 1 ]; then
		TARGET=$(busybox expr $MAX_Y - $MOTORSTATE)
	  else
		TARGET=$MOTORSTATE
	  fi
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motors/vertical ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$TARGET"
    ;;

    "${TOPIC}/motors/vertical/set down")
      motor down
	  MOTORSTATE=$(motor status vertical)
	  if [ `/opt/bin/setconf -g f` -eq 1 ]; then
		TARGET=$(busybox expr $MAX_Y - $MOTORSTATE)
	  else
		TARGET=$MOTORSTATE
	  fi	   
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motors/vertical ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$TARGET"
    ;;

    "${TOPIC}/motors/vertical/set "*)
      COMMAND=$(echo "$line" | awk '{print $2}')
      MOTORSTATE=$(motor status vertical)
      if [ -n "$COMMAND" ] && [ "$COMMAND" -eq "$COMMAND" ] 2>/dev/null; then   
        if [ `/opt/bin/setconf -g f` -eq 1 ]; then
          echo Changing motor from $COMMAND to $MOTORSTATE
          TARGET=$(busybox expr $MOTORSTATE + $COMMAND - $MAX_Y)
        else
          echo Changing motor from $MOTORSTATE to $COMMAND
          TARGET=$(busybox expr $COMMAND - $MOTORSTATE)
        fi
        echo Moving $TARGET
        if [ "$TARGET" -lt 0 ]; then
          motor down $(busybox expr $TARGET \* -1)
        else
          motor up $TARGET
        fi
      else
        echo Requested $COMMAND is not a number
      fi
    ;;
	
    "${TOPIC}/motors/horizontal/set left")
      motor left
	  MOTORSTATE=$(motor status horizontal)
	  if [ `/opt/bin/setconf -g f` -eq 1 ]; then
		TARGET=$(busybox expr $MAX_X - $MOTORSTATE)
	  else
		TARGET=$MOTORSTATE
	  fi
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motors/horizontal ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$TARGET"
    ;;

    "${TOPIC}/motors/horizontal/set right")
      motor right
	  MOTORSTATE=$(motor status horizontal)
	  if [ `/opt/bin/setconf -g f` -eq 1 ]; then
		TARGET=$(busybox expr $MAX_X - $MOTORSTATE)
	  else
		TARGET=$MOTORSTATE
	  fi
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motors/horizontal ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$TARGET"
    ;;

    "${TOPIC}/motors/horizontal/set "*)
      COMMAND=$(echo "$line" | awk '{print $2}')
      MOTORSTATE=$(motor status horizontal)
      if [ -n "$COMMAND" ] && [ "$COMMAND" -eq "$COMMAND" ] 2>/dev/null; then
        if [ `/opt/bin/setconf -g f` -eq 1 ]; then
          echo Changing motor from $COMMAND to $MOTORSTATE
          TARGET=$(busybox expr $MOTORSTATE + $COMMAND - $MAX_X)
        else
          echo Changing motor from $MOTORSTATE to $COMMAND
          TARGET=$(busybox expr $COMMAND - $MOTORSTATE)
        fi
        echo Moving $TARGET
        if [ "$TARGET" -lt 0 ]; then
          motor left $(busybox expr $TARGET \* -1)
        else
          motor right $TARGET
        fi
      else
        echo Requested $COMMAND is not a number
      fi
    ;;

    "${TOPIC}/motors/set calibrate")
      motor reset_pos_count
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motors ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(motor status horizontal)"
    ;;

    "${TOPIC}/rtsp_h264_server")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/rtsp_h264_server ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(rtsp_h264_server status)"
    ;;

    "${TOPIC}/rtsp_h264_server/set ON")
      rtsp_h264_server on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/rtsp_h264_server ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(rtsp_h264_server status)"
    ;;

    "${TOPIC}/rtsp_h264_server/set OFF")
      rtsp_h264_server off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/rtsp_h264_server ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(rtsp_h264_server status)"
    ;;

    "${TOPIC}/rtsp_mjpeg_server")
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/rtsp_mjpeg_server ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(rtsp_mjpeg_server status)"
    ;;

    "${TOPIC}/rtsp_mjpeg_server/set ON")
      rtsp_mjpeg_server on
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/rtsp_mjpeg_server ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(rtsp_mjpeg_server status)"
    ;;

    "${TOPIC}/rtsp_mjpeg_server/set OFF")
      rtsp_mjpeg_server off
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/rtsp_mjpeg_server ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(rtsp_mjpeg_server status)"
    ;;

    "${TOPIC}/set "*)
      COMMAND=$(echo "$line" | awk '{print $2}')
      #echo "$COMMAND"
      F_cmd="${COMMAND}" /opt/www/cgi-bin/action.cgi -o /dev/null 2>/dev/null
      if [ $? -eq 0 ]; then
        /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}/${COMMAND}" ${MOSQUITTOOPTS} -m "OK (this means: action.cgi invoke with parameter ${COMMAND}, nothing more, nothing less)"
      else
        /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}/error" ${MOSQUITTOOPTS} -m "An error occured when executing ${line}"
      fi
      # Publish updated states
      /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}" ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -m "$(/opt/scripts/mqtt-status.sh)"
    ;;
  esac
done
