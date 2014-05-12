// UTFT_Textrotation_Demo (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of the textrotation-functions.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
//
//// Uncomment the next line for Arduino 2009/Uno
//UTFT myGLCD(ITDB32S,19,18,17,16);   // Remember to change the model parameter to suit your display module!

// Uncomment the next line for Arduino Mega
//UTFT myGLCD(ITDB32S,38,39,40,41);   // Remember to change the model parameter to suit your display module!
#define cs   10
#define dc   9
#define rst  8  
UTFT myGLCD(TFT01_22SP,51,52,cs,rst,dc);   



char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {25, 26, 27, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {22, 23, 24}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
  Serial.begin(9600);
}

void loop()
{
  int count =0;
  while (true)
  {
     // String keystring = "";
   String content = "";
   char character;
    int val = analogRead(A0);
    
  String keystring = "Analog0:";
  keystring +=analogRead(A0);
  keystring += "    ";
  
    myGLCD.setColor(0,0,0);
     myGLCD.drawLine(count, 0, count, 240);
     myGLCD.setColor(255,255,255);
     int min = val/4;
     if (min>230) {
       min = 230;
     }
     myGLCD.drawLine(count, min, count, min+1);
 //  myGLCD.print("Enter DATA:", 0, 0);
 //  char key = keypad.getKey();
  // keystring.concat(key);
   // myGLCD.print(keystring, 100,100, 0);
  //  delay(5);
    
    
    count++;
    if (count>300) {
      count=0;
    }
//   while(Serial.available()) {
//      character = Serial.read();
//      if (character == '\n')
//      {
//        break;
//      }
//      else
//      {
//        content.concat(character);
//      }
//      
//  }
//
//    myGLCD.print("DATA:", 0, 0);
//    myGLCD.setColor(0, 0, 255);
//    myGLCD.print(content, 0, 16, 0);
//    myGLCD.print(content, 319, 0, 90);
//    myGLCD.print(content, 319, 239, 180);
//    myGLCD.print(content, 0, 239, 270);
  }
}

