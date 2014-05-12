#include <LiquidCrystal.h>


#define TRIGGER_PIN  9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     8  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


int backLight = 13; 
int i=0;
int stoplight=0;

  unsigned long echo = 0;
 int ultraSoundSignal = 9; // Ultrasound signal pin
 unsigned long ultrasoundValue = 0;
 
void setup()
{
  //pinMode(backLight, OUTPUT);
  //digitalWrite(backLight, HIGH); 

  
  // sensor


  // Serial.begin(9600);
   
 //  pinMode(ultraSoundSignal,OUTPUT);
   pinMode(6, OUTPUT); // Use LED indicator (if required)
 pinMode(5, OUTPUT); // Use LED indicator (if required)
 pinMode(4, OUTPUT); // Use LED indicator (if required)
}   
   

   
    unsigned long ping(){
      unsigned int duration;
      unsigned int distance;
    //* The following trigPin/echoPin cycle is used to determine the
    // distance of the nearest object by bouncing soundwaves off of it. */ 
     digitalWrite(TRIGGER_PIN, LOW); 
     delayMicroseconds(2); 
    
     digitalWrite(TRIGGER_PIN, HIGH);
     delayMicroseconds(10); 
     
     digitalWrite(TRIGGER_PIN, LOW);
     duration = pulseIn(ECHO_PIN, HIGH);
     
     //Calculate the distance (in cm) based on the speed of sound.
     distance = duration/58.2;
     
//     if (distance >= maximumRange || distance <= minimumRange){
//     /* Send a negative number to computer and Turn LED ON 
//     to indicate "out of range" */
//     Serial.println("-1");
//     digitalWrite(LEDPin, HIGH); 
//     }
//     else {
//     /* Send the distance to the computer using Serial protocol, and
//     turn LED OFF to indicate successful reading. */
//     Serial.println(distance);
//     digitalWrite(LEDPin, LOW); 
//     }

  ultrasoundValue = (echo / 58.138) * .39; //convert to CM then to inches
  return distance;
 }

void loop()
{
  //boolean blink = false;
 // lcd.scrollDisplayRight();
//  delay(150);
   // unsigned int uS = ping(); // Send ping, get ping time in microseconds (uS).
 // Serial.print("Ping: ");
 // Serial.print(uS ); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  //Serial.println("cm");

  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  if (stoplight==0)
  {
        digitalWrite(6, HIGH);
           stoplight++;
  }
  else if (stoplight==1)
  {
     digitalWrite(5, HIGH);
        stoplight++;
  }
  else
  {
       digitalWrite(4, HIGH);
       stoplight=0;
  }

  
  delay(4000); //delay 1/4 seconds.


}

