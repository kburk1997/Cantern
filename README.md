# Cantern
An interactive lantern constructed from an Arduino and a Monster Energy can.

To make the lantern, a can of Monster Energy was cut open from the bottom and an Arduino strip light component was glued onto the inside. The strip light component was connected to the Arduino, which was controlled by the code in this repo.

Two buttons and a sound sensor are also connected to the Arduino.
The first button toggles "calm mode". When toggled on, the lantern will smoothly transition from one color to the next. When toggled off, the lantern will rapidly change colors.
The second button toggles interactive mode. When toggled on, the lantern will change colors based on sound readings from the sound sensor. The speed of the transition is determined by whether or not "calm" mode is on. When toggled off, the lantern will change colors based on the colors of the rainbow.

Devpost submission:
https://devpost.com/software/cantern
