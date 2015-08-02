Network Enabling the Daikin BRC1D52 Thermostat

Wouldn’t it be nice if your airconditioner could be switched on or off remotely, make intelligent decisions and be made to follow your own custom rules? Why not automatically switch off when you leave the house? If the house is at 17 degrees and you are heating it to 19 degrees, but the temperature outside is 20 degrees, why not just open the windows and turn off the air conditioner? 

There are some smarter thermostats now available such as NEST, but they don’t necessarily give you the control customisation you require, cost too much or are simply not compatible with your air conditioner.

Unfortunately my Daikin VRV system uses a proprietary 2 way protocol and electrical standard  between the thermostat and main unit. Although it uses 2 wires, it is not compatible with most other thermostats including NEST. Daikin offers a WiFi add-on called SkyFi, but the costs are quite ridiculous (around $600 AUD fitted) and you are stuck with a proprietary app that doesn't support intelligent processing or integration with common IOT methods.
The Goal

The goal was really quite simple:

Preserve existing thermostat controller capabilities.
Add network connectivity.
Switch the air conditioner on or off based on commands received over the network.
Send air conditioner on/off status information over the network 
Budget under $100.

I could then add whatever software rules I liked running from another network location to control the air conditioner. There are plenty of other potential options to control such as switch between heat/cool, zones, etc. but these features are well managed by the controller and are not really required for typical remote operations.
Where to start?

Hacking the protocol was going to be complex given its closed nature and 2 way signalling methods, so hacking the controller itself was the best option. As it turns out, it was easy to do.

The picture below shows the on/off switch and the status led. The aim was to trigger the button with a relay and measure the voltage across the led to detect on/off status without affecting the basic operation of the controller.


Modifying the Controller

The casing is easily removed with a small flat end screwdriver at the points marked. The circuit board can then be carefully lifted out after removing all the anchoring screws.You should switch the air conditioner off at the mains before proceeding. I also suggest labelling the main connecting wires before detaching from the circuit board.




After removing the main circuit board, solder the four connecting wires in the locations shown. The black and red wires are soldered to the on/off button pins and the green and yellow wires are soldered to the cathode and anode pins of the on/off LED.

Route the cable along the available channel (no modification required) in the casing, replace the circuit board and pass the cable back through the wall cavity.

Controlling via an Arduino

The Arduino is ideal for this task with the addition of a basic relay and network capabilities. The key tasks required are:

Toggle a relay to emulate the on/off button
Measure the voltage across the LED to distinguish between the on and off state
Measure the ambient temperature/s where relevant (assuming you don’t already have this+data)
Publish the status and temperature to MQTT topics over an IP connection
Subscribe to an MQTT topic to listen for on/off commands.

A basic relay shield and ethernet or wifi shield are all that is needed. I was lucky enough to have a spare ethernet and pebble shield available that provided the relay, temperature sensor and as a bonus, an LCD display. If you don’t have temperature data already, I would recommend connecting at least one DS18B20 or similar sensor.
Wiring

After running the cable back through the wall cavity, I connected it to the arduino on an adjacent wall as shown below. Note: the ethernet shield is stacked on the arduino directly below the pebble shield shown.



The black and red button wires are connected directly to the relay terminals on pin 6 (polarity is not important).  When the relay is tripped (closed) for 500ms, a button press action is generated.

The green wire from the LED is connected to Analog pin 4 and via a 10k pulldown resistor to ground. The yellow wire from the LED is connected to +5V.

I ran some simple tests on the analog pin values when the led was on and off to determine the threshold values for the LED state. The mid value between the high and low values was 760 (out of 1024 units).  Above 760 the LED is detected as OFF and below 760 the LED is detected as ON.

Note: with the wiring as shown, when power is lost to the arduino, the LED will turn off, even though the air conditioner is active.

The DS18B20 temperature sensor is connected on digital pin 5.  I actually chained a second DS18B20 sensor off the same pin on a long cable so I could measure the temperature more accurately closer to the controller and not be affected by any heat generated by the arduino.
Code

In its simplest form the code is as follows:
mqtt
etc.,

Advanced 
Adding Node Red smarts
Dashboard

