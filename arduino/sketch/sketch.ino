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
char receivedBytes[numChars];

boolean newData = false;


Servo esc, steering;

void setup() {
  pinMode(ESC_PIN, OUTPUT);
  pinMode(SERVO_CONTROL_PIN, OUTPUT);

  esc.attach(ESC_PIN);
  steering.attach(SERVO_CONTROL_PIN);

  Serial.begin(9600);
  Serial.println("Honey I'm hoooome!");
}

int rcThrottleValue, rcSteeringValue;
bool safetyStop = false;

void loop() {
  readFromSerial();
  showNewData();
}


void readFromSerial() {
  char terminate = '\n';
  if (Serial.available() > 0) {
    Serial.readBytesUntil(terminate, receivedBytes, numChars);
    newData = true;
  } else {
    
  }
}

//Speed: Steeringwheelangle \n

void parseData(char recievedBytes[]){
  char tmp[4];
  for(int i = 0; i < sizeof(receivedBytes); i++){
    int x = 0;
    if (i > 1){
      tmp[x] = receivedBytes[i];
      x++;
    }
  }
  int n;
  n = atoi(tmp);
  setAngle(n);
  
}

void showNewData() {
  if (newData == true) {
    Serial.print("This just in ... ");
    for (int i = 0; i < sizeof(receivedBytes) - 1; i++) {
      Serial.print(receivedBytes[i]);
    }
    Serial.println("");
    newData = false;
    receivedBytes[0] = (char)0;
  }
}
//name setSpeed already used by something
void updateSpeed(int inSpeed) {
  esc.write(inSpeed);
  Serial.println("Speed set to " + inSpeed);
}

void setAngle(int inAngle) {
  steering.write(inAngle);
  Serial.println("Angle set to " + inAngle);
}

