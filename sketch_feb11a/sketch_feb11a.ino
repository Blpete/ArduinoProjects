#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
      // while the serial stream is not open, do nothing:
    while (!Serial) ;
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  Serial.print(".");
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}






