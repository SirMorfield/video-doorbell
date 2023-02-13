# Installation

This file contains all the steps required to install on a fresh touchscreen computer

### Operating system
1. Install a debian version
2. When you get to the "Software Selection" part of the installation, only select the SSH server
3. Follow steps below

### Remove defaults
```shell
su root
cd /root
rm -rf Desktop/ Documents/ Music/ Pictures/ Templates/ Videos/
```

### On debian based systems you have to install a extra user alongside the root user, we don't need that
```shell
userdel -rf <installed non-root-user>
```

### Requirements, might not be complete
```shell
apt update -y && apt upgrade -y
apt install -y git rsync # tools
apt install -y xinit xinput x11-xserver-utils openbox # x11
apt install -y make libglfw3-dev pkg-config build-essential	# for the c++ app
apt install -y isc-dhcp-server ifconfig # for the dhcp server
```

### Get this repo
```shell
git clone https://github.com/SirMorfield/video-doorbell
```

### Disabling all but one virtual terminal
```shell
vim /etc/systemd/logind.conf
```
And change:
```conf
[Login]
NAutoVTs=1
ReserverVT=1
```

### Install SIP server
```shell
apt install -y asterisk
```

### Configure the dhcp server
```shell
apt install -y isc-dhcp-server
cp dhcp-server/dhcpd.conf /etc/dhcp/dhcpd.conf
cp dhcp-server/interfaces /etc/network/interfaces
cp dhcp-server/isc-dhcp-server /etc/default/isc-dhcp-server
systemctl restart isc-dhcp-server.service
systemctl enable isc-dhcp-server.service
```

### Enable auto login for root
```shell
systemctl edit getty@tty1
```
Add the following lines:
```conf
[Service]
ExecStart=-/sbin/agetty --autologin root --noclear %I 38400 linux
```

```shell
systemctl enable getty@tty1.service
```

### Run the production.sh script on startup
```shell
echo '# if this terminal is not a ssh session execute the script script' >> /root/.bashrc
echo '[ -z "${SSH_CLIENT}" ] && /root/video-doorbell/production.sh' >> /root/.bashrc
reboot
```

# Install NodeJS and ts-node
```shell
curl -sL https://deb.nodesource.com/setup_18.x | sudo -E bash -
apt install -y nodejs
npm i -g ts-node
```
