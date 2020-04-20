# Integrated Design Project
By: Bernie Cieplak ([@sideberns](https://github.com/sideberns)) & Risa Philpott ([@philpottrj](https://github.com/philpottrj))

---

## Purpose
The purpose of this code is to create a smart home device run by Arduino.  This allows for an arbitrary device, like a smartphone or Raspberry Pi, to interface with the Arduino wirelessly and interact with it.

## Changelog

### v1.0 - Milestone 4
- Added HTTPS to website
- Password-protected website using Nginx
	- Default User: ID-User
	- Default Password: secure-password
- Website host moved from Windows PC to Raspberry Pi
- Changed port forwarding agent from ngrok to localhost.run
- Added garbage collection on web server to prevent overloading
- Website and Android now have a grid status page
- Android application now uses a material design theme
- Android application and website can now be used simultaneously

### v0.3 - Milestone 3
- Implemented Bluetooth connectivity
- Developed Android Application for controlling the Arduino from a cell phone
- Utilized ngrok as port forwarding agent

### v0.25 - Website Integration (Prerelease)
- Implemented WiFi connectivity
- Built Python Flask application to handle Button presses remotely
- AJAX requests are used to handle reading and displaying Arduino status
- Note: Website CSS will be implemented at a later date

### v0.2 - Milestone 2
- Created the arduino circuit and code
- Allows for local functionality of the following appliances
	- LCD Display
	- Smart Switch
	- Weather Station/Temperature Sensor
	- Air Conditioning Unit
	- Intruder Alert System
	- Night light
- Note: all interaction with Bluetooth is simulated using button presses

## Implemented Features
- [x] Wireless connectivity
- [x] Bluetooth functionality
- [x] Encryption capability