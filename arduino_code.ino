#include <SoftwareSerial.h>

/*
 * Car Motor constant pints
 */ 
const int kLeftForward = 2; 
const int kLeftBackward = 3; 
const int kRightForward = 4; 
const int kRightBackward = 5;
/*
 * Leds pins
 */
const int kBackWhite = 6;
const int kLeftGreen = 7;
const int kFrontRed = 8;
const int kRightYellow = 9;
/*
 * Ultrasonic sensor pins and data
 */
const int kEchoPin = 11; 
const int kTriggerPin = 12;
long duration, cm;
/*
 * Buzzer Pin
 */
const int kBuzzerPin = 13; 
/*
 * Flame Pins and data.
 */
const int kFlameSensorRight = A0;
const int kFlameSensorLeft = A1;
const int kThresholdTemp = 100;
int flameSensorValueLeft = 0;
int flameSensorValueRight = 0;

/*
 * Baud rate for Serials.
 */
const int kBaudRate = 9600;
/*
 * Bluetooth configurations and variables.
 * When Bluetooth gets  "F" - go front, "B" - back, "L" - left, "R" - Right, "S" - Stop. 
 * Otherwise, it will do nothing.
 */
const byte kRxPin = 0;
const byte kTxPin = 1;
const String kCmdStop = "S";
const String kCmdFront = "F";
const String kCmdLeft = "L";
const String kCmdRight = "R";
const String kCmdBack = "B";
char data_from_bluetooth;
String cmd_data = "";
SoftwareSerial HM10(kRxPin, kTxPin);

void setup(){
  Serial.begin(kBaudRate);
  Serial.println("HM10 serial start");
  // begin work wtih bluetooth
  // set HM10 bluetooth Serail at BAUD RATE
  HM10.begin(kBaudRate);

  //car motors 
  pinMode(kLeftForward , OUTPUT);
  pinMode(kLeftBackward , OUTPUT);
  pinMode(kRightForward , OUTPUT);
  pinMode(kRightBackward , OUTPUT);

  digitalWrite(kLeftForward, LOW);
  digitalWrite(kLeftBackward, LOW);
  digitalWrite(kRightForward , LOW);
  digitalWrite(kRightBackward, LOW);

  // leds
  pinMode(kLeftGreen, OUTPUT);
  pinMode(kBackWhite, OUTPUT);
  pinMode(kRightYellow, OUTPUT);
  pinMode(kFrontRed, OUTPUT); 

  digitalWrite(kLeftGreen, LOW);
  digitalWrite(kBackWhite, LOW);
  digitalWrite(kRightYellow, LOW);
  digitalWrite(kFrontRed, LOW);

  //Ultrasonic Sensor
  pinMode(kTriggerPin, OUTPUT);
  pinMode(kEchoPin, INPUT);

  // Buzzer
  pinMode(kBuzzerPin, OUTPUT); // Set buzzer - pin 13 as an output

buzzer_on();

}

/*
 * ultrasonic_sensor_measure measures the distance from the sensor.
 * If the distance is less than 10cm then stop the car.
 */ 
void ultrasonic_sensor_measure(){
  digitalWrite(kTriggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(kTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(kTriggerPin, LOW);
  duration = pulseIn(kEchoPin, HIGH);
  cm = (duration/2) / 29.1;
  Serial.print(cm);
  Serial.println("cm");
  delay(100);
  if(cm <10){
   cmd_data = "S"; 
  }
}

/*
 * trun on buzzer sound for 1 sec with 1KHZ signal and then turn-off.
 */ 
void buzzer_on(){
  tone(kBuzzerPin, 1000);
  delay(1000);
  noTone(kBuzzerPin);
  delay(1000);
}

/*
 * measure the flmae sensor right value, if it less than kThresholdTemp then buzzer on.
 */
void flame_sensor_right_measure(){
  flameSensorValueRight = analogRead(kFlameSensorRight);
  if (flameSensorValueRight < kThresholdTemp){
    Serial.println("Fire Detected");
    buzzer_on();
    }
}

/*
 * measure the flmae sensor left value, if it less than kThresholdTemp then buzzer on.
 */
void flame_sensor_left_measure(){
  flameSensorValueLeft = analogRead(kFlameSensorLeft);
  if (flameSensorValueLeft < kThresholdTemp){
    Serial.println("Fire Detected");
    buzzer_on();
    }
}

void loop(){
  
  flame_sensor_right_measure();
  flame_sensor_left_measure();

  // enable the HM10 to listen
  HM10.listen();
  // check if available for each char
  if (HM10.available() > 0){
    data_from_bluetooth = HM10.read();
    cmd_data = String(data_from_bluetooth);
    Serial.write(data_from_bluetooth);
  }

  ultrasonic_sensor_measure();

  if (cmd_data == kCmdBack){
    digitalWrite(kLeftForward , LOW);
    digitalWrite(kLeftBackward , HIGH);
    digitalWrite(kRightForward , LOW);
    digitalWrite(kRightBackward , HIGH);
    Serial.write("DEBUG: cmd is Back");
    //leds
    digitalWrite(kBackWhite, HIGH);
    digitalWrite(kRightYellow, LOW);
    digitalWrite(kLeftGreen, LOW);
    digitalWrite(kFrontRed, LOW);
  } else if (cmd_data == kCmdFront) {
    digitalWrite(kLeftForward , HIGH);
    digitalWrite(kLeftBackward , LOW);
    digitalWrite(kRightForward , HIGH);
    digitalWrite(kRightBackward , LOW); 
    Serial.write("DEBUG: cmd is Front");
    //leds
    digitalWrite(kRightYellow, LOW);
    digitalWrite(kLeftGreen, LOW);
    digitalWrite(kBackWhite, LOW);
    digitalWrite(kFrontRed, HIGH);
  } else if (cmd_data == kCmdLeft){
    digitalWrite(kLeftForward , HIGH);
    digitalWrite(kLeftBackward , LOW);
    digitalWrite(kRightForward , LOW);
    digitalWrite(kRightBackward , LOW); 
    Serial.write("DEBUG: cmd is Left");
    //leds
    digitalWrite(kLeftGreen, HIGH);
    digitalWrite(kRightYellow, LOW);
    digitalWrite(kBackWhite, LOW);
    digitalWrite(kFrontRed, LOW);
 } else if(cmd_data  == kCmdRight){
    digitalWrite(kLeftForward , LOW);
    digitalWrite(kLeftBackward , LOW);
    digitalWrite(kRightForward , HIGH);
    digitalWrite(kRightBackward , LOW);
    Serial.write("DEBUG: cmd is Right");
    //leds 
    digitalWrite(kRightYellow, HIGH);
    digitalWrite(kLeftGreen, LOW);
    digitalWrite(kBackWhite, LOW);
    digitalWrite(kFrontRed, LOW);
 } else if(cmd_data == kCmdStop){
    digitalWrite(kLeftForward , LOW);
    digitalWrite(kLeftBackward , LOW);
    digitalWrite(kRightForward , LOW);
    digitalWrite(kRightBackward , LOW);
    Serial.write("DEBUG: cmd is else - not B, L, R and F. Stop the Car!");
    //leds
    digitalWrite(kLeftGreen, LOW);
    digitalWrite(kBackWhite, LOW);
    digitalWrite(kRightYellow, LOW);
    digitalWrite(kFrontRed, LOW);
      
  } else{
    Serial.write("DEBUG: Don't do anything!");
  }

  delay(100);
}


