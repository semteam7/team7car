#include <Servo.h>
#include <stdio.h>

extern "C" {
  #include "pb_encode.h"
  #include "pb_decode.h"
  #include "double_conversion.h" 
  #include "vehiclecontrol.pb.h"
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

void setup() {
  pinMode(ESC_PIN, OUTPUT);
  pinMode(SERVO_CONTROL_PIN, OUTPUT);
  esc.attach(ESC_PIN);
  esc.write(90);
  steering.attach(SERVO_CONTROL_PIN);


  initSerial();
  delay(1000);


}

//Serial init
uint8_t send_buffer[66];
pb_ostream_t send_ostream;

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


void testSteering(){
  setAngle(STEERING_FULL_RIGHT);
  delay(2000);
}

int rcThrottleValue, rcSteeringValue;
bool safetyStop = false;



void loop() {
  readFromSerial();

  sendIrData(2, 10);
  sendUsData(4, 5);

  sendSerial();

  delay(100);
}


void readFromSerial() {
  String command;
  float carSpeed;
  float carAngle;
  if(Serial.available())
  {
    command = Serial.readStringUntil('\n');
    //Serial.println("Received control message: " + command);
    int colonIndex = command.indexOf(';');
    
    carSpeed = command.substring(0,colonIndex).toFloat();
    carAngle = command.substring(colonIndex + 1,command.length()).toFloat();

   //  if (carAngle > 1.5){
     // carAngle =  1.5;
    //}
    //else if (carAngle < -1.5){
      //carAngle = (-1.5);
    //}
    
     carAngle = (carAngle * 57.3)  + 90;

//     carSpeed = (carSpeed * 4.5) + 90;
//     if (carSpeed > 99.1){
//      carSpeed = 99.1;
//      received++;
//    }
//    if (carAngle > 135){
//      carAngle = 135;
//    }
//    if (carAngle < 45){
//      carAngle = 45;
//    }
    // carAngle = (carAngle * 57.3)  + 90;

     if (carSpeed < 2 && carSpeed > 0){
      carSpeed = 98.999;
    }
    else {
     carSpeed = (carSpeed * 4.5) + 90;
     if (carSpeed > 99.00){
      carSpeed = 99.00;
      //received++;
      }
    }
     carAngle = (4.28 * carAngle) + 90;

     if (carAngle > 130) {
      carAngle = 130;
     }
     else if (carAngle < 50){
      carAngle = 50;
     }
    //if (carAngle > 155){
      //carAngle = 155;
    //}
    //if (carAngle < 35){
     // carAngle = 35;
      //}

     //carAngle = ((carAngle - (-6.2)) / (6.2 - (-6.2))) * (135-45) + 40; 
    
   // Serial.println(carAngle);
    
    if (received == 10){
      Serial.println("done 10 times");
       if (carSpeed < 99 && carSpeed > 90){
           updateSpeed(102);
            received = 0;
            delay(300);
      }
    }
    
    Serial.print("A");
    Serial.print(carAngle);
    Serial.print("S");
    Serial.println(carSpeed);

    updateSpeed(carSpeed);
    setAngle(carAngle);
  }

}

void updateSpeed(float inSpeed) {
  esc.write(inSpeed);
  //Serial.println("Speed set to " + inSpeed);
}

void setAngle(float inAngle) {
  steering.write(inAngle);
  //Serial.println("Angle set to " + inAngle);
}

uint8_t pbuffer[50];

void sendProtobufMessage(){

//
//  size_t message_length;
//  bool state;
//
//  scaledcars_VehicleCommand vc = scaledcars_VehicleCommand_init_default;
//
//  vc.speed = test;
//  vc.has_speed = true;
//
//  pb_ostream_t stream = pb_ostream_from_buffer(pbuffer, sizeof(pbuffer));
//  state = pb_encode(&stream, scaledcars_VehicleCommand_fields, &vc);
//
//  if (!state)
//  {
//    Serial.print("Encoding failed: %s\n");
//    Serial.println(PB_GET_ERROR(&stream));
//
//  }
//  // bool pb_get_encoded_size(size_t *size, const pb_field_t fields[], const void *src_struct);
//  Serial.write(buffer, stream.bytes_written);
}

void sendIrData(int sensor_id, int reading){
  scaledcars_IRData ird = scaledcars_IRData_init_zero;
  ird.has_sensor_id = true;
  ird.sensor_id = sensor_id;
  ird.has_reading = true;
  ird.reading = reading;
  pb_encode(&send_ostream, scaledcars_IRData_fields, &ird);
}

void sendUsData(int sensor_id, int reading){
  scaledcars_USData usd = scaledcars_USData_init_zero;
  usd.has_sensor_id = true;
  usd.sensor_id = sensor_id;
  usd.has_reading = true;
  usd.reading = reading;
  pb_encode(&send_ostream, scaledcars_USData_fields, &usd);
}


