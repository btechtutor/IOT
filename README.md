IOT
===

This repository contains MQTT, node.js, arduino and other useful stuff for the 'Internet of Things' sense and control systems. Additional components will be added over time.

Kambrook Remote Control Power Switch v1.1
-----------------------------------------
Based on the pioneering efforts of Nev V (Nevoz) posted here: http://forum.arduino.cc/index.php?PHPSESSID=s8nama9jdekqsc6pn8r5u7gc92&topic=178765.0 this code allows you to control the Kambrook RF3399 Remote Control Power Outlet using an Arduino. 

The code has been updated from the original V1.00 code to include support for controlling via subscription to an MQTT command topic over an IP network and the ability to optionally set the required master code so you can still use the included remote to control the switches.

To sniff your own master code run something like the RFSniff code example and trap the 47 bits between sync pulses and  align with the code example
 eg. 0101 0101 1111 1011 0110 0011 1001 1001 1110 0001 1111 1111   The master code is contained in blocks 5,6,7 and 8. Change the master code by altering the "Code_Master" string in KambrookRCO1.cpp
 
The following is an extract from NevOz' description:

The Kambrook remote control has model no. RF4471. Both of the above are made by Winplus(www.winplus.com.au) and re-sold by Kambrook.

When a remote control button is pressed, an RF signal is sent 5 times, each separated by a delay designated by "PULSE_DELAY" in my code below. All transitions are either a short off and then a short on (which I've designated as "1") , or a long off and then short on (which I've designated as "0")

Usage: SendCode("Letter position of remote control(A,B,C,D)","Number button of remote control(1,2,3,4,5)","Power state");

e.g. to turn ON A 3 :

SendCode("A","3","ON");

Note: "hidden" letter positions "E" and "F" have been added to this sketch, giving you another 10 codes not available on the original Kambrook remote control. An advantage of this is that if your neighbour has purchased the same Kambrook kit, he/she will not be able to inadvertently control your outlets set on
these hidden codes. More of these hidden codes can be added quite easily.
