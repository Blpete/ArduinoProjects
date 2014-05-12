
//#include <Servo.h>
#include <IRremote.h>



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


// Codes for the motor function.
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4


#define TRIGGER_PIN  14  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     15  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define X_SERVO_PIN 18  // servo pin
#define Y_SERVO_PIN 19  // servo pin
#define SPEAKER_PIN  17
#define IR_RECV_PIN  16  // IR REC Signal - A2

#define C_FORWARD  10
#define C_BACKWARD 15
#define C_STOP  20
#define C_TURN_CC 30

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


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
 
IRrecv irrecv(IR_RECV_PIN);
decode_results results;
long lastCommand = 0;

bool forwardMode = false;
   
//Servo myservo;  // create servo object to control a servo 
int xservoPin;   // for soft servo to avoid use of timer
int xservoAngle = 0;
int yservoPin;   // for soft servo to avoid use of timer
int yservoAngle = 0;      
      
    
      // hitachi vcr remote VT-RM431A codes
    #define RESET_BTN 1560195
    #define PLAY_BTN 1396395
    #define CHANNELUP_BTN 1322685
    #define CHANNELDOWN_BTN 1330875
    #define STOP_BTN 1371825
    #define REW_BTN 1363635
    #define FASTFWD_BTN 1347255
    #define MENU_BTN 1486485
    #define PAUSE_BTN 1527435
    #define POWER_BTN 1318590
    #define INDEX_BTN 1543815
    #define DISPLAY_BTN 1412775
    #define REPEAT_LAST_COMMAND 4294967295
    #define VOLUME_UP 183847095
    #define VOLUME_DOWN 183871575

      
void setup()
{
   Serial.begin(9600);
   Serial.println("Simple Barry Robot sketch");
   
   // ultrasonic setup
   pinMode(TRIGGER_PIN,OUTPUT);
   pinMode(ECHO_PIN, INPUT); // Use LED indicator (if required)
   record = false;

  
   pinMode(SPEAKER_PIN, OUTPUT);
 //  playNote('f', 500);
//   turnCounterClock(100);
//   goStop(10);
    softServoAttach();
   resetScanner();
   
   irrecv.enableIRIn(); // Start the receiver
   Serial.println("Robot initialized");
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
    Serial.println(time);
    motor(3, FORWARD,255);
    motor(4, FORWARD, 255);  
    delay(time);
}

void turnCounterClock(int  time)
{
    recordCommand(C_TURN_CC, time);
    Serial.print("TURN CounterClockwise:");
    Serial.println(time);
    motor(3, FORWARD, 255);
    motor(4, RELEASE,0);
    delay(time);
}

void turnClockwise(int  time)
{
    Serial.print("TURN Clockwise:");
    Serial.println(time);
    motor(4, FORWARD, 255);
    motor(3, RELEASE,0);
    delay(time);
}

void spin(int  time)
{
    Serial.print("SPIN Clockwise:");
    Serial.println(time);
    motor(4, FORWARD, 255);
    motor(3, BACKWARD,255);
    delay(time);
}

