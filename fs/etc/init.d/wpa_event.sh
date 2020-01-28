#!/bin/sh

case "$2" in
    CONNECTED)
	PID=`/bin/pidof udhcpc`;
        /bin/kill -SIGUSR1 $PID;
	;;
    DISCONNECTED)
        ;;
esac
