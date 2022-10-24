# Frontend APP

This folder contains the C++17 app that allows users to call indoor station(s) in an apartment.

It should be launched on a full screen touch screen computer. In this case `/production.sh` handles it.

And because this is a fully custom app there is no limit to it's configurability.

## occupants.conf
The app reads `occupants.conf` (see `occupants.example.csv` for how to configure it) and displays the information as selections on the frontend.
`occupants.conf` is easily configurable and allows for:
- Unlimited occupant names per apartment
- Unlimited users
- Unlimited apartments
- Calling multiple indoor stations at once, for example 1 station upstairs and 1 downstairs
