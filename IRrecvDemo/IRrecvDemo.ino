/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 16;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
    
    // hitachi vcr remote
    if (results.value == 0x2EB5C035)
    {
      Serial.println("PLAY");
    }
     if (results.value == 312417881)
    {
      Serial.println("STOP");
    }
    if (results.value == 402094229)
    {
      Serial.println("REW");
    }
    if (results.value == 3933546233)
    {
      Serial.println("FWD");
    }
    

  }
  
  delay(1000);
}
