#!/bin/bash
docker save scaledcars-on-opendlv-on-opendlv-core-on-opendavinci-ubuntu-16.04-armhf-complete-lanefollowing | bzip2 | pv | ssh pi@192.168.43.200 'bunzip2 | docker load'
