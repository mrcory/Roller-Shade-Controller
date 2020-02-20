# Roller-Shade-Controller
D1 Mini powered roller shade controller for my 3D printed roller shade.

Intended for use with a custom PCB I designed, but can also be used with a custome protoboard. (As I did during testing.)

## Welcome to my little project!
The purpose behind this project was two-fold. One, I want a remote controlled shade for my window, but didn't want to spend that much. Two, I wanted a small project to do to break the tedium of my working on my aquarium controller.

I have succeeded in both of those goals. (If I do say so myself.) I managed to build the majority of the hardware for the shade for under $20 unit cost estimated. (I did luck out on cheap motors) and Unfortunately, this does not include beautification. Function on the other hand has been a success. (Who says you need form with function.)

More features, like physical button control, will add a little bit to the cost but aren't strictly necessary.

## Project Info

## Setup
[Check The Wiki](https://github.com/mrcory/Roller-Shade-Controller/wiki)

### Hardware
 [Thingiverse Models](https://www.thingiverse.com/thing:3628982) are now available.
 
 Motion is driven by a 28BYJ-48 with a ULN2003.

 Currently controlled by a Lolin NodeMCU v3, but it will be replaced by a Wemos D1 Mini

 A donor roller shade.

 1/2" EMT conduit.

### Software
Blynk is used as a remote control for this project.

[AccelStepper (Fork)](https://github.com/waspinator/AccelStepper) is used to actually drive the motor. _(This version is available in the library manager.)_

[CmdArduino](https://github.com/fakufaku/CmdArduino) is used for control of serial monitor. (A little library I really like.)

FastLED is used to control addressable LEDs.

### Goal
The goal is to make a low cost remote controlled roller shade that can be made DIY. The only tools needed are some hex wrenches, a hacksaw (band saw is also acceptable), and a 3D printer.

More tools may be required to make the final product look nice, but that is _optional_.

## Known Issues
* Due to the way that the stepper is driven Blynk will disconnect when the shade is traveling. This is because Blynk slows the code execution. I believe this is happening due to the time it takes for Blynk to communicate with the server to check for any updates. To make the stepper run smoothly Blynk.run() is not run while the stepper is active by putting it behind an if check.

This issue could be fixed by using an ESP32 in the D1 Mini form factor.

* The code is ugly ~~and lacks comments.~~
