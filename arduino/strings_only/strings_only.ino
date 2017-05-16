#include <Servo.h>
#include <stdio.h>
#include <Wire.h>
#include "SonarSRF08.h"

//Borrowed from examples
#define GAIN_REGISTER 0x09
#define LOCATION_REGISTER 0x18

#define MOCK     false
#define SENSE    true
#define DEBUG    false
#define ODOMETRY true

#define RADIO_MOTOR_PIN 4  
#define RADIO_SERVO_PIN 5  

#define SPEED_COF 0.20 //Coefficient to control max speed
#define REVERSE_COF 0.65 //Coefficient to control max speed, whilst reversing
#define SPEED_BASELINE 1500 //Neutral value for the speed radio input

//Digital
int SERVO_CONTROL_PIN = 6;
int ESC_PIN = 9;

//Analog
int SERVO_SENSE_PIN = 4;

int RC_STEERING_PIN = 2;//switch?
int RC_THROTTLE_PIN = 4;

int IR_1 = 0; //left_front
int IR_2 = 1; //left_back
int IR_3 = 2; //back

int US_1 = 0xE2 >> 1; // front
int US_2 = 0xE4 >> 1; // front-left

int received = 0;

int STEERING_STRAIGHT = 135;
int STEERING_FULL_LEFT = 90;
int STEERING_FULL_RIGHT = 180;
int SPEED_INPUT;
int STEERING_INPUT;

int IR_DISTANCE_FACTOR = 12.5;
int IR_DISTANCE_CUTOFF = 40;

char US_UNIT = 'c';
SonarSRF08 sonar1(US_1, GAIN_REGISTER, LOCATION_REGISTER);
SonarSRF08 sonar2(US_2, GAIN_REGISTER, LOCATION_REGISTER);

Servo esc, steering;

int ledPin = 13;

int ODOA = 5; // Odometer A. Not used for now
int ODOB = 3; // Odometer B

volatile char odocount=0; // Odometer interrupt counter.
void updateCounter(){ 
  odocount++; 
  if(odocount > 96)
  {
    odocount = 0;
  }
}

void setup() {
  //pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);
  
  pinMode(ESC_PIN, OUTPUT);
  pinMode(SERVO_CONTROL_PIN, OUTPUT);
  pinMode(RADIO_MOTOR_PIN, INPUT);
  pinMode(RADIO_SERVO_PIN, INPUT);
  esc.attach(ESC_PIN);
  steering.attach(SERVO_CONTROL_PIN);
  
  sonar1.begin();
  sonar2.begin();

  if(ODOMETRY)
  {
    attachInterrupt(digitalPinToInterrupt(ODOB), updateCounter, CHANGE);
  }
   
  //digitalWrite(ledPin, HIGH);
  delay(1000);
  esc.write(90);  
}

int rcThrottleValue, rcSteeringValue;
bool safetyStop = false;
long lastRead = 0;
void loop() {  
  readFromSerial();
  //Serial.println("in loop");

  if((lastRead + 100) < millis())
  {
    lastRead = millis();
    if(SENSE){
      sendSensorData();
    }
  }
  
}

void readFromSerial() {
  //digitalWrite(ledPin, LOW);
  String command;
  float carSpeed = 0;
  float carAngle = 0;
  
  if(Serial.available()) {
    while (Serial.available() > 1) {
      Serial.read();
    }

    byte cmd = Serial.read();
    carAngle = ((cmd >> 3) - 16) / 10.0;
    carSpeed = ((7 & cmd) * 6) / 10.0 ;
    
    executeVehicleCommand(carSpeed, carAngle);
  }
}

void error(String reason){
  if(DEBUG)
  {
    Serial.print("Error, reason: ");
    Serial.println(reason);
  }
}

void executeVehicleCommand(float carSpeed, float carAngle)
{
  if (carAngle > 1.5){
      carAngle =  1.5; 
    }
    else if (carAngle < -1.5){
      carAngle = (-1.5);
    }
    carAngle = (carAngle * 57.3)  + 90;

    if     ( carSpeed >  1 ) { carSpeed = 1595; } //1580
    else if( carSpeed >  0 ) { carSpeed = 1590; }
    else if( carSpeed == 0 ) { carSpeed = 1500; }
    else if( carSpeed > -1 ) { carSpeed = 1400; }
    else                     { carSpeed = 1360; }

    if(DEBUG) 
    {
      Serial.print("Setting car speed to : ");
      Serial.println(carSpeed);
      Serial.print("Setting car angle to : ");
      Serial.println(carAngle);
    }
    
    esc.writeMicroseconds(carSpeed);
    steering.write(carAngle);
}

char readIRSensor(int pin)
{
  if(MOCK) return 4;
  
  float v = analogRead(pin) * (5.0 / 1023.0); //scale analog read value to voltage
  float d = IR_DISTANCE_FACTOR / v;           //get distance in centimeters
  if(d > IR_DISTANCE_CUTOFF) { d = -1; }    //if above cutoff, return -1


  return ((char) d) + 31;
}

char readUSSensor(int address)
{  
  if(MOCK) return 3;
  
  int range = -1; 

  Wire.beginTransmission(address);                
  Wire.write(0x00);                               // start command
  Wire.write(0x51);                               // start ranging in cm
  Wire.endTransmission();
 
  delay(70);                                      // Wait for ranging to be complete
  
  Wire.beginTransmission(address);            
  Wire.write(0x02);                               // request ranging data
  Wire.endTransmission();
  
  Wire.requestFrom(address, 2);                   // request 2 bytes from SRF module
  while(Wire.available() < 2);                   
  byte highByte = Wire.read();                          // Get high byte
  byte lowByte = Wire.read();                           // Get low byte
  range = (highByte << 8) + lowByte;

  if(range > 90 || range < 0)
  {
    range = -1;
  }
  
  return ((char) range) + 31;
}

//int getRequestedSpeed(){
//  int pulse = pulseIn(RADIO_MOTOR_PIN, HIGH); //Read pulse rate
//  float scale = pulse > SPEED_BASELINE ? SPEED_COF : REVERSE_COF;
//  /*
//    Scale pulse by relivant speed Coefficient, with SPEED_BASELINE as the origin
//  */
//  int s = pulse - SPEED_BASELINE; 
//  s *= scale; 
//  s += SPEED_BASELINE; 
//  return s;
//}

void sendSensorData(){
  String sensorData = "S";
  sensorData += readIRSensor(IR_1);
  sensorData += readIRSensor(IR_2);
  sensorData += readIRSensor(IR_3);
  sensorData += readUSSensor(US_1);
  sensorData += readUSSensor(US_2);
  if(ODOMETRY){ sensorData += (char)(odocount + 31);}
  
  Serial.println(sensorData);
}

