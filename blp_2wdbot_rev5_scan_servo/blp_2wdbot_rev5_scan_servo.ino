
// Simple Adafruit Motor Shield sketch
// -----------------------------------
//
// By arduino.cc user "Krodal".
// June 2012
// Open Source / Public Domain
//
// Using Arduino 1.0.1
//
// A simple sketch for the Adafruit motor shield,
// without using the Adafruit library.
//
// The outputs can be used for DC-motors 
// (either full H-bridge or just On and Off), lights, 
// relays, solenoids, etc.
// But stepper motors can not be used !
// Servo motors can be used with the default Servo library.
//
// A maximum of 4 DC motors can be used with full-bridge,
// or a maximum of 8 normal outputs, or a combination.
// Two servo motors can always be used, they use the +5V 
// of the Arduino board, so the voltage regulator could 
// get hot.
//
// Tested with an Ebay clone with the Arduino Uno.
//
// Parts of the code are from the Adafruit Motor Shield
// library.
// The Adafruit program is public domain.
// This code is also public domain
//
// This simplified program is using the normal 
// Arduino library functions as much as possible.
//
// The motors will make a whistling sound, 
// due to the analogWrite() PWM frequency.
// The Adafruit library is specifically designed to avoid
// this, so use the Adafruit library for a better result.
//
//
//
// Connector usage
// ---------------
// The order is different than what you would expect.
// If the Arduino (Uno) board is held with the USB
// connector to the left, the positive (A) side is 
// at the top (north), and the negative (B) side is 
// the bottom (south) for both headers.
//
//   Connector X1:
//     M1 on outside = MOTOR1_A   (+) north
//     M1 on inside  = MOTOR1_B   (-)
//     middle        = GND
//     M2 on inside  = MOTOR2_A   (+)
//     M2 on outside = MOTOR2_B   (-) south
//
//   Connector X2:
//     M3 on outside = MOTOR3_B   (-) south
//     M3 on inside  = MOTOR3_A   (+)
//     middle        = GND
//     M4 on inside  = MOTOR4_B   (-)
//     M4 on outside = MOTOR4_A   (+) north
//
//
//         -------------------------------
//         | -+s                         |
//         | -+s                         |
//    M1 A |                             | M4 A
//    M1 B |                             | M4 B
//    GND  |                             | GND
//    M2 A |                             | M3 A
//    M2 B |                             | M3 B
//         |                       ..... |
//         -------------------------------
//                + - 
//
//
//
// Pin usage with the Adafruit Motorshield
// ---------------------------------------
// Analog pins: not used at all
//     A0 ... A5 are still available
//     They all can also be used as digital pins.
//     Also I2C (A4=SDA and A5=SCL) can be used.
//     These pins have a breadboard area on the shield.
// Digital pins: used: 3,4,5,6,7,8,9,10,11,12
//     Pin 9 and 10 are only used for the servo motors.
//     Already in use: 0 (RX) and 1 (TX).
//     Unused: 2,13
//     Pin 2 has an soldering hole on the board, 
//           easy to connect a wire.
//     Pin 13 is also connected to the system led.
// I2C is possible, but SPI is not possible since 
// those pins are used.
//

/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */






#include <Servo.h>
#include <IRremote.h>

int RECV_PIN = 16;  // IR REC Signal - A2
// Arduino pins for the shift register
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

// 8-bit bus after the 74HC595 shift register 
// (not Arduino pins)
// These are used to set the direction of the bridge driver.
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

// Arduino pins for the PWM signals.
#define MOTOR1_PWM 11
#define MOTOR2_PWM 3
#define MOTOR3_PWM 6
#define MOTOR4_PWM 5
#define SERVO1_PWM 10
#define SERVO2_PWM 9

// Codes for the motor function.
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4


#define TRIGGER_PIN  14  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     15  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.



#define C_FORWARD  10
#define C_BACKWARD 15
#define C_STOP  20
#define C_TURN_CC 30


int length = 7; // the number of notes
char notes[] = "ccggaagffeeddc"; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 200;



   int count=0;
 int command[140];
 int commandTimer[140];
 int commandCounter=0;
 boolean record = false;
 int playbackCounter=0;
 
IRrecv irrecv(RECV_PIN);
decode_results results;
int speakerPin = 18;
bool forwardMode = false;
   
Servo myservo;  // create servo object to control a servo 
      
