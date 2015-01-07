/* Arduino Controlled Kambrook Remote Power Switch by: Mike Thompson August 2013, based on the pioneering efforts of Nevoz
posted here: http://forum.arduino.cc/index.php?PHPSESSID=s8nama9jdekqsc6pn8r5u7gc92&topic=178765.0
Added support for controlling via subscription to an MQTT command topic and the ability to optionally set the required master code
so you can still use the included remote to control the switches. 
To sniff your own master code run something like the RFSniff code example and trap the 47 bits between sync pulses and 
align with the code example eg. 0101 0101 1111 1011 0110 0011 1001 1001 1110 0001 1111 1111
The master code is contained in blocks 5,6,7 and 8
Change the master code by altering the "Code_Master" string in KambrookRCO1.cpp
*/

#include "Arduino.h"
#include "KambrookRCO1.h"
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35 };
byte ip[]     = { 192, 168, 2, 150 }; //Arduino IP
byte gateway[] = { 192, 168, 2, 1 };
byte netmask[]  = { 255, 255, 255, 0 };
byte server[] = { 192, 168, 2, 26 }; //MQTT Server

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void callback(char* topic, byte* payload, unsigned int length) //get MQTT command
{
  payload[length] = '\0'; //adds null to end of string to create correct string
  String strPayload = String((char*)payload);
  
  String strGroup = getValue(strPayload, ';', 0);
  String strDevice = getValue(strPayload, ';', 1);
  String strCommand = getValue(strPayload, ';', 2);
  
  // e.g. SendCode("A";"1";"ON");
  SendCode((char*)strGroup.c_str(),(char*)strDevice.c_str(),(char*)strCommand.c_str());
}

unsigned long timesent=0;
unsigned long time=0;

void setup()
{
  delay(600); // allow some time after powerup and sketch start, for the Wiznet W5100 Reset IC to release and come out of reset. 
  Ethernet.begin(mac, ip, gateway, netmask);

  int con=false;
  while(con==false)
  {
    con=client.connect("ArduinoKambrook");
    delay(500);
  }
  client.subscribe("Kambrook/command"); //MQTT topic where commands published
}

void loop()
{
  if(client.loop()==false) //if connection lost
  { 
    int con=false;
    while(con==false)
    {
      con=client.connect("ArduinoKambrook");
      delay(500);
    }
    client.subscribe("Kambrook/command"); //resubscribe if connection reset
  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
