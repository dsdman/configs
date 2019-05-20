#!/bin/sh

# Uses dmenu to prompt for secondary monitor.
# Gives a choice of setting displays up, then sets it up with xrandr

# setting dmenu variable for font
font="Inconsolata:size=14"

# Get a list of displays, and prompt user for secondary display
displays=$(xrandr | grep -w "connected" | awk '{print $1}')
count=$(echo "${displays}" | wc -l)
[ "$count" = "1" ] && exit 0
secondary=$(echo "${displays}" | dmenu -i -fn $font -p "Which is your second display?")
[ "$secondary" = "" ] && exit 0
# The primary display is the one not chosen (given by grep -v)
primary=$(echo "${displays}" | grep -v "${secondary}")

# Prompt for option
answer=$(printf "Extended displays\nDuplicated displays\nPrimary display only\nSecondary display only" | dmenu -i -fn $font  -p "Choose an option.")
[ "$answer" = "" ] && exit 0

# Run the appropriate xrandr command
if [ "$answer" = "Extended displays" ]; then
	xrandr --output "${primary}" --primary --auto --output "${secondary}" --auto --right-of "${primary}"
elif [ "$answer" = "Duplicated displays" ]; then
	xrandr --output "${primary}" --primary --auto --output "${secondary}" --auto --same-as "${primary}"
elif [ "$answer" = "Primary display only" ]; then
	xrandr --output "${secondary}" --off --output "${primary}" --auto --primary
elif [ "$answer" = "Secondary display only" ]; then
	xrandr --output "${primary}" --off --output "${secondary}" --auto --primary
else
	echo "This shouldn't happen."
fi
