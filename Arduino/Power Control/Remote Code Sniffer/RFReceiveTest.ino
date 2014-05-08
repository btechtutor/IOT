byte pin = 8;
unsigned long ulStartTime;

const int ArraySize=200;
unsigned long ulaDuration[ArraySize];
unsigned long ulaMicros[ArraySize];

long iIdx=0;

void outputArray();

void setup() {
  pinMode(pin, INPUT);
  Serial.begin(115200);  // Set Serial Monitor to match
  Serial.flush(); 
  Serial.println("Test data");
  ulStartTime = millis();
}

void loop() {

  ulaDuration[iIdx] = pulseIn(pin, HIGH);
  ulaMicros[iIdx] = micros();
  ++iIdx;
  
  if (iIdx == ArraySize-1) outputArray();
}

void outputArray() {
  int i;
  
  Serial.println("Entering OutputArray");
  for (i=0;i<=iIdx; ++i) {
    Serial.print(ulaDuration[i]);
    Serial.print("     ");
    if (i==0) Serial.println(); else Serial.println(ulaMicros[i]-ulaMicros[i-1]);
  }
  iIdx = 0;
}
