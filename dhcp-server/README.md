# Config files for the touchscreen computer

This folder contains configuration scripts for the dhcp server on the touchscreen computer
They will probably not work for other computers. In each file is a link to te tutorial I followed

Also see `../production.sh`
That contains commands that need to be run on startup to get the dhcp server to work

quote:
```shell
# The isc-dhcp-server cannot start without the interface having a ip
# We set that here
ifconfig enp3s0 192.168.2.18
systemctl restart isc-dhcp-server.service
```
