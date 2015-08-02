/* DaikinAircon.ino
 * ~~~~~~~~~~~~~
 * Daikin BRC1D52 Airconditioning network ON/OFF control and status reporting
 * ~~~~~~~~~~~~~
 * Please do not remove the following notices.
 * Version: 0.2
 * Copyright (c) 2015 by Mike Thompson
 * Documentation: http://home.linus.com.au/wordpress/daikin
 * License: This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Australia License.
 * Further information is available here - http://creativecommons.org/licenses/by-nc-sa/3.0/au/deed.en_GB 
 * 
 * Description: This code allows the Daikin BRC1D52 airconditioning thermostat controller to be remotely switched on and off via
 * MQTT commands and report on/off status and temperature back via MQTT. Using tools such as Node-red, complex rules
 * can be created to automatically control the air conditioner well beyond the basic controller capabilities.
 *
 * This code requires connecting an ardunio to the BRC1D52 with four wires to emulate an on/off button press and to capture
 * the the on/off ststus. Further details can be obtained from the supporting documentation.
 *
 * Third-Party libraries
 * ~~~~~~~~~~~~~~~~~~~~~
 * There are several third-party libararies such as OnweWire that are not included in the Arduino IDE and
 * need to be downloaded and installed separately.
 *
 */
 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <PString.h>

#define PIN_ONE_WIRE           5  // DS18B20
#define RELAY_A                6
#define PIN_AIR               A4  //Aircon LED ON Voltage (1.84v) detect
#define BUFFER_SIZE          200

char globalBuffer[BUFFER_SIZE];  // Store dynamically constructed strings
PString globalString(globalBuffer, sizeof(globalBuffer));
String globalState;

//Ethernet config
byte mac[]     = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFD, 0x02 };
byte ip[]      = { 192, 168, 2, 100 };
byte gateway[] = { 192, 168, 2, 1 };
byte netmask[] = { 255, 255, 255, 0 };

//IP address of MQTT server
byte server[]  = { 192, 168, 2, 22 };

OneWire oneWire(PIN_ONE_WIRE);  // Maxim DS18B20 temperature sensor
DallasTemperature sensors(&oneWire);
EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup() 
{
  pinMode(RELAY_A, OUTPUT);

  globalString.begin();
  sensors.begin(); //DS18B20
  
  delay(2000); //allow time for Ethernet initialisation

  Ethernet.begin(mac, ip, gateway, netmask);
  while(!client.connect("ArduinoAircon"))
  {
    delay(500);
  }
  client.subscribe("Daikin/command"); //subscribe to topic to receive MQTT commands
}

void loop() 
{
  if(client.loop()==false) //reconnect and subscribe if connection lost
  { 
    while(!client.connect("ArduinoAircon"))
    {
      delay(500);
    }
    client.subscribe("Daikin/command"); //resubscribe if connection reset
  }
  
  int ledVoltage = analogRead(PIN_AIR);
  if (ledVoltage > 760) //mean value between upper and lower limits
  {
    globalState = "Off";
  }
  else
  {
    globalState = "On";
  }
  globalString = globalState;
  client.publish("Daikin/state", globalBuffer);
  
  sensors.requestTemperatures(); // Send the command to get DS18B20 temperature
  globalString = sensors.getTempCByIndex(0); //change index to retrieve other linked DS18B20 sensors
  client.publish("Daikin/temp", globalBuffer);
  
  delay(1000);
}

//function to extract values from string.  Used to process MQTT message payload
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

void callback(char* topic, byte* payload, unsigned int length) //process MQTT command when received  
{
  payload[length] = '\0'; //adds null to end of string to create correct string
  String strPayload = String((char*)payload);
  String strTopic = String(topic);
  
  if (strTopic=="Daikin/command")
  {
    globalString = getValue(strPayload,' ',0);
    if (globalString == "Off")
    {
      if (globalState == "On")
      {
        tripRelay();
      }
    }
    else
    {
      if (globalState == "Off")
      {
        tripRelay();
      }        
    }
  }          
}

//toggle relay to emulate button press
void tripRelay(void) 
{
  digitalWrite(RELAY_A, HIGH);
  delay(500);
  digitalWrite(RELAY_A, LOW);
}
