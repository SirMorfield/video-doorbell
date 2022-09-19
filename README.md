# Digital Doorbell

## Description
This project is a video doorbell system that shows users who is at the front door and allows them to open it.\
This system consists of a touchscreen computer that runs headless
- `Debian` with  a
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
The `Touchscreen computer` runs a
- `DHCP` server that manages all the clients, and assigns a
- `static IP` to itself.\
A user can also call a second SIP camera in the hallway powered by a
- `Raspberry Pi 3` that runs a
- `linphone` SIP client.\
The system also supports
- `calling between users`

## Components
Why these components? Because they had already been bought, so I might as well use them.

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

- HP 2520-24-PoE 28 port managed network switch

## Links
https://github.com/mailrocketsystems/AsteriskVOIP
https://github.com/antonraharja/book-asterisk-101/blob/master/book-contents/en/Chapter_04/Ring-Groups.md
https://wiki.linphone.org/xwiki/wiki/public/view/Linphone/Linphone%20and%20Raspberry%20Pi/

## Abbreviations
SIP (Session Initiation Protocol)
	- A signaling protocol used for initiating, maintaining, and terminating communication sessions that include voice, video and messaging applications.

UDP (User Datagram Protocol)
TCP (Transmission Control Protocol)
SCTP (Stream Control Transmission Protocol)
	- The transport layer protocols

TLS (Transport Layer Security)
	- Secure SIP messages

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
