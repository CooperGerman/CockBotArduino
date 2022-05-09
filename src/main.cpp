/*
Header here
*/

#include "Arduino.h"
#include "gen_pwm.h"
#include "scale.h"

// MAPPING:
const unsigned scale_sck     = 2;
const unsigned scale_dout    = 3;
const unsigned pinServozero  = 5;
const unsigned pinServofull  = 6;

// PWM constructor:
gen_pwm_C pwm(pinServofull, 180, pinServozero, 90);

//scale constructor:
scale_C scale(scale_dout, scale_sck);

boolean setupDone = false ;

void setup()
{
  pwm.begin();
  scale.begin();
  // scale.tare();
  Serial.begin(9600);
  delay(100);
  Serial.println();
  Serial.println("Starting...");
  setupDone = true;
}

void loop()
{

  unsigned i = 0;

  // i = scale.update() ;
  Serial.print("weight : ");
  Serial.print(i);
  Serial.print("                      g\r");

  // // receive command from serial terminal, send 't' to initiate tare operation:
  // if (Serial.available() > 0)
  // {
  //   char inByte = Serial.read();
  //   if (inByte == 't')
  //     scale.tare();
  // }

  // // check if last tare operation is complete:
  // if (scale.isTare == true)
  // {
  //   Serial.println("Tare complete");
  // }

}