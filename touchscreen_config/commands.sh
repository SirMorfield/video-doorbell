su root
cd /root
rm -rf Desktop/ Documents/ Music/ Pictures/ Templates/ Videos/

apt update && apt upgrade
apt install -y xinit openbox git

git clone https://github.com/SirMorfield/video-doorbell

# test with:
startx

reboot

systemctl edit getty@tty1
# add the following lines
# [Service]
# ExecStart=
# ExecStart=-/sbin/agetty --autologin root --noclear %I 38400 linux
systemctl enable getty@tty1.service
reboot

echo 'if [[ -z $DISPLAY ]] && [[ $(tty) = /dev/tty1 ]]; then exec startx; fi' >> ~/.bashrc
reboot

vi /etc/xdg/openbox/rc.xml
# add the following lines just before the </applications> tag
#   <application class="app*">
#     <maximized>yes</maximized>
#     <decor>no</decor>
#   </application>
