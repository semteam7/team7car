#include <Servo.h>
#include <stdio.h>

extern "C" {
  #include "pb.h"
  #include "pb_encode.h"
  #include "pb_decode.h"
  #include "double_conversion.h" 
  #include "vehiclecommand.pb.h"
  #include "usdata.pb.h"
  #include "irdata.pb.h"
}

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"


//Digital
int SERVO_CONTROL_PIN = 6;
int ESC_PIN = 9;

//Analog
int SERVO_SENSE_PIN = 4;

int RC_STEERING_PIN = 2;//switch?
int RC_THROTTLE_PIN = 4;

int IR_1 = 0;
int IR_2 = 1;
int IR_3 = 2;

int US_1;
int US_2;

int received = 0;

int STEERING_STRAIGHT = 135;
int STEERING_FULL_LEFT = 90;
int STEERING_FULL_RIGHT = 180;
int SPEED_INPUT;
int STEERING_INPUT;

const byte numChars = 80;
#define SERIAL_BUFFER 1000
byte receivedBytes[SERIAL_BUFFER];

boolean newData = false;

Servo esc, steering;


//Serial init
uint8_t send_buffer[66];
pb_ostream_t send_ostream;

uint8_t receive_buffer[128];
int received_bytes;
pb_istream_t receive_istream;


int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
  
  pinMode(ESC_PIN, OUTPUT);
  pinMode(SERVO_CONTROL_PIN, OUTPUT);
  esc.attach(ESC_PIN);
  steering.attach(SERVO_CONTROL_PIN);
   
   
  initSerial();
  //digitalWrite(ledPin, HIGH);
  delay(1000);
  esc.write(90);
  

}


void initSerial()
{
  Serial.begin(57600);
  send_ostream = pb_ostream_from_buffer(send_buffer, sizeof(send_buffer));
}

void sendSerial()
{
  Serial.write(send_buffer, send_ostream.bytes_written);
  send_ostream = pb_ostream_from_buffer(send_buffer, sizeof(send_buffer)); //reset send_ostream
}

int rcThrottleValue, rcSteeringValue;
bool safetyStop = false;

void loop() {
  readFromSerial();

 // sendIrData(2, 10);
 // sendUsData(4, 5);

  //sendSerial();
  //sendSensorData("I",2,10);
  //delay(100);
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

boolean serialDebug = true;
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

void sendSensorData(String type, int sensorId, float sensorReading){
  String sensorData = "S";
  sensorData += sensorId;
  sensorData += sensorReading;
  Serial.println(sensorData);
}
