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
  radio.openWritingPipe(pipe);
   delay(500);
   digitalWrite(3, LOW);
   
}
   
void loop(void){
 // if (digitalRead(SW1) == HIGH){
   delay(1000);
      msg[0] = 111;
      radio.write(msg, 1);
       digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);    
      Serial.println("Ping");
      // wait for a second
      digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW
      delay(1000); 

}

