#!/bin/sh

# copies url to systemwide clipboard

ans=$(printf "Clipboard\nPrimary" | dmenu -i -p "Copy to Clipboard selection or Primary selection?")
if [ "$ans" = "Clipboard" ]; then
  xsel | xsel -b
else
  xsel -b | xsel
fi