void setup()
{
   Serial.begin(9600);
   Serial.println("Simple Barry Robot sketch");
   
   // ultrasonic setup
   pinMode(TRIGGER_PIN,OUTPUT);
   pinMode(ECHO_PIN, INPUT); // Use LED indicator (if required)
   record = false;

   irrecv.enableIRIn(); // Start the receiver
   pinMode(speakerPin, OUTPUT);
   playNote('f', 500);
   turnCounterClock(100);
   goStop(10);
}


void goForward(int time)
{
    Serial.print("FORWARD:");
    Serial.print(time);
    motor(3, BACKWARD,255);
    motor(4, BACKWARD, 255);  
    delay(time);
    recordCommand(C_FORWARD, time);
}

void goBackward(int time)
{
    recordCommand(C_BACKWARD, time);
    Serial.print("BACKWARD:");
    Serial.print(time);
    motor(3, FORWARD,255);
    motor(4, FORWARD, 255);  
    delay(time);
}

void turnCounterClock(int  time)
{
    recordCommand(C_TURN_CC, time);
    Serial.print("TURN CounterClockwise:");
    Serial.print(time);
    motor(3, FORWARD, 255);
    motor(4, RELEASE,0);
    delay(time);
}

void turnClockwise(int  time)
{
    Serial.print("TURN Clockwise:");
    Serial.print(time);
    motor(4, FORWARD, 255);
    motor(3, RELEASE,0);
    delay(time);
}

void spin(int  time)
{
    Serial.print("SPIN Clockwise:");
    Serial.print(time);
    motor(4, FORWARD, 255);
    motor(3, BACKWARD,255);
    delay(time);
}

void goStop(int time)
{
    recordCommand(C_STOP, time);
    Serial.print("STOP:");
    Serial.print(time);
    motor(3,RELEASE,0);
    motor(4,RELEASE,0);
    delay(time);
}

void recordCommand(int c, int time)
{
  if (record)
  {
     command[commandCounter] = c;
     commandTimer[commandCounter] = time;
    commandCounter++;
  } 
}


void playback()
{
  record = false;
  // reverse  
  playbackCounter = commandCounter--;
  for (int i=playbackCounter; i>=0; i--)
  {
    Serial.println("PLAYBACK ");
    Serial.println(i);
      int time = commandTimer[i];
      
      if (command[i] == C_FORWARD)
      {
        goBackward(time);
      }
      if (command[i] == C_TURN_CC)
      {
       turnClockwise(time);
      }
      if (command[i] == C_BACKWARD)
      {
        goForward(time);
      }
      if (command[i] == C_STOP)
      {
        goStop(time);
      }
  }
}


void scan()
{
  Serial.println("SCANNNING");
 int pos =0;
 int maxdist =0;
 int tdist=0;
 int farangle = 0;
 myservo.attach(10, 650, 2350);
 myservo.write(0);
 delay(1000);
// myservo.attach(10);  // attaches the servo on pin 9 to the servo object    
  for(pos = 0; pos <= 180; pos += 5) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(250);                       // waits 15ms for the servo to reach the position 
    tdist = ping();
    if (maxdist<tdist)
    {
      maxdist = tdist;
      farangle = pos;
    }
    Serial.print("Angle: ");
    Serial.print(pos);
    Serial.print("  DIST=");
    Serial.println(tdist);

  } 
      
 Serial.print("MAX=");
Serial.println(maxdist);
Serial.print("POS=");
Serial.println(farangle);

  myservo.write(farangle);              // tell servo to go to position in variable 'pos' 
  delay(115);                       // waits 15ms for the servo to reach the position 
 
  myservo.detach();
  delay(1000);
}

