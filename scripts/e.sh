#!/bin/sh

# one-liner that edits a file in another terminal in the background
st -e "$EDITOR" "$1" > /dev/null 2>&1 &
