#include "HX711.h" 
byte pinData = 3;
byte pinClk = 2;
int vib_pin=7;
int led_pin=13;
const int xInput = A0;
const int yInput = A1;
const int zInput = A2;
int RawMin = 0;
int RawMax = 1023;
const int sampleSize = 10;
HX711 scale;
float calibration_factor = 137326; 
int trigPin = 9;    // TRIG pin
int echoPin = 8;    // ECHO pin
//
float duration_us, distance_cm;
void setup() {
 Serial.begin(9600);
 Serial.println("HX711 calibration");
 Serial.println("Remove all weight from scale");
 Serial.println("After readings begin, place known weight on scale");
 Serial.println("Press a,s,d,f to increase calibration factor by 10,100,1000,10000 respectively");
 Serial.println("Press z,x,c,v to decrease calibration factor by 10,100,1000,10000 respectively");

 scale.begin(pinData, pinClk);
 scale.set_scale();
 scale.tare(); 

 long zero_factor = scale.read_average(); //Get a baseline reading
 Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
 Serial.println(zero_factor);
 pinMode(vib_pin,INPUT);
 pinMode(led_pin,OUTPUT);
 analogReference(EXTERNAL);
 Serial.begin(9600);
}
void loop() {
 scale.set_scale(calibration_factor); //Adjust to this calibration factor
  digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
//
//   measure duration of pulse from ECHO pin
 duration_us = pulseIn(echoPin, HIGH);
//
//  // calculate the distance
 distance_cm = 0.017 * duration_us;

 // print the value to Serial Monitor
 Serial.print("distance: ");
 Serial.print(distance_cm);
 Serial.println(" cm");
 delay(500);
 if(distance_cm<0.92){
    Serial.println("ther's a crack");
 }
    else{
     ("No cracks found");
    }

 Serial.print("Reading: ");
 Serial.print(scale.get_units(), 2);
 Serial.print(" kgs"); //Change this to grams and re-adjust the calibration factor if you follow SI units like a sane person
 Serial.print(" calibration_factor: ");
 Serial.print(calibration_factor);
 Serial.println();

 if(Serial.available())
 {
   char temp = Serial.read();
   if(temp == '+' || temp == 'a')
     calibration_factor += 10;
   else if(temp == '-' || temp == 'z')
     calibration_factor -= 10;
   else if(temp == 's')
     calibration_factor += 100;  
   else if(temp == 'x')
     calibration_factor -= 100;  
   else if(temp == 'd')
     calibration_factor += 1000;  
   else if(temp == 'c')
     calibration_factor -= 1000;
   else if(temp == 'f')
     calibration_factor += 10000;  
   else if(temp == 'v')
     calibration_factor -= 10000;  
   else if(temp == 't')
     scale.tare();  //Reset the scale to zero
 }
 int val;
 int xRaw = analogRead(xInput);
 int yRaw = analogRead(yInput);
 int zRaw = analogRead(zInput);
 long xScaled = map(xRaw, RawMin, RawMax, -3000, 3000);
 long yScaled = map(yRaw, RawMin, RawMax, -3000, 3000);
 long zScaled = map(zRaw, RawMin, RawMax, -3000, 3000);
 float xAccel = xScaled / 1000.0;
 float yAccel = yScaled / 1000.0;
 float zAccel = zScaled / 1000.0;
  val=digitalRead(vib_pin);
 if(val==1)
 {
   Serial.print("Vibration Detected, Please send Rescue");
   digitalWrite(led_pin,HIGH);
   delay(300);
   digitalWrite(led_pin,LOW);
   delay(300);
   digitalWrite(led_pin,HIGH);
   delay(300);
   digitalWrite(led_pin,LOW);
   delay(300);
  }
  else
  digitalWrite(led_pin,LOW);
 

 Serial.println("X, Y, Z  :: ");
 Serial.print(xRaw);
 Serial.print(", ");
 Serial.print(yRaw);
 Serial.print(", ");
 Serial.print(zRaw);
 Serial.print(" :: ");
 Serial.print(xAccel,0);
 Serial.print("G, ");
 Serial.print(yAccel,0);
 Serial.print("G, ");
 Serial.print(zAccel,0);
 Serial.println("G");

 delay(200);
}

int ReadAxis(int axisPin)
{
 long reading = 0;
 analogRead(axisPin);
 delay(1);
 for (int i = 0; i < sampleSize; i++)
 {
 reading += analogRead(axisPin);
 }
 return reading/sampleSize;
}
}






/*

  ADXL3xx

  Reads an Analog Devices ADXL3xx accelerometer and communicates the

  acceleration to the computer. The pins used are designed to be easily

  compatible with the breakout boards from SparkFun, available from:

  http://www.sparkfun.com/commerce/categories.php?c=80

  The circuit:

  - analog 0: accelerometer self test

  - analog 1: z-axis

  - analog 2: y-axis

  - analog 3: x-axis

  - analog 4: ground

  - analog 5: vcc

  created 2 Jul 2008

  by David A. Mellis

  modified 30 Aug 2011

  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/ADXL3xx

*/

// these constants describe the pins. They won't change:

const int groundpin = 18;             // analog input pin 4 -- ground

const int powerpin = 19;              // analog input pin 5 -- voltage

