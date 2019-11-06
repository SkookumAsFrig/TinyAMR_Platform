/*
  Trinamic TMC2130 Example

  Other examples/libraries can be found here:
  https://github.com/teemuatlut/TMC2130Stepper
  https://github.com/janelia-arduino/TMC2130
  https://github.com/makertum/Trinamic_TMC2130
*/

#include "SPI.h"

// Note: You also have to connect GND, 5V/VIO and VM.
//       A connection diagram can be found in the schematics.

#define MOSI_PIN 51 //SDI/MOSI (ICSP: 4, Uno: 11, Mega: 51)
#define MISO_PIN 50 //SDO/MISO (ICSP: 1, Uno: 12, Mega: 50)
#define SCK_PIN  52 //CLK/SCK  (ICSP: 3, Uno: 13, Mega: 52)

#define EN_PIN   22  //enable (CFG6)
#define DIR_PIN  28  //direction
#define STEP_PIN  26  //step
#define CS_PIN    24  //chip select

//TMC2130 registers
#define WRITE_FLAG     (1<<7) //write flag
#define READ_FLAG      (0<<7) //read flag
#define REG_GCONF      0x00
#define REG_GSTAT      0x01
#define REG_IHOLD_IRUN 0x10
#define REG_CHOPCONF   0x6C
#define REG_COOLCONF   0x6D
#define REG_DCCTRL     0x6E
#define REG_DRVSTATUS  0x6F
#define REG_TSTEP      0x12

uint8_t tmc_write(uint8_t cmd, uint32_t data)
{
  uint8_t s;

  digitalWrite(CS_PIN, LOW);

  s = SPI.transfer(cmd);
  SPI.transfer((data >> 24UL) & 0xFF) & 0xFF;
  SPI.transfer((data >> 16UL) & 0xFF) & 0xFF;
  SPI.transfer((data >> 8UL) & 0xFF) & 0xFF;
  SPI.transfer((data >> 0UL) & 0xFF) & 0xFF;

  digitalWrite(CS_PIN, HIGH);

  return s;
}

uint8_t tmc_read(uint8_t cmd, uint32_t *data)
{
  uint8_t s;

  tmc_write(cmd, 0UL); //set read address

  digitalWrite(CS_PIN, LOW);

  s = SPI.transfer(cmd);
  *data  = SPI.transfer(0x00) & 0xFF;
  *data <<= 8;
  *data |= SPI.transfer(0x00) & 0xFF;
  *data <<= 8;
  *data |= SPI.transfer(0x00) & 0xFF;
  *data <<= 8;
  *data |= SPI.transfer(0x00) & 0xFF;

  digitalWrite(CS_PIN, HIGH);

  return s;
}

void setup()
{
  //set pins
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW); //LOW or HIGH
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  pinMode(MOSI_PIN, OUTPUT);
  digitalWrite(MOSI_PIN, LOW);
  pinMode(MISO_PIN, INPUT);
  digitalWrite(MISO_PIN, HIGH);
  pinMode(SCK_PIN, OUTPUT);
  digitalWrite(SCK_PIN, LOW);

  //init serial port
  Serial.begin(9600); //init serial port and set baudrate
  while (!Serial); //wait for serial port to connect (needed for Leonardo only)
  Serial.println("\nStart...");

  //init SPI
  SPI.begin();
  //SPI.setDataMode(SPI_MODE3); //SPI Mode 3
  //SPI.setBitOrder(MSBFIRST); //MSB first
  //SPI.setClockDivider(SPI_CLOCK_DIV128); //clk=Fcpu/128
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

  //set TMC2130 config
  tmc_write(WRITE_FLAG | REG_GCONF,      0x00000001UL); //voltage on AIN is current reference
  tmc_write(WRITE_FLAG | REG_IHOLD_IRUN, 0x00001010UL); //IHOLD=0x10, IRUN=0x10
  tmc_write(WRITE_FLAG | REG_CHOPCONF,   0x00008008UL); //native 256 microsteps, MRES=0, TBL=1=24, TOFF=8
  //tmc_write(WRITE_FLAG|REG_CHOPCONF,   0x01008008UL); //128 microsteps, MRES=0, TBL=1=24, TOFF=8
  //tmc_write(WRITE_FLAG|REG_CHOPCONF,   0x02008008UL); // 64 microsteps, MRES=0, TBL=1=24, TOFF=8
  //tmc_write(WRITE_FLAG|REG_CHOPCONF,   0x03008008UL); // 32 microsteps, MRES=0, TBL=1=24, TOFF=8
  //tmc_write(WRITE_FLAG|REG_CHOPCONF,   0x04008008UL); // 16 microsteps, MRES=0, TBL=1=24, TOFF=8
  //tmc_write(WRITE_FLAG|REG_CHOPCONF,   0x05008008UL); //  8 microsteps, MRES=0, TBL=1=24, TOFF=8
  //tmc_write(WRITE_FLAG|REG_CHOPCONF,   0x06008008UL); //  4 microsteps, MRES=0, TBL=1=24, TOFF=8
  //tmc_write(WRITE_FLAG|REG_CHOPCONF,   0x07008008UL); //  2 microsteps, MRES=0, TBL=1=24, TOFF=8
  //tmc_write(WRITE_FLAG|REG_CHOPCONF,   0x08008008UL); //  1 microsteps, MRES=0, TBL=1=24, TOFF=8

  //TMC2130 outputs on (LOW active)
  digitalWrite(EN_PIN, LOW);
}

uint8_t delay_time = 1;
bool dir_sw = true;

void loop()
{
  static uint32_t last_time = 0;
  uint32_t ms = millis();
  uint32_t data;
  uint8_t s;
  if ((ms - last_time)%1000 == 0) {
    s = tmc_read(REG_TSTEP, &data);
    Serial.print("REG_TSTEP: 0x");
    Serial.println(data, HEX);
  }

  if ((ms - last_time) > 5000) //run every 1s
  {
    last_time = ms;
    if (dir_sw){
      digitalWrite(DIR_PIN, LOW);
      dir_sw = false;
    }else{
      digitalWrite(DIR_PIN, HIGH);
      dir_sw = true;
    }
    //show REG_GSTAT
    s = tmc_read(REG_GSTAT, &data);
    Serial.print("GSTAT:     0x0");
    Serial.print(data, HEX);
    Serial.print("\t - ");
    Serial.print("Status: 0x");
    Serial.print(s, HEX);
    if (s & 0x01) Serial.print(" reset");
    if (s & 0x02) Serial.print(" error");
    if (s & 0x04) Serial.print(" sg2");
    if (s & 0x08) Serial.print(" standstill");
    Serial.println(" ");

    //show REG_DRVSTATUS
    s = tmc_read(REG_DRVSTATUS, &data);
    Serial.print("DRVSTATUS: 0x");
    Serial.print(data, HEX);
    Serial.print("\t - ");
    Serial.print("Status: 0x");
    Serial.print(s, HEX);
    if (s & 0x01) Serial.print(" reset");
    if (s & 0x02) Serial.print(" error");
    if (s & 0x04) Serial.print(" sg2");
    if (s & 0x08) Serial.print(" standstill");
    Serial.println(" ");
  }

  //make steps
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(delay_time);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(delay_time);
}


