/*
Purpose: to control the Kambrook RF3399 Remote Control Power Outlet using an Arduino.
The Kambrook remote control has model no. RF4471.
Both of the above are made by Winplus(www.winplus.com.au) and re-sold by Kambrook.

When a remote control button is pressed, an RF signal is sent 5 times, each separated by a delay
designated by "PULSE_DELAY" in my code below.
All transitions are either a short off and then a short on (which I've designated as "1") , or a
long off and then short on (which I've designated as "0")

Usage: SendCode("Letter position of remote control(A,B,C,D)","Number button of remote control(1,2,3,4,5)","Power state");

e.g. to turn ON A 3 :

SendCode("A","3","ON");

Note: "hidden" letter positions "E" and "F" have been added to this sketch, giving you another 10 codes
not available on the original Kambrook remote control. An advantage of this is that if your neighbour
has purchased the same Kambrook kit, he/she will not be able to inadvertently control your outlets set on
these hidden codes.
More of these hidden codes can be added quite easily.

Coded by Nev V 2013. version 1.00
Updated by mike_linus 2014 to include master code variable version 1.01
*/

#include "Arduino.h"
#include "KambrookRCO1.h"

#define RF_DATA_PIN 9
#define SHORT_ON 285
#define SHORT_OFF 295
#define LONG_OFF 645
#define PULSE_DELAY 9500 // time to wait before sending code again

// example of an RF signal (spaces added for readability): "1010 1010 1111 1111 1111 1111 0000 0100 1111 1100 0000 0000";

String Code_Prefix = "1010 1010 1111 1111"; 
String Code_Master = "0110 0010 0001 0001"; //unique code for each remote.  Only need to change if you want to match included remote. Sniff remote to get code  
String Code_Suffix = "0000 0000";

String Empty1 ="";

char* ABCD[]={
"1111",
"1110",
"1101",
"1100",
"1010",
"1001",
};

String Code_1_On  = "1110";
String Code_1_Off = "1101";
String Code_2_On  = "1100";
String Code_2_Off = "1011";
String Code_3_On  = "1010";
String Code_3_Off = "1001";
String Code_4_On  = "1000";
String Code_4_Off = "0111";
String Code_5_On  = "0110";
String Code_5_Off = "0101";
String return1;


void SendCode(char* transletter, char* transnumber, char* transstate) {
 
  
  pinMode(RF_DATA_PIN, OUTPUT); 
  
  strupr(transletter);
  
  Serial.println(transletter);
  Serial.println(transnumber);
  Serial.println(transstate);
  
  int transletterarraypos;
  if (transletter[0] == 'A') {
    transletterarraypos = 0;
  }
  
  if (transletter[0] == 'B') {
    transletterarraypos = 1;
  }

  if (transletter[0] == 'C') {
    transletterarraypos = 2;
  }
 
  if (transletter[0] == 'D') {
    transletterarraypos = 3;
  }
  
  if (transletter[0] == 'E') {
    transletterarraypos = 4;
  }
  
  if (transletter[0] == 'F') {
    transletterarraypos = 5;
  }
  
  
  String numberstate;
  numberstate = Empty1 + transnumber + transstate;
  numberstate.toUpperCase();
  
  String CodeOnOff="";
  if (numberstate == "1ON") {
    CodeOnOff = Code_1_On;
  }
  if (numberstate == "1OFF") {
    CodeOnOff = Code_1_Off;
  }
  if (numberstate == "2ON") {
    CodeOnOff = Code_2_On;
  }
  if (numberstate == "2OFF") {
    CodeOnOff = Code_2_Off;
  }
  if (numberstate == "3ON") {
    CodeOnOff = Code_3_On;
  }
  if (numberstate == "3OFF") {
    CodeOnOff = Code_3_Off;
  }
  if (numberstate == "4ON") {
    CodeOnOff = Code_4_On;
  }
  if (numberstate == "4OFF") {
    CodeOnOff = Code_4_Off;
  }
  if (numberstate == "5ON") {
    CodeOnOff = Code_5_On;
  }
  if (numberstate == "5OFF") {
    CodeOnOff = Code_5_Off;
  }
  
  
  String FullCode1 = Code_Prefix + Code_Master + ABCD[transletterarraypos] + CodeOnOff + Code_Suffix;
  
  FullCode1.replace(" ","");
  
  Serial.println(FullCode1);
  
  for (int f=0; f < 5; f++){
    for (int i = 0; i < FullCode1.length(); i++){
      if (FullCode1[i] == '1') {
      send1();
     }
     if (FullCode1[i] == '0') {
      send0();
     }
    }
  delayMicroseconds(PULSE_DELAY);
  }

 
}

// send 1
void send1() {
  // Serial.print("1");
  digitalWrite(RF_DATA_PIN, HIGH);
  delayMicroseconds(SHORT_OFF);
  digitalWrite(RF_DATA_PIN, LOW);
  delayMicroseconds(SHORT_ON);
}

void send0() {
  // Serial.print("0");
  digitalWrite(RF_DATA_PIN, HIGH);
  delayMicroseconds(LONG_OFF);
  digitalWrite(RF_DATA_PIN, LOW);
  delayMicroseconds(SHORT_ON);
}

