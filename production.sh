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

# hot reload config of asterisk
docker exec -it asterisk asterisk -rx  'reload'

# Optional: stop already running containers
if [ "$1" = "--restart-docker" ]; then
	docker stop -t 3 $(docker ps -a -q) || true
	docker rm -f $(docker ps -a -q) || true
fi

# Build frontend app
PRODUCTION=1 make -C app

# Tell x-server to use the touchscreen display, also needed for ImGui in the app to work
export DISPLAY=':0'

# Setting language locales
export LC_CTYPE=en_US.UTF-8
export LC_ALL=en_US.UTF-8

# Optional: Run asterisk SIP server
if [ "$1" = "--restart-docker" ]; then
	docker build -t asterisk asterisk || true
	docker run -d --net=host --restart unless-stopped --name asterisk \
	-v $PWD/asterisk/sip.conf:/etc/asterisk/sip.conf \
	-v $PWD/asterisk/extensions.conf:/etc/asterisk/extensions.conf \
	asterisk
fi

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