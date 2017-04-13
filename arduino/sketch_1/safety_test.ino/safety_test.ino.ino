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

int US_1 = ;
int US_2 = ;

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
  pinMode(ESC_PIN, OUTPUT);
  pinMode(SERVO_CONTROL_PIN, OUTPUT);
  pinMode(RC_STEERING_PIN, INPUT);
  pinMode(RC_THROTTLE_PIN, INPUT);
  
  esc.attach(ESC_PIN);
  steering.attach(SERVO_CONTROL_PIN);

  attachInterrupt(digitalPinToInterrupt(RC_STEERING_PIN), safetyInterrupt, RISING);

  steering.write(STEERING_STRAIGHT);
  delay(500);
/*
  esc.write(90);
  delay(3000);
  esc.write(100);
  delay(500);
  esc.write(70);
  delay(500);
  esc.write(90);

  
  
  delay(2000);
  steering.write(STEERING_FULL_RIGHT);
  delay(2000);
  steering.write(STEERING_FULL_LEFT);
  delay(2000);
  steering.write(STEERING_STRAIGHT);
  delay(2000);
*/
  Serial.begin(9600);
}

int rcThrottleValue, rcSteeringValue;
bool safetyStop = false;

void loop() {
  if(safetyStop != true)
  {
    esc.write(90);
    delay(3000);
    esc.write(100);
    delay(3000);
    esc.write(70);
    delay(3000);
    esc.write(90);
      
    // int steeringValue, throttleValue;
    /*
    while (Serial.available() > 0) {
      steeringValue = Serial.parseInt();
      Serial.println("Setting steering " );
  
      throttleValue = Serial.parseInt();
      Serial.println("Setting throttle");
  
  
      if (Serial.read() == '\n') {
        steering.write(steeringValue);
        esc.write(throttleValue);
      }
  */
    rcThrottleValue = pulseIn(RC_THROTTLE_PIN, HIGH);
    rcSteeringValue = pulseIn(RC_STEERING_PIN, HIGH);
  
    //Serial.println(String() + " " + String());
    Serial.println(rcThrottleValue);
  
    if(rcThrottleValue < 1400 || rcThrottleValue > 1700)
    {
      int offset = 1570;
      
      int throttleValue = 90+ (((rcThrottleValue - offset)/500.0) * 18);
      Serial.println("Setting esc to " + String(throttleValue));
     
      esc.write(throttleValue);
   
     }
   else
    {
      esc.write(90);    
    }
  
    if(rcSteeringValue < 1400 || rcSteeringValue > 1700)
    {
      int offset = 1550;
      float steeringValue = 135 + (((rcSteeringValue - offset)/501.0) * 45 );
      
      Serial.println("Setting steering to " + String(steeringValue));
      steering.write(steeringValue);
    }
    else
    {
       steering.write(STEERING_STRAIGHT);
      //esc.write(90);    
    }
  }
  else
  {
    esc.write(90);
  }
}

/**
 * Might be useful later
 
   void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
 
    if (Serial.available() > 0) {
        while (Serial.available() > 0 && newData == false) {
           rc = Serial.read();

           if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
           }
           else {
                receivedChars[ndx] = '\0'; // terminate the string
                ndx = 0;
                newData = true;
           }
        }
    }
}*/

void readFromSerial(){
  if(Serial.available()>0){
    receivedBytes = Serial.readBytesUntil();
    newData = true;
  }
}

void showNewData() {
    if (newData == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedChars);
    newData = false;
 }
}

void safetyInterrupt()
{
  esc.write(90);
  safetyStop = true;
  
}


