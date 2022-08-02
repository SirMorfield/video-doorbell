# Digital Doorbell

## Components
- FABS-810P/G(H)		Touchscreen computer

- AXIS A8105-E 			Network	Video Door Station
	- SIP	(Session Initiation Protocol)
	- ONVIF	(Open Network Video Interface Forum)
	- VAPIX	(Video API for Axis Products)

- Fanvil_I53W			IP Phone
	- SIP v1 (RFC2543), v2 (RFC3261) over UDP/TCP/TLS
	- STUN (Session Traversal Utilities for NAT)
		- RFC3489, RFC5389

	- L2TP
	- TR-069
	- dtmf
	- MCAST listening|paging

- Netgear FS116P		Switch 8 PoE ports + 8 normal ports


## Technologies
- SIP (Session Initiation Protocol)
	- https://github.com/pjsip/pjproject
	- RFC3261


- RTP (Real-time Transport Protocol), RTCP, SRTP
	- RTP is a network protocol for delivering audio and video over IP networks.
	- RFC 3550

	- https://github.com/j0r1/JRTPLIB
	- https://www.gnu.org/software/ccrtp/

- VoIP

## Reminders
- SIP is default on port 5060
- RTP is default on port 5004
- RTCP is default on port 5005
- RTSP is default on port 554

## Thoughts
- Is the 100Mbt Ethernet enough for the audio+video stream?
- Can we connect multiple PoE network switches together?
- Save this repo to a USB stick with all the documentation on it.
- Host info etch on joppekoers.nl that redirects to github.
- C++ gui options:
	- Qt
	- GTK
	- Nuklear
	- Imgui



## Links
http://192.168.2.11/admin-bin/editcgi.cgi?file=/
http://192.168.2.11/admin-bin/editcgi.cgi?file=/usr/html/axis-cgi

http://192.168.2.11/axis-cgi/mjpg/video.cgi
http://192.168.2.11/mjpg/video.mjpg
rtsp://192.168.2.11:554/axis-media/media.amp


## Test
rtsp://192.168.2.11:8083/onvif-media/media.amp # or other port
