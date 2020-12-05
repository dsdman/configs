#!/bin/sh

# this is a script for my dwm status bar

while true; do
  # networking
  ip_string=$(wpa_cli status | grep 'ip_address=*' | sed s/ip_address=//g) 
  network_string="[IP $ip_string]"

  # battery
  pre="Bat"
  state=$(cat /sys/class/power_supply/BAT1/status)
  charge=$(cat /sys/class/power_supply/BAT1/capacity)
  [ "$state" = "Full" ] && bat_string="[$pre $charge%]"
  [ "$state" = "Discharging" ] && bat_string="[$pre -$charge%]"
  [ "$state" = "Charging" ] && bat_string="[$pre +$charge%]"
  
  # volume
  vol_string=$(amixer -D pulse sget Master | grep 'Left:' | awk -F'[][]' '{ print $2 }')
  vol_string="[Vol $vol_string]"

  # date
  date_string="[$(date +%a\ %D\ %l:%M:%S%p)]"

  # mpc
  not_playing=$(mpc | head -1 | grep volume)
  if [ "$not_playing" = "" ]; then
    title="$(mpc | head -1)"
    time="$(mpc | sed -n 2p | awk '{ print $3 }')"
    mpc_string="[$title $time]"
  else
    mpc_string=""
  fi
  
  #Put it all together in one string and feed to xsetroot to display
  status="$mpc_string $network_string $bat_string $vol_string $date_string"
  xsetroot -name "$status"
  
  #Update every 2 seconds
  sleep 2
done
