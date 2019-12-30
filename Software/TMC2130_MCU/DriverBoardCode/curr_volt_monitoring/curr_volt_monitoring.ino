/*
monitor current, read each cell voltage of the 4 cell lipo
 
 */

// These constants won't change.  They're used to give names
// to the pins used:
const int cell4 = A3;  // Analog input pin that the potentiometer is attached to
const int cell3 = A4;
const int cell2 = A5;
const int cell1 = A6;
const int curr5v = A0;
const int curr12v = A1;
const int currchrg = A2;

int sensorValue = 0;        // value read from the pot
int pinvals[7] = {cell4, cell3, cell2, cell1, curr5v, curr12v, currchrg};

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200); 
}

float print_val = 0.1;

void loop() {
  for(int i = 0; i < 7; i ++){
    // read the analog in value:
    sensorValue = analogRead(pinvals[i]);         
  
    // print the results to the serial monitor:
    Serial.print(i);
    Serial.print("ain = " );
    if (i==0)
      Serial.print(float(sensorValue)*(13.3/3.3)*5/(1024));
    if (i==1)
      Serial.print(float(sensorValue)*3*5/(1024));
    if (i==2)
      Serial.print(float(sensorValue)*2*5/(1024));
    if (i==3)
      Serial.print(float(sensorValue)*5/(1024));
    if (i>=4)
      Serial.print((float(sensorValue) - 100)*5/(1024*0.4));
    else
      Serial.print(sensorValue);
    
    if (i!=6)
      Serial.print(", ");
    else
      Serial.println("");
  }

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2000);                     
}
