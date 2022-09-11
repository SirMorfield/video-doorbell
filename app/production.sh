#!/bin/sh

# apt-get install -y make libglfw3-dev pkg-config build-essential

# cd'ing to the directory where the script is located
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd "$SCRIPT_DIR"

while [ 1 ]; do
    make
	./app
	sleep 1
done
