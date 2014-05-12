#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
int msg[1];
RF24 radio(9,10);

/*
  - Pin 9 - CE
     - Pin 10 - CS(N)
     - Pin 11 - MOSI
     - Pin 12 - MISO
     - Pin 13 - SCK
     On the Receiver:
     - Pin 3 - LED
     On the Transmitter:
     - Pin 7 - Button

*/
const uint64_t pipe = 0xE8E8F0F0E1LL;
int SW1 = 7;
 
void setup(void){
    pinMode(3, OUTPUT);  
    digitalWrite(3, HIGH);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
   delay(500);
   digitalWrite(3, LOW);
   
}
   
void loop(void){
 // if (digitalRead(SW1) == HIGH){
   while (true);
   {
       if (radio.available())
       {
          bool done = false;
          while (!done)
          {
            // Fetch the data payload
            done = radio.read( msg, sizeof(msg) );
            Serial.print("X = ");
            Serial.println(msg[0]);
            if (msg[0]==111)
            {
               digitalWrite(3, HIGH);
               delay(5000);
            }
          }
       }
       else
       {
         Serial.println("no data available");
       }
       digitalWrite(3, HIGH);
       delay(500);
       digitalWrite(3, LOW);
   }
}

