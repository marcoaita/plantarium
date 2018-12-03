// Wemos D1 mini esp8266 two pots on A0 plus two switches to drive a PCA9685 pwm controller

// WARNING: In order to upload this sketch successfully, D4 (GPIO2) needs to be HIGH. If GPIO2 is kept LOW during upload, the upload will fail!!
// ALSO: After uploading and removing USB, the board will hang upon a power cycle if GPIO2 or GPIO0 are LOW on boot. 
// To reset the board GPIO2 and GPIO0 need to be HIGH.
// SUGGESTION: DO NOT USE SWITCHES ON GPIO2 and GPIO0 (D3 on Wemos D1 Mini)

// see https://bbs.espressif.com/download/file.php?id=893&mode=view for more info on pins, GPIOs and pullup/puudowns 

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//default values
int sensor=0;
int pot0val=0;
int pot1val=0;

// MAPPINGS

//Pin names - Inputs/Outputs
const int sw0in = D5;       // GPIO14 // Switches. For ease of implementation they are used with pwm
const int sw1in = D6;       // GPIO12
const int Pot0in = D7;      // GPIO13 // The Pot pins are actually outputs to control which potentiometer is active and use multiple pot on the single ADC on the ESP8266
const int Pot1in = D8;      // GPIO15 

// PWM outputs
const int Pot0out = 2; // pwm output channel PCA9685
const int Pot1out = 5;
const int sw0out = 8;
const int sw1out = 11;

// Initialize pwm driver
// called this way, the PCA9685 uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
pinMode(Pot0in, OUTPUT);  // enabling/power source for pot 0
pinMode(Pot1in, OUTPUT);  // pot1
pinMode(sw0in,INPUT_PULLUP); //switch 1 - pump
pinMode(sw1in,INPUT_PULLUP); // switch 2 - pump
pinMode(A0, INPUT);  // do we really need this??

  Serial.begin(9600);
  delay(1000);
  Serial.println("Jana's PWM test! I am alive!!!!");

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  Wire.setClock(400000);
  }

// the loop function runs over and over again forever
void loop() {
  
  //read pot 0
  digitalWrite(Pot0in, HIGH); //enable pot0
  digitalWrite(Pot1in, LOW);  // disable pot1
//  sensor = analogRead(A0);  //read analog 
//  delay(10);  // allow ADC to settle
//  sensor+= analogRead(A0);  //read analog
//    delay(10);  // allow ADC to settle
//  sensor+= analogRead(A0);  //read analog
//    delay(10);  // allow ADC to settle
//  sensor+= analogRead(A0);  //read analog
//    delay(10);  // allow ADC to settle
//  sensor+= analogRead(A0);  //read analog

  sensor=ReadESP8266Analog(A0);  //read analog in 5 times and sum (convert from 10 bit to 12 bits for the pwm controller (and average)
  Serial.print("Pot 0 sensor value: ");
  Serial.println(sensor);
  pot0val=sensor -10;  //shifting down to be sure we get to zero
  pot0val=max(min(pot0val,4095),0);
  Serial.print("Pot 0 value: ");
  Serial.println(pot0val);

  //read pot 1
  digitalWrite(Pot0in, LOW); //disable pot0
  digitalWrite(Pot1in, HIGH);  // enable pot1
  sensor = ReadESP8266Analog(A0);  //read ADC 
  Serial.print("Pot 1 sensor value: ");
  Serial.println(sensor);
  pot1val=sensor -10;  //shifting down to be sure we get to zero
  pot1val=max(min(pot1val,4095),0);
  Serial.print("Pot 1 value: ");
  Serial.println(pot1val);
  
  delay(50);
  pwm.setPWM(Pot0out, 0,pot0val);     //light
  pwm.setPWM(Pot1out, 0,pot1val);   //pump 1

// Read switches 

  if (digitalRead(sw0in)==HIGH) {
    pwm.setPWM(sw0out,4096,0);  //pump off?
    Serial.print("Pump 0 off, pump0 (");
    Serial.print(sw0in);
    Serial.print(") is HIGH");  
  }
  else {
    pwm.setPWM(sw0out,0,4096);
    Serial.print("Pump 0 on, pump0 (");
    Serial.print(sw0in);
    Serial.print(") is LOW");   

  }
  if (digitalRead(sw1in)==HIGH) {
    pwm.setPWM(sw1out,4096,0);  //pump off?
    Serial.print("Pump 1 off, pump1 (");
    Serial.print(sw1in);
    Serial.print(") is HIGH");
  }
    else {
    pwm.setPWM(sw1out,0,4096);
    Serial.print("Pump 1 on, pump1 (");
    Serial.print(sw1in);
    Serial.print(") is LOW");

  }
  
#ifdef ESP8266
    yield();  // take a breather, required for ESP8266
#endif
}

int ReadESP8266Analog(int port) {   //read analog in 5 times and sum (convert from 10 bit to 12 bits for the pwm controller (and average)
  int sens = 0;
  sens = analogRead(port);  //read ADC - 10 bits on ESP8266
  delay(10);  // allow ADC to settle
  sens+= analogRead(port);  
  delay(10);  
  sens+= analogRead(port);  
  delay(10);  
  sens+= analogRead(port);  
  delay(10);  
  sens+= analogRead(port);  
  delay(10);
return sens;
}
