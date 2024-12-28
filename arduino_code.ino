#include <SoftwareSerial.h>

/*
 * Car Motor constant pints
 */ 
const int kLeftForward = 2; 
const int kLeftBackward = 3; 
const int kRightForward = 4; 
const int kRightBackward = 5;
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
}


void loop(){
  // enable the HM10 to listen
  HM10.listen();
  // check if available for each char
  if (HM10.available() > 0){
    data_from_bluetooth = HM10.read();
    cmd_data = String(data_from_bluetooth);
    Serial.write(data_from_bluetooth);
  }

  if (cmd_data == kCmdBack){
    digitalWrite(kLeftForward , LOW);
    digitalWrite(kLeftBackward , HIGH);
    digitalWrite(kRightForward , LOW);
    digitalWrite(kRightBackward , HIGH);
    Serial.write("DEBUG: cmd is Back");
  } else if (cmd_data == kCmdFront) {
    digitalWrite(kLeftForward , HIGH);
    digitalWrite(kLeftBackward , LOW);
    digitalWrite(kRightForward , HIGH);
    digitalWrite(kRightBackward , LOW); 
    Serial.write("DEBUG: cmd is Front");
  } else if (cmd_data == kCmdLeft){
    digitalWrite(kLeftForward , HIGH);
    digitalWrite(kLeftBackward , LOW);
    digitalWrite(kRightForward , LOW);
    digitalWrite(kRightBackward , LOW); 
    Serial.write("DEBUG: cmd is Left");
 } else if(cmd_data  == kCmdRight){
    digitalWrite(kLeftForward , LOW);
    digitalWrite(kLeftBackward , LOW);
    digitalWrite(kRightForward , HIGH);
    digitalWrite(kRightBackward , LOW);
    Serial.write("DEBUG: cmd is Right");
 } else if(cmd_data == kCmdStop){
    digitalWrite(kLeftForward , LOW);
    digitalWrite(kLeftBackward , LOW);
    digitalWrite(kRightForward , LOW);
    digitalWrite(kRightBackward , LOW);
    Serial.write("DEBUG: cmd is else - not B, L, R and F. Stop the Car!");
  } else{
    Serial.write("DEBUG: Don't do anything!");
  }

  delay(100);
}


