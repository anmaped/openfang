#!/bin/sh
. /opt/config/mqtt.conf
. /opt/scripts/common_functions.sh

if [ "$STATUSINTERVAL" -lt 30 ]; then
  STATUSINTERVAL=30
fi

while true
do
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/blue ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(blue_led status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/yellow ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(yellow_led status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/leds/ir ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(ir_led status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/ir_cut ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(ir_cut status)"
  if [ "$SENDLDR" != "false" ]; then
    /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/brightness ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(ldr status)"
  fi
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/rtsp_h264_server ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(rtsp_h264_server status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/rtsp_mjpeg_server ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(rtsp_mjpeg_server status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/night_mode ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(night_mode status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/night_mode/auto ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(auto_night_mode status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/detection ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(motion_detection status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/send_mail ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(motion_send_mail status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/send_telegram ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(motion_send_telegram status)"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motion/tracking ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$(motion_tracking status)"
  MOTORSTATE=$(motor status vertical)
  if [ `/opt/bin/setconf -g f` -eq 1 ]; then
    TARGET=$(busybox expr $MAX_Y - $MOTORSTATE)
  else
    TARGET=$MOTORSTATE
  fi  
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motors/vertical ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m  "$TARGET"
  MOTORSTATE=$(motor status horizontal)
  if [ `/opt/bin/setconf -g f` -eq 1 ]; then
    TARGET=$(busybox expr $MAX_X - $MOTORSTATE)
  else
    TARGET=$MOTORSTATE
  fi
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}"/motors/horizontal ${MOSQUITTOPUBOPTS} ${MOSQUITTOOPTS} -r -m "$TARGET"
  /usr/bin/mosquitto_pub -h "$HOST" -p "$PORT" -u "$USER" -P "$PASS" -t "${TOPIC}" ${MOSQUITTOOPTS} ${MOSQUITTOPUBOPTS} -r -m "$(/opt/scripts/mqtt-status.sh)"
  sleep $STATUSINTERVAL
done
