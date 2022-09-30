# Frontend APP

This folder contains the C++17 app that allows users to call a apartment

It should be launched on a full screen touch screen computer. In this case `/production.sh` handles that using xinit

And because this is a fully custom app there is no limit to it's configurability.

## occupants.csv
The app reads `occupants.csv` (see `occupants.example.csv` for how to configure it) and displays the information as selections on the frontend.
`occupants.csv` is easily configurable and allows for:
- Unlimited occupant names per apartment
- Unlimited users
- Unlimited apartments

Syntax:
```
<sip phone number>,<Occupant name>
```
`<sip phone number>` is the phone number configured in `/asterisk/sip.conf`
