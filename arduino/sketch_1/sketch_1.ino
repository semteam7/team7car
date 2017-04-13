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

int STEERING_STRAIGHT=135;
int STEERING_FULL_LEFT=90;
int STEERING_FULL_RIGHT=180;
int SPEED_INPUT;
int STEERING_INPUT;

const byte numChars=32;
byte receivedBytes[numChars];

boolean newData = false;


Servo esc, steering;

void setup() {
<<<<<<< HEAD
  pinMode(ESC_PIN, OUTPUT);
  pinMode(SERVO_CONTROL_PIN, OUTPUT);
  pinMode(RC_STEERING_PIN, INPUT);
  pinMode(RC_THROTTLE_PIN, INPUT);
  
  esc.attach(ESC_PIN);
  steering.attach(SERVO_CONTROL_PIN);
=======
    // put your setup code here, to run once:
>>>>>>> 1ab00f6260e8725746c0e50c32bd90a16d41ddcb

  Serial.begin(9600);
}

int rcThrottleValue, rcSteeringValue;
bool safetyStop = false;

void loop() {
<<<<<<< HEAD
  readFromSerial();
  showNewData();
}


void readFromSerial(){
  char terminate = '>';
  if(Serial.available()>0){
    Serial.readBytesUntil(terminate, receivedBytes, numChars);
    newData = true;
  }
}

void showNewData() {
    if (newData == true) {
    Serial.print("This just in ... ");
    for(int i = 0; i<sizeof(receivedBytes)-1; i++){
      Serial.print(receivedBytes[i]);
    }
    Serial.println("");
    newData = false;
 }
}
=======
    // put your main code here, to run repeatedly:

}
>>>>>>> 1ab00f6260e8725746c0e50c32bd90a16d41ddcb
