# Installation

This file contains all the steps required to install on a fresh touchscreen computer

### Operating system
1. Install a debian version
2. When you get to the "Software Selection" part of the installation, only select the SSH server
3. Follow steps below

### Remove defaults
```bash
su root
cd /root
rm -rf Desktop/ Documents/ Music/ Pictures/ Templates/ Videos/
```

### On debian you have to install a extra user alongside the root user, we don't need that user
```bash
userdel -rf <installed non-root-user>
```

### Requirements, might not be complete
```bash
apt update -y && apt upgrade -y
apt install -y xinit openbox git
apt install -y make libglfw3-dev pkg-config build-essential
```

### If rebooting is not working change the following
```bash
vim /etc/default/grub
# And change:
GRUB_CMDLINE_LINUX="reboot=pci"
```

Get this repo
```
git clone https://github.com/SirMorfield/video-doorbell
```

### Disabling all but one virtual terminal
```bash
vim /etc/systemd/logind.conf
# And change:
[Login]
NAutoVTs=1
ReserverVT=1
```

### Install SIP server
```
apt install -y asterisk
```

### Configure the dhcp server
```
apt install -y isc-dhcp-server
cp config/dhcpd.conf /etc/dhcp/dhcpd.conf
cp config/interfaces /etc/network/interfaces
cp config/isc-dhcp-server /etc/default/isc-dhcp-server
systemctl restart isc-dhcp-server.service
systemctl enable isc-dhcp-server.service
```

### Enable auto login for root
```bash
systemctl edit getty@tty1
# Add the following lines:
[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin root --noclear %I 38400 linux
```
```
systemctl enable getty@tty1.service
```
### Run the production.sh script on startup
```
echo '# if this terminal is not a ssh session execute the script script' >> /root/.bashrc
echo '[ -z "${SSH_CLIENT}" ] && /root/video-doorbell/production.sh' >> /root/.bashrc
reboot
