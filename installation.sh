# This file contains all the scripts required to install on a fresh computer
# DO NOT just run this script, read it and execute all commands indivualy whilst using your brain

# 1. install a debian version
# 2. When you get to the "Software Selection" part of the installation, only select the SSH server
# 3. Follow steps below

su root
cd /root
rm -rf Desktop/ Documents/ Music/ Pictures/ Templates/ Videos/

# On debian you have to install a extra user alongside the root user, we don't need that user
userdel -rf <installed non-root-user>

apt update -y && apt upgrade -y
apt install -y xinit openbox git
apt install -y make libglfw3-dev pkg-config build-essential

# If rebooting is not working change the following
# vim /etc/default/grub
# GRUB_CMDLINE_LINUX="reboot=pci"

git clone https://github.com/SirMorfield/video-doorbell

# vim  /etc/systemd/logind.conf
# [Login]
# NAutoVTs=1
# ReserverVT=1

systemctl edit getty@tty1
# add the following lines
# [Service]
# ExecStart=
# ExecStart=-/sbin/agetty --autologin root --noclear %I 38400 linux # root is the default user in this case
systemctl enable getty@tty1.service

echo '# if this terminal is not a ssh session execute the script script' >> /root/.bashrc
echo '[ -z "${SSH_CLIENT}" ] && /root/video-doorbell/production.sh' >> /root/.bashrc
reboot
