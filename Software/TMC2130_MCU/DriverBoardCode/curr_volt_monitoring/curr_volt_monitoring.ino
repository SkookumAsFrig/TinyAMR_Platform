/*
monitor current, read each cell voltage of the 4 cell lipo
 
 */

#define EN1 38

#define EN2 49

#define EN3 36

#define EN4 22

// These constants won't change.  They're used to give names
// to the pins used:
const int cell4 = A3;  // Analog input pin that the potentiometer is attached to
const int cell3 = A4;
const int cell2 = A5;
const int cell1 = A6;
const int curr5v = A0;
const int curr12v = A1;
const int currchrg = A2;

const float ref_volt = 4.87;

int sensorValue = 0;
int pinvals[7] = {
  cell4, cell3, cell2, cell1, curr5v, curr12v, currchrg};
float batcells[4];

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
  pinMode(EN1, OUTPUT);
  digitalWrite(EN1, HIGH);
  pinMode(EN2, OUTPUT);
  digitalWrite(EN2, HIGH);
  pinMode(EN3, OUTPUT);
  digitalWrite(EN3, HIGH);
  pinMode(EN4, OUTPUT);
  digitalWrite(EN4, HIGH);

  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);
  pinMode(27, INPUT_PULLUP);
}

float print_val = 0.1;
float diffcell1;
float diffcell2;
float diffcell3;
int val = 0;

void loop() {
  char buff[8];
  char buff1[8];
  char buff2[8];
  char buff3[8];
  char buff4[8];
  char bufftop[8];
  val = digitalRead(27);
  for(int i = 0; i < 7; i ++){
    // read the analog in value:
    sensorValue = analogRead(pinvals[i]);         

    // print the results to the serial monitor:
    if (i==0){
      dtostrf(batcells[0],2,2,bufftop);
      Serial.print("Top: ");
      Serial.print(bufftop);
      Serial.print("V, ");
      
      Serial.print("Cell 1: ");
      batcells[0] = float(sensorValue)*(13.3/3.3)*ref_volt/(1024);
      Serial.print(buff1);
      Serial.print("V");
    }

    if (i==1){
      Serial.print("Cell 2: ");
      batcells[1] = float(sensorValue)*3*ref_volt/(1024);
      Serial.print(buff2);
      Serial.print("V");
    }

    if (i==2){
      Serial.print("Cell 3: ");
      batcells[2] = float(sensorValue)*2*ref_volt/(1024);
      Serial.print(buff3);
      Serial.print("V");
    }

    if (i==3){
      Serial.print("Cell 4: ");
      batcells[3] = float(sensorValue)*ref_volt/(1024);
      diffcell1 = batcells[0]-batcells[1];
      diffcell2 = batcells[1]-batcells[2];
      diffcell3 = batcells[2]-batcells[3];
      
      dtostrf(diffcell1,2,2,buff4);
      dtostrf(diffcell2,2,2,buff3);
      dtostrf(diffcell3,2,2,buff2);
      dtostrf(batcells[3],2,2,buff1);
      Serial.print(buff4);
      Serial.print("V");
    }

    if (i>=4)
      dtostrf((float(sensorValue) - 100)*ref_volt/(1024*0.4),2,2,buff);
    
    if (i==4){
      Serial.print("5VDC: ");
    }
    if (i==5){
      Serial.print("12VDC: ");
    }
    if (i==6){
      Serial.print("CHG: ");
    }
    
    if (i>3){
      Serial.print(buff);
      Serial.print("A");
    }

    if (i!=6)
      Serial.print(", ");
    else
      Serial.println("");
  }

  if (val){
    Serial.println("Switch On");
  }

  delay(2000);                     
}
