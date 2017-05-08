#include <Servo.h>
#include <stdio.h>
#include <Wire.h>
#include "SonarSRF08.h"

//Borrowed from examples
#define GAIN_REGISTER 0x09
#define LOCATION_REGISTER 0x8C
#define MOCK false

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
SonarSRF08 sonar1(US_1, GAIN_REGISTER, LOCATION_REGISTER);//sonar2(US_2);

Servo esc, steering;

int ledPin = 13;



void setup() {
  //pinMode(ledPin, OUTPUT);
  
  pinMode(ESC_PIN, OUTPUT);
  pinMode(SERVO_CONTROL_PIN, OUTPUT);
  esc.attach(ESC_PIN);
  steering.attach(SERVO_CONTROL_PIN);
  
  sonar1.begin();
 // sonar2.begin();
   
  initSerial();
  //digitalWrite(ledPin, HIGH);
  delay(1000);
  esc.write(90);
  

}


void initSerial()
{
  Serial.begin(115200);
}

int rcThrottleValue, rcSteeringValue;
bool safetyStop = false;
long lastRead = 0;
void loop() {
  readFromSerial();

  if((lastRead + 100) < millis())
  {
    lastRead = millis();
    readSensors();
  }
  
}

void readFromSerial() {
  //digitalWrite(ledPin, LOW);
  String command;
  float carSpeed;
  float carAngle;
  
  if(Serial.available()){
    Serial.setTimeout(100);
    command = Serial.readStringUntil('\n');
    int startIndex = command.indexOf('C');
    if(startIndex == -1){
      error("Malformed command, no start");
      return;
      }
    
    command = command.substring(startIndex, command.length()-1); //trim command

    int delimiter = command.indexOf(';');

    if(delimiter == -1){
      error("Malformed command, no delim");
      return;
    }
    
    carSpeed = command.substring(1, delimiter).toFloat();
    carAngle = command.substring(delimiter+1).toFloat();
    //digitalWrite(ledPin, HIGH);
    executeVehicleCommand(carSpeed, carAngle);
  }

}

boolean serialDebug = false;
void error(String reason){
  if(serialDebug)
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
    
    esc.write(carSpeed);
    steering.write(carAngle);
}

void readSensors(){
  //Read IR Sensors
  sendSensorData();
}

float readIRSensor(int pin)
{
  if(MOCK) return 4;
  
  float v = analogRead(pin) * (5.0 / 1023.0); //scale analog read value to voltage
  float d = IR_DISTANCE_FACTOR / v;           //get distance in centimeters
  if(d > IR_DISTANCE_CUTOFF) { return 2; }   //if above cutoff, return -1
  return d;
}

float readUSSensor(int address)
{  
  if(MOCK) return 0;
  
  int range = 0; 
  
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
  
  return range;
}


void sendSensorData(){
  String sensorData = "S";
  sensorData += ":";
  sensorData += readIRSensor(IR_1);
  sensorData += ":";
  sensorData += readIRSensor(IR_2);
  sensorData += ":";
  sensorData += readIRSensor(IR_3);
  sensorData += ":";
  sensorData += readUSSensor(US_1);
  sensorData += ":";
  sensorData += readUSSensor(US_2);
  
  
  Serial.println(sensorData);
}
