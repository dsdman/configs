#!/bin/sh

#locks screen, then suspends system

sudo slock &
sleep 1.00s && sudo s2ram
