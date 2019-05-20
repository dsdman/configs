#!/bin/sh

#removes traces of old kernels from system version
echo "Removing kernel version " "$1"

rm -rfvi /boot/*"$1"*
rm -rfv /usr/src/*"$1"*
rm -rfv /var/tmp/genkernel/*"$1"*
rm -rfv /lib/modules/*"$1"*