const int xpin = A3;                  // x-axis of the accelerometer

const int ypin = A2;                  // y-axis

const int zpin = A1;                  // z-axis (only on 3-axis models)

void setup() {

  // initialize the serial communications:

  Serial.begin(9600);

  // Provide ground and power by using the analog inputs as normal digital pins.

  // This makes it possible to directly connect the breakout board to the

  // Arduino. If you use the normal 5V and GND pins on the Arduino,

  // you can remove these lines.

  pinMode(groundpin, OUTPUT);

  pinMode(powerpin, OUTPUT);

  digitalWrite(groundpin, LOW);

  digitalWrite(powerpin, HIGH);
}

void loop() {

  // print the sensor values:

  Serial.print(analogRead(xpin));

  // print a tab between values:

  Serial.print("\t");

  Serial.print(analogRead(ypin));

  // print a tab between values:

  Serial.print("\t");

  Serial.print(analogRead(zpin));

  Serial.println();

  // delay before next reading:

  delay(100);
}

const int trigPin = 2;
const int echoPin = 4;
long duration;
int distance;

const int flexPin = 34;

 // HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;

HX711 scale;


void setup() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
 
  
   // Read the analog voltage from the flex sensor
  int flexValue = analogRead(flexPin);
  
  // Convert the analog reading to a voltage (assuming a 3.3V reference voltage)
  float voltage = (flexValue / 4095.0) * 3.3;  // 4095 is the maximum ADC value for a 12-bit ADC
  
  // Print the raw analog reading and the voltage to the serial monitor
  Serial.print("Flex Value: ");
  Serial.print(flexValue);
  Serial.print("\tVoltage: ");
  Serial.println(voltage, 2);  // Display voltage with 2 decimal places


  
  delay(1000); 
}



#include "HX711.h" 
byte pinData = 16;
byte pinClk = 4;
int vib_pin=7;
int led_pin=13;
const int xInput = A0;
//const int yInput = A1;
//const int zInput = A2;
int RawMin = 0;
int RawMax = 1023;
const int sampleSize = 10;
HX711 scale;
float calibration_factor = 137326; 
void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press a,s,d,f to increase calibration factor by 10,100,1000,10000 respectively");
  Serial.println("Press z,x,c,v to decrease calibration factor by 10,100,1000,10000 respectively");
  scale.begin(pinData, pinClk);
  scale.set_scale();
  scale.tare(); 
 
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  pinMode(vib_pin,INPUT);
  pinMode(led_pin,OUTPUT);
//  analogReference(EXTERNAL);
  Serial.begin(9600);
}
void loop() {
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
 
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 2);
  Serial.print(" kgs"); //Change this to grams and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
 
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
    else if(temp == 's')
      calibration_factor += 100;  
    else if(temp == 'x')
      calibration_factor -= 100;  
    else if(temp == 'd')
      calibration_factor += 1000;  
    else if(temp == 'c')
      calibration_factor -= 1000;
    else if(temp == 'f')
      calibration_factor += 10000;  
    else if(temp == 'v')
      calibration_factor -= 10000;  
    else if(temp == 't')
      scale.tare();  //Reset the scale to zero
  }
  int val;
  int xRaw = analogRead(xInput);
//  int yRaw = analogRead(yInput);
//  int zRaw = analogRead(zInput);
  long xScaled = map(xRaw, RawMin, RawMax, -3000, 3000);
//  long yScaled = map(yRaw, RawMin, RawMax, -3000, 3000);
//  long zScaled = map(zRaw, RawMin, RawMax, -3000, 3000);
  float xAccel = xScaled / 1000.0;
//  float yAccel = yScaled / 1000.0;
//  float zAccel = zScaled / 1000.0;
   val=digitalRead(vib_pin);
  if(val==1)
  {
    Serial.print("Vibration Detected, Please send Rescue");
    digitalWrite(led_pin,HIGH);
    delay(300);
    digitalWrite(led_pin,LOW);
    delay(300);
    digitalWrite(led_pin,HIGH);
    delay(300);
    digitalWrite(led_pin,LOW);
    delay(300);
   }
   else
   digitalWrite(led_pin,LOW);
  

  Serial.println("X, Y, Z  :: ");
  Serial.print(xRaw);
  Serial.print(", ");
  Serial.print(yRaw);
  Serial.print(", ");
  Serial.print(zRaw);
  Serial.print(" :: ");
  Serial.print(xAccel,0);
  Serial.print("G, ");
  Serial.print(yAccel,0);
  Serial.print("G, ");
  Serial.print(zAccel,0);
  Serial.println("G");

  delay(200);
}

int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
  reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}



//Ulttrasonic

int trigPin = 9;    // TRIG pin
int echoPin = 8;    // ECHO pin
//
float duration_us, distance_cm;

void setup() {
  // begin serial port
  Serial.begin (9600);

  // configure the trigger pin to output mode
  pinMode(trigPin, OUTPUT);
  // configure the echo pin to input mode
  pinMode(echoPin, INPUT);
}
//
void loop() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
//
//   measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);
//
//  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  delay(500);
  if(distance_cm<0.92){
     Serial.println("ther's a crack");
  }
     else{
      ("No cracks found");
     }
    }


