#!/bin/sh

# script for downloading youtube videos as mp3 and then playing it with mpd

# download it
url="$1"
url=$(echo $url | sed s/\&/\ /g | cut -d\  -f 1)
#echo "$url" | dmenu > url.txt
tmp=$(youtube-dl --get-filename -i --extract-audio --embed-thumbnail --audio-format mp3 --add-metadata --audio-quality 0 $url)
youtube-dl -i --extract-audio --embed-thumbnail --audio-format mp3 --add-metadata --audio-quality 0 $url

# move it to tmp directory
filename="${tmp%.*}"
file="$filename".mp3
mv -fv "$file" /media/windows/Users/dsd123/Music/tmp/

# play it
mpc update && mpc clear && sleep 1 && mpc add "tmp/$file" && mpc play
