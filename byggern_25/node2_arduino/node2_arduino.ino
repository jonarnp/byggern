#include <SPI.h>
#include "CAN.h"

/* This program demonstrates simple use of the CAN library
 * by sending a single number that increments each time. 
 * It also displays any number that it receives.  You can
 * program this onto two Arduinos and they will send numbers
 * to each other and print what they receive.  */

/* If you don't have two CAN shields, this program can run on
 * just one by setting the mode to CAN_MODE_LOOPBACK instead
 * of CAN_MODE_NORMAL.  It will internally send the numbers
 * to itself instead of sending them over the bus.  */

unsigned long last;
unsigned long now;
int i;
int val;
CanMessage sendMessage;
CanMessage receiveMessage;
CanMessage message;

void setup()
{
  CAN.begin(CAN_SPEED_125000);
  CAN.setMode (CAN_MODE_NORMAL);
  Serial.begin(9600);
  
  
  //pinMode(53,OUTPUT);
}

void loop()
{
	  //digitalWrite(53, HIGH);   // turn the LED on (HIGH is the voltage level)
	  //delay(1000);               // wait for a second
	  //digitalWrite(53, LOW);    // turn the LED off by making the voltage LOW
	  //delay(1000);
	  
  /* Send a new value every second */
  now = millis();
  if (now > last + 1000) {
    last = now;

    //sendMessage.clear ();
    //sendMessage.setIntData (i);
    //sendMessage.send ();
    i++;
  }

  /* Receive data if available */
  if (CAN.available()) {
	  message = CAN.getMessage ();
	  message.print (HEX);
  }
}
