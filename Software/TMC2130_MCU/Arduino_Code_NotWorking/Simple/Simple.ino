/**
 * Author Teemu MÃ¤ntykallio
 * Initializes the library and turns the motor in alternating directions.
 * Need to install https://github.com/teemuatlut/TMC2130Stepper for this to work, verified functionality on Arduino Uno
*/
//driver 1
#define EN1_PIN   38  //enable (CFG6)
#define DIR1_PIN  44  //direction
#define STEP1_PIN  42  //step
#define CS1_PIN    40  //chip select
//driver 2
#define EN2_PIN   49  //enable (CFG6)
#define DIR2_PIN  48  //direction
#define STEP2_PIN  46  //step
#define CS2_PIN    53  //chip select
//driver 3
#define EN3_PIN   36  //enable (CFG6)
#define DIR3_PIN  30  //direction
#define STEP3_PIN  32  //step
#define CS3_PIN    34  //chip select
//driver 4
#define EN4_PIN   22  //enable (CFG6)
#define DIR4_PIN  28  //direction
#define STEP4_PIN  26  //step
#define CS4_PIN    24  //chip select

bool dir = true;

#include <TMC2130Stepper.h>
TMC2130Stepper driver1 = TMC2130Stepper(EN1_PIN, DIR1_PIN, STEP1_PIN, CS1_PIN);
TMC2130Stepper driver2 = TMC2130Stepper(EN2_PIN, DIR2_PIN, STEP2_PIN, CS2_PIN);
TMC2130Stepper driver3 = TMC2130Stepper(EN3_PIN, DIR3_PIN, STEP3_PIN, CS3_PIN);
TMC2130Stepper driver4 = TMC2130Stepper(EN4_PIN, DIR4_PIN, STEP4_PIN, CS4_PIN);
void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Start...");
  SPI.begin();
  pinMode(MISO, INPUT_PULLUP);
  driver1.begin();      // Initiate pins and registeries
  driver1.rms_current(600);   // Set stepper current to 600mA. The command is the same as command TMC2130.setCurrent(600, 0.11, 0.5);
  //driver1.stealthChop(1);   // Enable extremely quiet stepping
  driver2.begin();      // Initiate pins and registeries
  driver2.rms_current(600);   // Set stepper current to 600mA. The command is the same as command TMC2130.setCurrent(600, 0.11, 0.5);
  driver2.stealthChop(1);   // Enable extremely quiet stepping
  driver3.begin();      // Initiate pins and registeries
  driver3.rms_current(600);   // Set stepper current to 600mA. The command is the same as command TMC2130.setCurrent(600, 0.11, 0.5);
  driver3.stealthChop(1);   // Enable extremely quiet stepping
  driver4.begin();      // Initiate pins and registeries
  driver4.rms_current(600);   // Set stepper current to 600mA. The command is the same as command TMC2130.setCurrent(600, 0.11, 0.5);
  //driver4.stealthChop(1);   // Enable extremely quiet stepping
  digitalWrite(EN1_PIN, LOW);
  digitalWrite(EN2_PIN, LOW);
  digitalWrite(EN3_PIN, LOW);
  digitalWrite(EN4_PIN, LOW);
}

uint8_t ms_delay = 10;

void loop() {
  digitalWrite(STEP1_PIN, HIGH);
//  digitalWrite(STEP2_PIN, HIGH);
//  digitalWrite(STEP3_PIN, HIGH);
  digitalWrite(STEP4_PIN, HIGH);
  delayMicroseconds(ms_delay);
  digitalWrite(STEP1_PIN, LOW);
//  digitalWrite(STEP2_PIN, LOW);
//  digitalWrite(STEP3_PIN, LOW);
  digitalWrite(STEP4_PIN, LOW);
  delayMicroseconds(ms_delay);
  uint32_t ms = millis();
  static uint32_t last_time = 0;
  if ((ms - last_time) > 5000) {
    Serial.println(driver1.microsteps());
    Serial.println(driver2.microsteps());
    Serial.println(driver3.microsteps());
    Serial.println(driver4.microsteps());
    Serial.println(driver4.run_current());
    if (dir) {
      Serial.println("Dir -> 0");
      driver1.shaft_dir(0);
      driver2.shaft_dir(0);
      driver3.shaft_dir(0);
      driver4.shaft_dir(0);
    } else {
      Serial.println("Dir -> 1");
      driver1.shaft_dir(1);
      driver2.shaft_dir(1);
      driver3.shaft_dir(1);
      driver4.shaft_dir(1);
    }
    dir = !dir;
    last_time = ms;
  }
}

