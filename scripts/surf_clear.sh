#!/bin/sh
ans=$(printf "Yes\nNo" | dmenu -i -p "Are you sure you want to clear cookies, certifications and cache (will restart surf)")
if [ "$ans" = "Yes" ]; then
  rm -rfv ~/.surf/cache/
  rm -rfv ~/.surf/cookies.txt
  rm -rfv ~/.surf/certificates/
  killall surf
  surf-open.sh duckduckgo.com
fi
