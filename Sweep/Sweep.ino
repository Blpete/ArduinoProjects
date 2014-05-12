// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo xservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
Servo yservo; 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  xservo.attach(18);  // attaches the servo on pin 9 to the servo object 
  yservo.attach(19);
  
} 
 
 
void loop() 
{ 
  xservo.write(90);
  yservo.write(90);
  delay(1000);
  for(pos = 60; pos < 140; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    yservo.write(pos);              // tell servo to go to position in variable 'pos'
  // xservo.write(pos); 
    delay(45);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 140; pos>=60; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    yservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(25);                       // waits 15ms for the servo to reach the position 
  } 
} 
