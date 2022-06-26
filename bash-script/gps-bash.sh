#!/bin/bash

# FILE = /home/star/Smart-video/Acc--Crash-detected.avi
# FILE1 = /home/star/vish
while true
do

if test -d "/home/pi/gps-data"
then
sleep 0.005

else
if [  -f "/home/pi/Smart-Video/gps-activate.txt" ]
then
	cd /home/pi
	mkdir gps-data
	./gps_info > /home/pi/gps-data/gps-data.txt
fi


fi

done
