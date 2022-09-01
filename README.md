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

https://github.com/mailrocketsystems/AsteriskVOIP
https://mir-server.io/docs/make-a-secure-ubuntu-web-kiosk
https://stackoverflow.com/questions/43743005/is-group-calling-over-sip-possible-with-asterisk
https://github.com/antonraharja/book-asterisk-101/blob/master/book-contents/en/Chapter_04/Ring-Groups.md
http://www.asteriskdocs.org/en/3rd_Edition/asterisk-book-html/asterisk-book.html

## Test
rtsp://192.168.2.11:8083/onvif-media/media.amp # or other port

## Reminders
Remove DHCP bindings from test network
For the automatic doorbell:  Fanvil -> Settings -> action
Disable automatic hangup timeout
Time server


## Possible future upgrades
- Multiple cameras at the Door
- Alarm mode
- (Motion) security camera
	- Saving video file every time someone opens the door
- Logging who opened the door when
- Calling between clients
- Correct datetime on the indoor stations
- Select best sound and video codecs
- Do not connect call between front door camera and client when they are not home
- Enable ICE

## Technologies explained
SIP (Session Initiation Protocol)
	- A signaling protocol used for initiating, maintaining, and terminating communication sessions that include voice, video and messaging applications.

UDP (User Datagram Protocol)
TCP (Transmission Control Protocol)
SCTP (Stream Control Transmission Protocol)
	- The transport layer protocols

TLS (Transport Layer Security)
	- Secure SIP messages

SDP (Session Description Protocol)
	- Describes the media streams in a SIP session

RTP (Real-time Transport Protocol) SRTP (Secure Real-time Transport Protocol)
	- Transports the media (audio/video) streams

unicast multicast
	- SIP can be used to establish two-party (unicast) or multiparty (multicast) sessions.

SDP (Session Description Protocol)
	- Describes the media streams in a SIP session

SIP URI
	- has the form sip:username@domainname or sip:username@hostport, where domainname requires DNS SRV records to locate the servers for SIP domain while hostport can be an IP address or a fully qualified domain name of the host and port.
SIP ports
	- SIP clients typically use TCP or UDP on port numbers 5060 or 5061(secure) for SIP traffic to servers and other endpoints

Each **user agent (UA)** performs the function of a **user agent client (UAC)** when it is requesting a service function, and that of a **user agent server (UAS)**

A SIP phone is an IP phone that implements client and server functions of a SIP user agent and provides the traditional call functions of a telephone, such as dial, answer, reject, call hold, and call transfer.

Registrar
- A registrar is a SIP endpoint that provides a location service. It accepts REGISTER requests, recording the address and other parameters from the user agent. For subsequent requests, it provides an essential means to locate possible communication peers on the network. The location service links one or more IP addresses to the SIP URI of the registering agent. Multiple user agents may register for the same URI, with the result that all registered user agents receive the calls to the URI.

ICE (Interactive Connectivity Establishment)
a technique used in computer networking to find ways for two computers to talk to each other as directly as possible in peer-to-peer networking. This is most commonly used for interactive media such as Voice over Internet Protocol (VoIP), peer-to-peer communications, video, and instant messaging.
