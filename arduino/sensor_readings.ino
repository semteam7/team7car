
#include <Wire.h>

//Ultrasonic addresses
int us_frontCenter = 113;
int us_frontRight = 114;

//IR pins on Arduino
#define front_righ A0
#define rear_righ A1
#define rear A2

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  String netstring = ReadSensors();
  Serial.println(netstring);
}



/*
 * Reads the sensors
 */
String ReadSensors(){
  int sonar1 = ReadUSSensor(us_frontCenter);  //Read ultrasonic sensors

  int sonar2 = ReadUSSensor(us_frontRight);


      //resd the IRs:  
  int inred1 = VoltageToCm(analogRead(front_righ));  

  int inred2 = VoltageToCm(analogRead(rear_righ));

  int inred3 = VoltageToCm(analogRead(rear));

  String sensorValues = setString(sonar1, sonar2, inred1, inred2, inred3); //sensor values as ints

  String encodedString = encodeNetstring(sensorValues); //Form netstring
  return encodedString;
}

/*
 * Function which uses the Wire library to read from an ultrasonic sensor,
 * given the address of that sensor.
 */
int ReadUSSensor(int address){
  int readValue = 0;
  
  Wire.beginTransmission(address);
  Wire.write(byte(0x00)); //sets register pointer to the command register
  Wire.write(byte(0x51)); //command sensor to measure in cm
  Wire.endTransmission(); //stop transmitting

  delay(70); //Wait for readings to happen

  Wire.beginTransmission(address); //transmit to device
  Wire.write(byte(0x02)); //sets register pointer to echo #1 register (0x02)
  Wire.endTransmission(); //stop transmitting

  Wire.requestFrom(address, 2); //request two bytes from us_frontCenter

  //receiver reading from sensor:
  if(2 <= Wire.available()){ //if 2 bytes were received
    readValue = Wire.read(); //receive high byte (overwrites previous reading
    readValue = readValue << 8; //shift high byte as lower 8 bits
    readValue |= Wire.read(); //receive low byte as lower 8 bits
  }
  
  return readValue; //Return the value
}

/*
 * Takes 5 integer values and separate them with spaces in a string
 */
String setString(int i_1, int i_2, int i_3, int i_4, int i_5){
  String s1 = String(i_1);
  String s2 = String(i_2);
  String s3 = String(i_3);
  String s4 = String(i_4);
  String s5 = String(i_5);

  String valueS = s1 + ", " + s2 + ", " + s3 + ", " + s4 + ", " + s5;
  return valueS;
}

/*
 * Takes a string as an argument and encodes it as a netstring
 */
String encodeNetstring(String string){
  int len = string.length();
  if (len <= 0){
    return "empty";
  }
  return String(string + ",");
}

/*
 * Takes the voltage respresenation read from an analog port, transforms it into
 * voltages, and calculates and returns the distance in cm
 */
int VoltageToCm(int voltrep){
  float voltage = voltrep * (5.0 / 1023.0);
  float value = 12.5 / voltage;
  int i = (int) value;
  if (value > 45) return -1;
  return i;
}