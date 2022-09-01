#!/bin/bash

if [ $(pwd) != "**/sip_server" ]; then
	cd sip_server
	echo "Changed directory to sip_server"
fi

find . -not -name install.sh -type f -exec sudo cp -f {} /{} \;
find . -not -name install.sh -type f -exec sudo chown asterisk:asterisk /{} \;
