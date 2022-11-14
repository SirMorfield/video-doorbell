#!/bin/sh

# See ./installation.sh for prerequists

# echo each command before it is executed
set -o xtrace

# cd'ing to the directory where the script is located
SCRIPT_DIR=$(dirname "$0")
echo moving to $SCRIPT_DIR
cd "$SCRIPT_DIR"

# kill still runing instances of this app, and wait for it to exit
while $(pkill -0 --signal 9 app  2>/dev/null); do sleep 0.5; done
while $(pkill -0 --signal 9 xinit 2>/dev/null); do sleep 0.5; done
while $(pkill -0 --signal 9 Xorg 2>/dev/null); do sleep 0.5; done


# Optional: stop already running containers
if [ "$1" = "--docker" ]; then
# hot reload config of asterisk
docker exec -it asterisk asterisk -rx  'reload'
fi

# Build frontend app
PRODUCTION=1 make -C app || exit 1

# Tell x-server to use the touchscreen display, also needed for ImGui in the app to work
export DISPLAY=':0'

# Setting language locales
export LC_CTYPE=en_US.UTF-8
export LC_ALL=en_US.UTF-8

cd config-generator
# ts-node generator.ts || exit 1
cd ..

# install asterisk config files
cp asterisk/sip.conf /etc/asterisk/sip.conf
cp asterisk/extensions.conf /etc/asterisk/extensions.conf
asterisk -rx reload

# TODO restart after crash
xinit /root/video-doorbell/app/app -- :0 -nocursor &

sleep 1
# Apply some X settings
setup_display () {
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
}

while ! setup_display; do
	sleep 1
done