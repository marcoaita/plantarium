// esp8266 two pots on A0


int sensor=0;
int led0=0;
int led1=0;

void setup() {
pinMode(D0, OUTPUT);  //pot 0
pinMode(D1, OUTPUT);  //pot1
pinMode(D3, OUTPUT);  // led 0
pinMode(D4, OUTPUT);  // led 1
pinMode(A0, INPUT);  // do we really need this??
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(D0, HIGH); //enable pot0
  digitalWrite(D1, LOW);  // disable pot1
  sensor = analogRead(A0);  //read analog 
  // map it to the range of the analog out:
  led0 = map(sensor, 0, 1023, 0, 255);  

  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(10); 
  
  digitalWrite(D0, LOW); //disable pot0
  digitalWrite(D1, HIGH);  // enable pot1
  sensor = analogRead(A0);  //read analog 
  // map it to the range of the analog out:
  led1 = map(sensor, 0, 1023, 0, 255);  
  
  // change the analog out value:
  analogWrite(D3, led0);  //light the first led
  analogWrite(D4, led1);  //light the second led 
  
  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(10);                    
}

