#!/bin/sh

# prerequists:
# apt-get install -y make libglfw3-dev pkg-config build-essential

# cd'ing to the directory where the script is located
SCRIPT_DIR=$(dirname "$0")
echo moving to $SCRIPT_DIR
cd "$SCRIPT_DIR"

# Tell x-server to use the touchscreen display, also needed for ImGui in the app to work
export DISPLAY=':0'

# Setting language locales
export LC_CTYPE=en_US.UTF-8
export LC_ALL=en_US.UTF-8

xinit /root/video-doorbell/app/app -- :0 -nocursor &
# Keep screen on
xset -dpms     # Disable DPMS (Energy Star) features
xset s off     # Disable screensaver
xset s noblank # Don't blank video device

# Orient display vertical
xrandr -o right

# Making sure that the touch part of the touchscreen is also rotated
# Syntax: xinput map-to-output <device> <output name>
# find <device> with:		xrandr --listmonitors
# find <output name> with:	xinput
xinput map-to-output 11 eDP-1