void loop()
{  
//   if (forwardMode)
//   {
//        Serial.println("FWD-AUTO");
//
//         irrecv.resume(); // Receive the next value
//    }
    
    if (irrecv.decode(&results) )
      {
    
         Serial.println("REMOTERECEIVED:");
         Serial.println(results.value);
        
        // hitachi vcr remote
        if (results.value == 4294967295)
        {
          Serial.println("FWD");
          forwardMode =true;
          Serial.println("Going to forward mode");
//          
//
//          long dist = ping ();
//          if (dist<90)
//          {
//            if (dist<30)
//            {
//              forwardMode = false;
//              goStop(50);
//              playNote('f', 50);
//            }
//            else
//            {
//                goForward(20);
//              playTone(dist*200, 50);
//            }
//          }
//          else
//          {
//            goForward(100);
//           // goStop(10);
//      
//          }
         // goForward(100);
        }
        else if (results.value == 3416028668)
        {
          Serial.println("STOP");
          goBackward(100);
          forwardMode= false;
        }
        else if  (results.value == 1363635)
        {
          Serial.println("REW");
          turnCounterClock(100);
             goStop(100);
              forwardMode= false;
        }
        else if (results.value == 1347255)
        {
          Serial.println("FASTFWD");
          turnClockwise(100);
             goStop(100);
              forwardMode= false;
        }
        else if (results.value == 2540796281) // pause button
        {
          Serial.println("PAUSE");
          goStop(100);
           forwardMode= false;
        }
        else if (results.value == 3570438137) // menu
        {
           Serial.println("MENU");
           spin(500);
           goStop(100);
           forwardMode= false;
        }  
       else if (results.value ==  1318590)

       {
          Serial.println("POWER");
          //play_robot_sound();
          goStop(100);
           scan();
     //     play_robot_sound();
      
          forwardMode= false;
       }
       else if (results.value ==  2681643546)
       {
         Serial.println("UTURN:+100");
         // u-turn
         goBackward(300);
         spin(480);
         goStop(100);
         forwardMode= false;
       }
      else
      {
           goStop(50);
            forwardMode= false;
      }
      irrecv.resume(); // Receive the next value
    
  }
  else
  {
    if (forwardMode)
    {
        //  delay(20);
          long dist = ping ();
          if (dist<50)
          {
            // warn
             playTone(dist*200, 50);
             goForward(50);
             goStop(10);
             if (dist<30)
             {
               forwardMode = false;
               goStop(100);
               playNote('f', 50);
             }
//            if (dist<30)
//            {
//              goStop(50);
//              playNote('f', 50);
//            }
//            else
//            {
//                playTone(dist*200, 50);
//            }
          }
          else
          {
            goForward(100);
          }
    }
  }
  // autonomous mode
  
//  count++ ;
//  if(count==4)
//  {
//      record = false;
//    //  spin(5000); 
//      goStop(1000);
//      playback();
//  }
//  
//  if (count==5)
//  {
//    spin(5000);
//  }
//  
//  if (count>3)
//  {
//  
//     goStop(10000);
//  }
//  else
//  {
//    
//    long dist = ping ();
//
//     while (dist>20)
//     {
//       goForward(100);
//       dist = ping();
//     }
//  
//    if (dist==0)
//    {
//      count=20;
//      Serial.println("no sensor");
//    }
//    
//    goStop(500);
//  
//    goBackward(1000);
//
//    goStop(500);
//
//    turnCounterClock(300);
//
//    goStop(500);
//  }
}


// Initializing
// ------------
// There is no initialization function.
//
// The shiftWrite() has an automatic initializing.
// The PWM outputs are floating during startup, 
// that's okay for the Adafruit Motor Shield, it stays off.
// Using analogWrite() without pinMode() is valid.
//


// ---------------------------------
// motor
//
// Select the motor (1-4), the command, 
// and the speed (0-255).
// The commands are: FORWARD, BACKWARD, BRAKE, RELEASE.
//
void motor(int nMotor, int command, int speed)
{
   int motorA, motorB;

   if (nMotor >= 1 && nMotor <= 4)
   {  
     switch (nMotor)
     {
     case 1:
       motorA   = MOTOR1_A;
       motorB   = MOTOR1_B;
       break;
     case 2:
       motorA   = MOTOR2_A;
       motorB   = MOTOR2_B;
       break;
     case 3:
       motorA   = MOTOR3_A;
       motorB   = MOTOR3_B;
       break;
     case 4:
       motorA   = MOTOR4_A;
       motorB   = MOTOR4_B;
       break;
     default:
       break;
     }

     switch (command)
     {
     case FORWARD:
       motor_output (motorA, HIGH, speed);
       motor_output (motorB, LOW, -1);     // -1: no PWM set
       break;
     case BACKWARD:
       motor_output (motorA, LOW, speed);
       motor_output (motorB, HIGH, -1);    // -1: no PWM set
       break;
     case BRAKE:
       // The AdaFruit library didn't implement a brake.
       // The L293D motor driver ic doesn't have a good
       // brake anyway.
       // It uses transistors inside, and not mosfets.
       // Some use a software break, by using a short
       // reverse voltage.
       // This brake will try to brake, by enabling 
       // the output and by pulling both outputs to ground.
       // But it isn't a good break.
       motor_output (motorA, LOW, 255); // 255: fully on.
       motor_output (motorB, LOW, -1);  // -1: no PWM set
       break;
     case RELEASE:
       motor_output (motorA, LOW, 0);  // 0: output floating.
       motor_output (motorB, LOW, -1); // -1: no PWM set
       break;
     default:
       break;
     }
   }
}


