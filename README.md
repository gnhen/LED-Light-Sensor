# LED-Light-Sensor
Takes voltage output from LED and turns on an indicator when light drops below a certain level


# Introduction

This is more of a side project for tinkering with ESPs and Arduinos than anything, so this is pretty specific to my use case.

The way this works is simple

#### An LED outputs a voltage based on the amount of light that is going into the hardware in the LED.
##### This voltage is minimal and the light amounts are sensitive. It is best to use this with one single light source, such as another LED from some other hardware, isolated from any other light

# Usage

1. Download the file in releases
2. Use the Arduino IDE to push code to your ESP
3. Put your LEDs in the pins labeled on the top of the code
4. Watch it work!
