#include <Servo.h>

extern "C" {
  #include "pb_encode.h"
  #include "pb_decode.h"
  #include "double_conversion.h" 
  #include "vehiclecontrol.pb.h"
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

uint8_t buffer[128];
size_t message_length;
bool state;

Servo esc, steering;

void setup() {
  pinMode(ESC_PIN, OUTPUT);
  pinMode(SERVO_CONTROL_PIN, OUTPUT);
  Serial.begin(57600);

  uint64_t test = float_to_double(50.0);

  Serial.println("check me out");


  scaledcars_VehicleCommand vc = scaledcars_VehicleCommand_init_zero;
  
  vc.speed = test;
  vc.acceleration = test;
  
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  state = pb_encode(&stream, scaledcars_VehicleCommand_fields, &vc);

  message_length = stream.bytes_written;


  Serial.print("speed ");
  Serial.println(double_to_float(vc.speed));

  Serial.print("message length ");
  Serial.println(message_length);
  Serial.write(buffer, 100);

  if (!state)
  {
      Serial.print("Encoding failed: %s\n");
      Serial.println(PB_GET_ERROR(&stream));
      
  }
  
  esc.attach(ESC_PIN);
  steering.attach(SERVO_CONTROL_PIN);

  Serial.begin(57600);
}

void testSteering(){
  setAngle(STEERING_FULL_RIGHT);
  delay(2000);
}

int rcThrottleValue, rcSteeringValue;
bool safetyStop = false;

void loop() {
  readFromSerial();
}


void readFromSerial() {

  while(Serial.available())
  {
    Serial.write(Serial.read());
    
  }
  String command;
  float carSpeed;
  float carAngle;
  if(Serial.available() > 0)
  {
    command = Serial.readStringUntil('\n');
    //Serial.println("Received control message: " + command);
    int colonIndex = command.indexOf(';');
    carSpeed = command.substring(0,colonIndex).toFloat();
    carAngle = command.substring(colonIndex + 1,command.length()).toFloat();

     carSpeed = (carSpeed * 10) + 90;
     if (carSpeed > 100){
      carSpeed = 100;
    }
    carAngle += 90;
    
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
