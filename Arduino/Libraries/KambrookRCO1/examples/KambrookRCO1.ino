#include "Arduino.h"
#include "KambrookRCO1.h"


void setup(){
 Serial.begin(9600);

 SendCode("B","1","ofF");
 
}

void loop(){

}
