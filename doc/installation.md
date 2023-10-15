# Installation

This file contains all the steps required to install on a fresh touchscreen computer

### Operating system
1. Install ubuntu server
2. Follow steps below

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
apt install -y isc-dhcp-server net-tools # for the dhcp server
apt install -y asterisk
apt instal -y unzip # for bun
curl -fsSL https://bun.sh/install | bash
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

### Configure the dhcp server
```shell
mv /etc/dhcp/dhcpd.conf{,.backup}
cp dhcp-server/dhcpd.conf /etc/dhcp
mv /etc/network/interfaces{,.backup}
cp dhcp-server/interfaces /etc/network
mv /etc/default/isc-dhcp-server{,.backup}
cp dhcp-server/isc-dhcp-server /etc/default

systemctl restart isc-dhcp-server.service
systemctl enable isc-dhcp-server.service
```

### Enable auto login for root
```shell
systemctl edit getty@tty1
```
Add the following lines:\
The `ExecStart=` line is required (do not ask why)
```conf
# ...
### Anything between here and the comment below will become the new contents of the file
[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin root --noclear %I 38400 linux
# ...
```

```shell
systemctl enable getty@tty1.service
```

### Disable the wait for network startup service
We don't need a internet connection for this project
```shell
 systemctl disable systemd-networkd-wait-online.service
```

### Install config deps
```shell
cd /root/video-doorbell/config
bun i # internet connection required
```

### Run the production.sh script on startup
```shell
echo '# if this terminal is not a ssh session execute the script script' >> /root/.bashrc
echo '[ -z "${SSH_CLIENT}" ] && /root/video-doorbell/production.sh' >> /root/.bashrc
echo "alias config='bun /root/video-doorbell/config/config.ts'" >> /root/.bashrc
reboot
```
