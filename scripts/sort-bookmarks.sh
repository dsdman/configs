#!/bin/sh

# sorts bookmarks in surf

cp -rfv bookmarks bookmarks-bak
cp -rfv bookmarks bm
cat bm | sed s?https:\/\/?? | sed s?www\.?? | sed s?http:\/\/?? | sort > bookmarks
rm bm
