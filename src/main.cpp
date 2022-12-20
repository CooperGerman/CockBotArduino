/*
Header here
*/

#include "Arduino.h"
#include "gen_pwm.h"
#include "scale.h"
#include "Wire.h"

// MAPPING:
const unsigned scale_sck     = 2;
const unsigned scale_dout    = 3;
const unsigned pinServozero  = 5;
const unsigned pinServofull  = 6;

// PWM constructor:
gen_pwm_C pwm(pinServofull, 180, pinServozero, 90);

//scale constructor:
scale_C scale(scale_dout, scale_sck);

//i2cSlave constructor:
#define SLAVE_ADDRESS 0x08
byte weight = 0;
byte cmd = 0;

void sendData();
void receiveData(int bytecount);

void receiveData(int bytecount)
{
  for (int i = 0; i < bytecount; i++)
  {
    cmd = Wire.read();
  }
  switch (cmd)
  {
  case 1:
    weight = scale.update();
    break;

  case 133:
    scale.tare();
    break;
  case 4:
    // print command received
    Serial.print("Command received: ");
    Wire.write(weight);
    break;

  default:
    break;
  }
}
void sendData()
{
  Wire.write(weight);
}

void setup()
{
  pwm.begin();
  scale.begin();
  Serial.begin(9600);
  delay(100);
  Serial.println();
  Serial.println("Starting...");
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop()
{

  weight = scale.update() ;
  Serial.print("weight : ");
  Serial.print(weight);
  Serial.print("                      \r");

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0)
  {
    char inByte = Serial.read();
    if (inByte == 't')
      scale.tare();
  }
}