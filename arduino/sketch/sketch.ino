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

int received = 0;

int STEERING_STRAIGHT = 90;
int STEERING_FULL_LEFT = 45 ;
int STEERING_FULL_RIGHT = 135;
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
  String command;
  float carSpeed;
  float carAngle;
  
  if(Serial.available())
  {
    command = Serial.readStringUntil('\n');
    Serial.println(" liturulurulruly");
    //Serial.println("Received control message: " + command);
    int colonIndex = command.indexOf(';');
    carSpeed = command.substring(0,colonIndex).toFloat();
    carAngle = command.substring(colonIndex + 1,command.length()).toFloat();

     carSpeed = (carSpeed * 4.5) + 90;
     if (carSpeed > 99.1){
      carSpeed = 99.1;
    }
   // if (carAngle > 1.5){
     // carAngle =  1.5; 
    //}

    //else if (carAngle < -1.5){
      //carAngle = (-1.5);
    //}
    //Serial.print("Before Calculation ");
    //Serial.println(carAngle);
   // carAngle = (carAngle * 57.3)  + 90;

    if (carAngle > 135){
      carAngle = 135;
    }
    if (carAngle < 45){
      carAngle = 45;
    }
    //carAngle = ((carAngle - (-6.2)) / (6.2 - (-6.2))) * (135-45) + 40; 
    
   // Serial.println(carAngle);
    
    received++;
    if (received == 10){
       if (carSpeed < 99 && carSpeed > 90){
      updateSpeed(102);
      received = 0;
      }
    }
    delay(300);
    
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
