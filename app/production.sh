#!/bin/sh

# apt-get install -y make libglfw3-dev pkg-config build-essential

# cd'ing to the directory where the script is located
SCRIPT_DIR=$(dirname "$0")
echo moving to $SCRIPT_DIR
cd "$SCRIPT_DIR"

export DISPLAY=':0'
xrandr -o right

while [ 1 ]; do
    make
	./app
	sleep 1
done
