# Digital Doorbell
Designed by Joppe Koers in september 2022

## Description
This project is a video doorbell system that shows users who is at the front door and allows them to open it.\
This system consists of a touchscreen computer that runs headless
- `Debian` with  a=
- `xinit` window manager, that runs a
- `C++17` application with a frontend in
- `ImGui` with a
- `OpenCV3` backend.\
When the user selects a name to call the C++ application calls a
- `docker` container that runs a
- `asterisk` `SIP` server, and uses asterisk's
- `originate` command to make the front door camera
- `AXIS A8105-E` call a the selected client's
- `IP video phone` model:
- `Fanvil_I53W`\
This happens over a local network that is not connected to the outside internet
The `touchscreen computer` runs a
- `DHCP` server that manages the local network's IP addresses, and assigns a
- `static IP` to itself.\
A user can also call a second SIP camera in the hallway powered by a
- `Raspberry Pi 3` that runs a
- `linphone` SIP client.\
The system also supports
- `calling between users`
See `doc/notes.md` for more information.

## Changing apartment occupant(s) (names)
1. Connect a computer to the `PoE switch`
2. ssh into the `touchscreen computer`: `ssh root@192.168.2.18`
3. Edit the occupants config file: `nano /root/video-doorbell/app/occupants.conf`
	See `/root/video-doorbell/app/occupants.example.csv` for how to configure it
4. Run `/root/video-doorbell/production.sh` to load the config file, watch the output, it will tell you if you've not formatted the file correctly
5. Validate that the app has started correctly by making a test call

### Get a list connected `indoor stations` and their IPs
1. ssh into the `touchscreen computer` as described above
2. run `asterisk -rx 'sip show peers'`

## Components
Why these components? Because they had already been bought, so I might as well use them.\
This project uses names like `indoor Station` throughout, this is a list of what those devices are referring to.
- FABS-810P/G(H)			`touchscreen computer`
- AXIS A8105-E 				network	video door station / `front door camera`
- AXIS A8105-E 				network	video door station / `hallway camera`
- Raspberry Pi 2B			linphone camera / `rpi camera`
- Fanvil_I53W				ip phone / `indoor station`
- HP 2520-24-PoE 28 port	managed `network switch`
