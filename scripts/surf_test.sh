SURF_WINDOW=$1
URL=$(xprop -id "$SURF_WINDOW" _SURF_URI | cut -d '"' -f 2)
echo $URL | dmenu -p test
