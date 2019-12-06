#include <avr/interrupt.h>
long system_clock = 16000000;

long prescaler = 1;

#define DIR_1 44

#define STEP_1 5

#define CS1 40

#define DIR_2 48

#define STEP_2 46

#define CS2 53

#define DIR_3 30

#define STEP_3 11

#define CS3 34

#define DIR_4 28

#define STEP_4 6

#define CS4 24

#define MOSI 51

#define SCK 52

#define MS3_State LOW

#define SCALER_1 B00000001

#define SCALER_8 B00000010

#define SCALER_64 B00000011

#define SCALER_256 B00000100

#define SCALER_1024 B00000101

int scaler[] = {SCALER_1, SCALER_8, SCALER_64, SCALER_256, SCALER_1024};

int FREQ = 60;

int data[5] = {0,0,0,0,0};

int timernumber = 252;



void setup() {

  Serial.begin(115200);

 

}




int acc_dir = 1;

int mode_select = 1;

int F_Lim = 9000;

String sep = "=======================";

void loop() {   
  char buff[9];
  if(Serial.available()){
//    digitalWrite(12, HIGH);
    Serial.readBytesUntil('\n', buff, 10);
    data[0] = (((int)buff[0]) << 8) | ((int)buff[1] & 0xFF);
    data[1] = (((int)buff[2]) << 8) | ((int)buff[3] & 0xFF);
    data[2] = (((int)buff[4]) << 8) | ((int)buff[5] & 0xFF);
    data[3] = (((int)buff[6]) << 8) | ((int)buff[7] & 0xFF);     
    data[4] = buff[8];
//    digitalWrite(12, LOW);   
 
  for(int i = 0; i < 5; i ++){
    Serial.print(data[i]);
//    delayMicroseconds(300);
    Serial.print(',');
  }
  Serial.println('\n');
  Serial.println(buff);
 }
// Serial.println(((138) << 8) | (B11010000));
}
