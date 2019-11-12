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
int lastdata[8] = {0,0,0,0,0,0,0,0};

void setup() {
  Serial.begin(9600);
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
  pinMode(12, OUTPUT);
}

void motorFrequency(int motorNum, int frequency, int direct){
  //motorNum = 1  pin 5   
  //motorNum = 2  pin 46
  //motorNum = 3  pin 11
  //motorNum = 4  pin 6
  
  long Top = (system_clock /( frequency * 2 * prescaler)) - 1;   // TOP  frequency=(system clock)/ [2 * prescaler * (TOP +1)]
  switch(motorNum){
    case 1:
      pinMode(STEP_1, OUTPUT);
      TCCR3A = B01010111;    // Fast PWM
      TCCR3B = TCCR3B & B11100000 | B00011001;         // Prescaler = 1
      OCR3A = Top;
      pinMode(DIR_1, OUTPUT);
      if(direct){ 
        digitalWrite(DIR_1, HIGH);
      }else{
        digitalWrite(DIR_1, LOW);
      }
      break;
    case 2:
      pinMode(46, OUTPUT);
      TCCR4A = B01010111;    // Fast PWM
      TCCR4B = TCCR4B & B11100000 | B00011001;         // Prescaler = 1
      OCR4A = Top;
      pinMode(DIR_2, OUTPUT);
      if(direct){ 
        digitalWrite(DIR_2, HIGH);
      }else{
        digitalWrite(DIR_2, LOW);
      }
      break;
    case 3:
      pinMode(11, OUTPUT);
      TCCR1A = B01010111;    // Fast PWM
      TCCR1B = TCCR1B & B11100000 | B00011001;         // Prescaler = 1
      OCR1A = Top;
      pinMode(DIR_3, OUTPUT);
      if(direct){ 
        digitalWrite(DIR_3, HIGH);
      }else{
        digitalWrite(DIR_3, LOW);
      }
      break;
    case 4:
      pinMode(6, OUTPUT);
      TCCR5A = B01010111;    // Fast PWM
      TCCR5B = TCCR5B & B11100000 | B00011001;         // Prescaler = 1
      OCR5A = Top;
      pinMode(DIR_4, OUTPUT);
      if(direct){ 
        digitalWrite(DIR_4, HIGH);
      }else{
        digitalWrite(DIR_4, LOW);
      }
      break;
  }

}

int acc_dir = 1;
int mode_select = 1;
int F_Lim = 9000;


int* getBuffer(int* lastdata){
    /***************************************************
     * start char '$'
     * acceleration end char '%'
     * stepper1 end char '@'
     * stepper2 end char '#'
     * stepper3 end char '!'
     * stepper4 end char '&'
     * $acceleration%dir1,fqc1@dir2,fqc2#dir3,fqc3!dir4,fqc4&
    ***************************************************/
    int i = 1;
    int dataindex = 0;
    int flag; 
    int* data = lastdata;
    while(Serial.available()){
    delay(3);
    if(Serial.available()){
      String buff = Serial.readString();
      if(buff[0] != '$')
        break;
      digitalWrite(12, HIGH);
      flag = false;
      while(i < buff.length()){
        if(buff[i] == ','){
          flag = i;
        }
        if(buff[i] == '@'){
          data[0] = (buff.substring(flag-1, flag)).toInt();
          data[1] = (buff.substring(flag+1, i)).toInt();
        }
        if(buff[i] == '#'){
          data[2] = (buff.substring(flag-1, flag)).toInt();
          data[3] = (buff.substring(flag+1, i)).toInt();
          
        }
        if(buff[i] == '!'){
          data[4] = (buff.substring(flag-1, flag)).toInt();
          data[5] = (buff.substring(flag+1, i)).toInt();
        }
        if(buff[i] == '&'){
          data[6] = (buff.substring(flag-1, flag)).toInt();
          data[7] = (buff.substring(flag+1, i)).toInt();
        }
        if(buff[i] == '%'){
          data[8] = (buff.substring(1, i)).toInt();
        }
        i ++;
      }
      digitalWrite(12, LOW);
      flag = true;
      
    }
  }
  return data;
}
String sep = "=======================";
bool flag = true;
void loop() {
  
  /***************************************************
   * getbuff[0] dir1
   * getbuff[1] fqc1
   * getbuff[2] dir2
   * getbuff[3] fqc2
   * getbuff[4] dir3
   * getbuff[5] fqc3
   * getbuff[6] dir4
   * getbuff[7] fqc4
   * getbuff[8] acceleration
    ***************************************************/
  
  
  
  int* getbuff = getBuffer(lastdata);
  
  
  if(mode_select){
    F_Lim = 12000;
    motorFrequency(1, FREQ, 1);  // motor number: 1, 2, 3, 4  frquency(speed) direction: 1, 0
    motorFrequency(2, FREQ, 1);
    motorFrequency(3, FREQ, 0);
    motorFrequency(4, FREQ, 0); 
  }
  else{
    F_Lim = 7500;
    motorFrequency(1, FREQ, 0);  // motor number: 1, 2, 3, 4  frquency(speed) direction: 1, 0
    motorFrequency(2, FREQ, 0);
    motorFrequency(3, FREQ, 0);  
    motorFrequency(4, FREQ, 0); 
  }
  
}

