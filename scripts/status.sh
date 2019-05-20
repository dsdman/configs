#!/bin/sh

#this is a script for my dwm status bar

while true; do
  ##### networking module ####
  ip_string=$(wpa_cli status | grep 'ip_address=*' | sed s/ip_address=//g) 
  ssid_string=$(wpa_cli status | grep -w 'ssid' | sed s/ssid=//g)
  network_string=" ip $ip_string wifi $ssid_string"

  ##### battery module ####
  state=$(cat /sys/class/power_supply/BAT1/status)
  charge=$(cat /sys/class/power_supply/BAT1/capacity)
  [ "$state" = "Full" ] && bat_string="bat $charge%"
  [ "$state" = "Discharging" ] && bat_string="bat -$charge%"
  [ "$state" = "Charging" ] && bat_string="bat +$charge%"
  
  #### volume module ####
  vol_string=$(amixer -D pulse sget Master | grep 'Left:' | awk -F'[][]' '{ print $2 }')
  vol_string="vol $vol_string"

  #### date module ####
  date_string=$(date +%a\ %D\ %l:%M:%S%p)

  #Put all modules together in one string and feed to xsetroot to display
  status="$network_string | $bat_string | $vol_string | $date_string"
  xsetroot -name "$status"
  
  #Update every second
  sleep 1
done
