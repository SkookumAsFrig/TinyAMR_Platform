int lastdata[9] = {0,0,0,0,0,0,0,0,0};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

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
//          Serial.println(data[4]);
//          delayMicroseconds(300);
//          Serial.println(data[5]);
//          delayMicroseconds(300);
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
      
    }
  }
  return data;
}
String sep = "=======================";
void loop() {
  // put your main code here, to run repeatedly:
  int* getbuff = getBuffer(lastdata);
  for(int i = 0; i < 9; i ++){
    Serial.println(getbuff[i]);
    delayMicroseconds(300);
  }
  Serial.println(sep);
}
