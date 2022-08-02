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

- Netgear FS116P		Switch


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
