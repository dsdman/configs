#!/bin/sh

# I use this for switching pulseaudio output device from HDMI to laptop speakers or vice-versa

switch_dev=$(pactl list short sinks | grep -v RUNNING | cut -f 1)
pacmd set-default-sink "$switch_dev"
echo "set default sink to $switch_dev"

pactl list short sink-inputs | while read stream; do
  streamId=$(echo $stream | cut "-d " -f 1)
  echo "moving stream $streamId"
  pactl move-sink-input "$streamId" "$switch_dev"
done
