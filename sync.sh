#/bin/bash
# This script is for manually syncing a dev laptop's state to the touchscreen computer
# we ignore obj and app because the touchscreen needs to build the files itself
rsync -z --archive --no-links --human-readable --perms -P --exclude={'obj','app'}  ~/git/video-doorbell/ root@192.168.2.18:/root/video-doorbell/
