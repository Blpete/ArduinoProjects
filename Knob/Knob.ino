// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
Servo rotServo;
int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin 
int val1;
int direction = 5;
int pos =90;
void setup() 
{ 
  rotServo.attach(3);
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object 
} 
 
void loop() 
{ 
     pos=pos+direction;
     if (pos>160)
     {
       direction = -5;
     }
     if (pos<10)
     {
       direction =5;
     }
     // in steps of 1 degree 
    rotServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position  
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  val1 = analogRead(1);
  val1 = map(val1, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  rotServo.write(val1);  
  delay(15);    // waits for the servo to get there 
} 