// ---------------------------------
// motor_output
//
// The function motor_ouput uses the motor driver to
// drive normal outputs like lights, relays, solenoids, 
// DC motors (but not in reverse).
//
// It is also used as an internal helper function 
// for the motor() function.
//
// The high_low variable should be set 'HIGH' 
// to drive lights, etc.
// It can be set 'LOW', to switch it off, 
// but also a 'speed' of 0 will switch it off.
//
// The 'speed' sets the PWM for 0...255, and is for 
// both pins of the motor output.
//   For example, if motor 3 side 'A' is used to for a
//   dimmed light at 50% (speed is 128), also the 
//   motor 3 side 'B' output will be dimmed for 50%.
// Set to 0 for completelty off (high impedance).
// Set to 255 for fully on.
// Special settings for the PWM speed:
//    Set to -1 for not setting the PWM at all.
//
void motor_output (int output, int high_low, int speed)
{
   int motorPWM;

   switch (output)
   {
   case MOTOR1_A:
   case MOTOR1_B:
     motorPWM = MOTOR1_PWM;
     break;
   case MOTOR2_A:
   case MOTOR2_B:
     motorPWM = MOTOR2_PWM;
     break;
   case MOTOR3_A:
   case MOTOR3_B:
     motorPWM = MOTOR3_PWM;
     break;
   case MOTOR4_A:
   case MOTOR4_B:
     motorPWM = MOTOR4_PWM;
     break;
   default:
     // Use speed as error flag, -3333 = invalid output.
     speed = -3333;
     break;
   }

   if (speed != -3333)
   {
     // Set the direction with the shift register 
     // on the MotorShield, even if the speed = -1.
     // In that case the direction will be set, but
     // not the PWM.
     shiftWrite(output, high_low);

     // set PWM only if it is valid
     if (speed >= 0 && speed <= 255)    
     {
       analogWrite(motorPWM, speed);
     }
   }
}


// ---------------------------------
// shiftWrite
//
// The parameters are just like digitalWrite().
//
// The output is the pin 0...7 (the pin behind 
// the shift register).
// The second parameter is HIGH or LOW.
//
// There is no initialization function.
// Initialization is automatically done at the first
// time it is used.
//
void shiftWrite(int output, int high_low)
{
   static int latch_copy;
   static int shift_register_initialized = false;

   // Do the initialization on the fly, 
   // at the first time it is used.
   if (!shift_register_initialized)
   {
     // Set pins for shift register to output
     pinMode(MOTORLATCH, OUTPUT);
     pinMode(MOTORENABLE, OUTPUT);
     pinMode(MOTORDATA, OUTPUT);
     pinMode(MOTORCLK, OUTPUT);

     // Set pins for shift register to default value (low);
     digitalWrite(MOTORDATA, LOW);
     digitalWrite(MOTORLATCH, LOW);
     digitalWrite(MOTORCLK, LOW);
     // Enable the shift register, set Enable pin Low.
     digitalWrite(MOTORENABLE, LOW);

     // start with all outputs (of the shift register) low
     latch_copy = 0;

     shift_register_initialized = true;
   }

   // The defines HIGH and LOW are 1 and 0.
   // So this is valid.
   bitWrite(latch_copy, output, high_low);

   // Use the default Arduino 'shiftOut()' function to
   // shift the bits with the MOTORCLK as clock pulse.
   // The 74HC595 shiftregister wants the MSB first.
   // After that, generate a latch pulse with MOTORLATCH.
   shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
   delayMicroseconds(5);    // For safety, not really needed.
   digitalWrite(MOTORLATCH, HIGH);
   delayMicroseconds(5);    // For safety, not really needed.
   digitalWrite(MOTORLATCH, LOW);
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
       Serial.print("PING:");
       Serial.println(distance);

  //ultrasoundValue = (echo / 58.138) * .39; //convert to CM then to inches
  return distance;
 }






void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void play_robot_sound() {
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }

    // pause between notes
    delay(tempo / 2); 
  }
}

