/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int offset =0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
 // pinMode(led, OUTPUT);  
//  pinMode (A5, INPUT); 
   Serial.begin(9600);
  // offset = analogRead(A5);
}

// the loop routine runs over and over again forever:
void loop() {
//  Ser
 // digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);    
  // wait for a second
  Serial.println("ACCEL:  ");

  Serial.println( analogRead(A3));
   Serial.println( analogRead(A4));
    Serial.println( analogRead(A5));

//  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);               // wait for a second
  
}