void goStop(int time)
{
    recordCommand(C_STOP, time);
    Serial.print("STOP:");
    Serial.println(time);
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

void softServoAttach()
{
   //myservo.attach(10, 650, 2350);

  xservoPin = X_SERVO_PIN;
  yservoPin = Y_SERVO_PIN;
  pinMode(xservoPin, OUTPUT);
  pinMode(yservoPin, OUTPUT);
}

//void softServoDetach()
//{
//  pinMode(servoPin, INPUT);
//}



void resetScanner()
{
    noInterrupts();
   Serial.println("RESET SCANNER");
   softServoWrite(xservoPin, 90, 1000);
   softServoWrite(yservoPin, 90, 1000);
   xservoAngle = 90;
   yservoAngle = 90;
    interrupts();
}

void setScanner(int angle)
{
   noInterrupts();
   softServoWrite(xservoPin, angle, 250);
   xservoAngle = angle;
   interrupts();
   Serial.print("SET SCANNER:");
   Serial.println(angle);
}

void softServoWrite(int servo, int angle, long servoDelay)
{

   int pulsewidth = map(angle, 0, 180, 544, 2400); 
   do {
     digitalWrite(servo, HIGH);
     delayMicroseconds(pulsewidth);
     digitalWrite(servo, LOW);
     delay(20);
     servoDelay -=20;
   } while (servoDelay >=0);
  
}

int scan()
{
  Serial.println("SCANNNING");
 int pos =0;
 int maxdist =0;
 int tdist=0;
 int farangle = 0;

 
 int scanMin = 30;
 int scanMax = 140;
 
  setScanner(scanMin);
  setScanner(scanMin);
  for(pos = scanMin; pos <= scanMax; pos += 5) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 5 degree 
    setScanner(pos);
    tdist = strongPing();
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

 setScanner(farangle);
 xservoAngle = farangle;
   return farangle;
}

void loop()
{  
    while (true)
    {
      if (irrecv.decode(&results) )
      {  
        
        if (results.value == REPEAT_LAST_COMMAND)
        {
          results.value = lastCommand;
        }
        // decode command
         switch (results.value)
         {         
           case RESET_BTN:
                resetScanner();
                break;
           case PLAY_BTN:
                forwardMode =true;
                Serial.println("Going to forward mode");
                break;               
           case CHANNELUP_BTN:
                xservoAngle = xservoAngle -5;
                 setScanner(xservoAngle);
                 break;
           case CHANNELDOWN_BTN:
                 xservoAngle= xservoAngle +5;
                 setScanner(xservoAngle);
                 break;
           case STOP_BTN:
                 Serial.println("STOP");
                 goBackward(100);
                 forwardMode= false;
                 break;              
           case REW_BTN:
                Serial.println("REW");
                turnCounterClock(100);
                goStop(100);
                forwardMode= false;
                break;
            case FASTFWD_BTN:
                Serial.println("FASTFWD");
                turnClockwise(100);
                goStop(100);
                forwardMode= false;
                break;      
            case PAUSE_BTN:   
                Serial.println("PAUSE");
                goStop(100);
                forwardMode= false;
                break;
             case MENU_BTN:
                 Serial.println("MENU");
                 spin(500);
                 goStop(100);
                 forwardMode= false;
                 break;
             case INDEX_BTN:
                 strongPing();
                 break;                
              case POWER_BTN:
                  scan();
                  break;
              case DISPLAY_BTN:
                 Serial.println("UTURN:+100");
                 // u-turn
                 goBackward(300);
                 spin(480);
                 goStop(100);
                 forwardMode= false;
                 break;
           default:
              Serial.print("REMOTE RECEIVED: ");
              Serial.println(results.value);
              forwardMode = false;
              break;
         }
         lastCommand = results.value;
   
         irrecv.resume(); // Receive the next value  
  }
  else
  {
 //   Serial.println("NO REMOTE CODE");
    if (forwardMode)
    {
       performForwardMode();
    }
  }
 } // end while
}

void scanAndTurn()
{
  int angle =90;
     Serial.println("SCAN AND TURN");
   //play_robot_sound();
   goStop(100);
   angle = scan();
   if (angle<90)
   {
     turnCounterClock((90-angle)*5);
   }
   else
   {
     turnClockwise((angle-90)*5);
   }
   goStop(100);
   resetScanner();
}

void performForwardMode()
{
         Serial.println("performForwardMode");
           long dist = strongPing ();
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

          }
          else
          {
            goForward(100);
          }
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


unsigned long strongPing()
{
     long a = ping();
     long b = ping();
     long c = ping();
    
    return (a+b+c)/3; 
  
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
    digitalWrite(SPEAKER_PIN, HIGH);
    delayMicroseconds(tone);
    digitalWrite(SPEAKER_PIN, LOW);
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

