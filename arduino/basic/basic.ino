#include <Servo.h>

//Digital
int SERVO_CONTROL_PIN = 6;
int ESC_PIN = 9;

Servo esc, steering;

void setup() {
  
  pinMode(ESC_PIN, OUTPUT);
  esc.attach(ESC_PIN);
  
  pinMode(SERVO_CONTROL_PIN, OUTPUT);
  steering.attach(SERVO_CONTROL_PIN);


  delay(1000);
  esc.write(90);
  steering.write(90);

  Serial.begin(115200);
}

void loop() {
  readFromSerial();
}


void readFromSerial() {
  
  String command;
  float carSpeed;
  float carAngle;
  
  if(Serial.available())
  {
    Serial.setTimeout(100);
    command = Serial.readStringUntil('\n');
    //Serial.println("Received control message: " + command);
    
    int colonIndex = command.indexOf(';');
    if(colonIndex == -1)
    {
      return;
    }
    //Serial.println(colonIndex);
    carSpeed = command.substring(0,colonIndex).toFloat();
    //Serial.println(carSpeed);
   
    carAngle = command.substring(colonIndex + 1,command.length()-2).toFloat();
    //Serial.println(carAngle);

    carAngle = (carAngle * 53.7) + 90;

    // Car Speed limiter
    if (carSpeed < 2 && carSpeed > 0){
      carSpeed = 99;
    } else {
      carSpeed = (carSpeed * 4.5) + 90;
      if (carSpeed > 99.00){
        carSpeed = 99.00;
      }
    }

    // Car angle maximum
    if (carAngle > 130) {
      carAngle = 130;
    }
    else if (carAngle < 50){
      carAngle = 50;
    }

    //Serial.println(carSpeed);
    //Serial.println(carAngle);

    esc.write(carSpeed);
    steering.write(carAngle);
  }
}
