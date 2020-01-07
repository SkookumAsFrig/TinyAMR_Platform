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

void setup() {
  Serial.begin(57600);
  pinMode(MOSI, OUTPUT);
  digitalWrite(MOSI, HIGH);
  pinMode(SCK, OUTPUT);
  digitalWrite(SCK, HIGH);
  pinMode(CS1, OUTPUT);
  digitalWrite(CS1, MS3_State);
  pinMode(CS2, OUTPUT);
  digitalWrite(CS2, MS3_State);
  pinMode(CS3, OUTPUT);
  digitalWrite(CS3, MS3_State);
  pinMode(CS4, OUTPUT);
  digitalWrite(CS4, MS3_State);

  pinMode(STEP_1, OUTPUT);
  pinMode(DIR_1, OUTPUT);
  pinMode(STEP_2, OUTPUT);
  pinMode(DIR_2, OUTPUT);
  pinMode(STEP_3, OUTPUT);
  pinMode(DIR_3, OUTPUT);
  pinMode(STEP_4, OUTPUT);
  pinMode(DIR_4, OUTPUT);

  TCCR3A = B01000011;    // Fast PWM
  TCCR4A = B01000011;    // Fast PWM
  TCCR1A = B01000011;    // Fast PWM
  TCCR5A = B01000011;    // Fast PWM
}

void motorFrequency(int motorNum, int frequency){
  //motorNum = 1  pin 5   
  //motorNum = 2  pin 46
  //motorNum = 3  pin 11
  //motorNum = 4  pin 6
  
  long Top;
  if (frequency != 0)  
    Top = (system_clock /( abs(frequency) * 2 * prescaler)) - 1;   // TOP  frequency=(system clock)/ [2 * prescaler * (TOP +1)]
  else
    Top = 65535;
  if (Top>65535)
    Top = 65535;
  
  switch(motorNum){
    case 1:
      if(frequency!=0)
        TCCR3B = TCCR3B & B11100000 | B00011001;         // Prescaler = 1
      else
        TCCR3B = TCCR3B & B11100000 | B00011000;         // Prescaler = 0, turn PWM off
      OCR3A = Top;
      if(frequency>0){ 
        digitalWrite(DIR_1, HIGH);
      }else{
        digitalWrite(DIR_1, LOW);
      }
      break;
    case 2:
      if(frequency!=0)
        TCCR5B = TCCR5B & B11100000 | B00011001;         // Prescaler = 1
      else
        TCCR5B = TCCR5B & B11100000 | B00011000;         // Prescaler = 0, turn PWM off
      OCR5A = Top;
      if(frequency>0){ 
        digitalWrite(DIR_2, HIGH);
      }else{
        digitalWrite(DIR_2, LOW);
      }
      break;
    case 3:
      if(frequency!=0)
        TCCR1B = TCCR1B & B11100000 | B00011001;         // Prescaler = 1
      else
        TCCR1B = TCCR1B & B11100000 | B00011000;         // Prescaler = 0, turn PWM off
      OCR1A = Top;
      if(frequency>0){ 
        digitalWrite(DIR_3, HIGH);
      }else{
        digitalWrite(DIR_3, LOW);
      }
      break;
    case 4:
      if(frequency!=0)
        TCCR4B = TCCR4B & B11100000 | B00011001;         // Prescaler = 1
      else
        TCCR4B = TCCR4B & B11100000 | B00011000;         // Prescaler = 0, turn PWM off
      OCR4A = Top;
      if(frequency>0){ 
        digitalWrite(DIR_4, HIGH);
      }else{
        digitalWrite(DIR_4, LOW);
      }
      break;
  }

}

int acc_dir = 1;
int mode_select = 1;
int F_Lim = 75;
int FREQ = 8000;

void loop() {
  delay(1000);
  Serial.println("still running");
  motorFrequency(3, FREQ);  // motor number: 1, 2, 3, 4  frquency(speed) direction: 1, 0
//  motorFrequency(2, 0, 0);
//  motorFrequency(3, 0, 0);
//  motorFrequency(4, 0, 0);
  delay(1000);
  FREQ = -FREQ;
  
}
