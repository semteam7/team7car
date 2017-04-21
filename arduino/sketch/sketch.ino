#include <Servo.h>

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
  if(Serial.available() > 0)
  {
    command = Serial.readStringUntil('\n');
    Serial.println("Received control message: " + command);  
  }
  
}

void updateSpeed(int inSpeed) {
  esc.write(inSpeed);
  Serial.println("Speed set to " + inSpeed);
}

void setAngle(int inAngle) {
  steering.write(inAngle);
  Serial.println("Angle set to " + inAngle);
}
