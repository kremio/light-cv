/*
This sketch shows a simple implementation of a serial to GATE/CV interface.

It is intended to operate a MCP4725 12 bits DAC.
Connect the DAC output pin to the CV input of your synth.
Connect the chosen gatePin of the Arduino to the GATE input of your synth.

Once booted, the Arduino will read input from its serial interface.
The protocol is made of 2 types of commands:
/OFF/       Don't play any note (GATE off)
/-?[0-9]+/  Play the note whose pitch is the given amount of semitones higher or lower than C4

For instance, if you send the message:
0

the synth should play the note C4. It will continue playing that note until you send the message:
OFF

If you send:
-12
the synth will play 12 semitones - that is one octave below C4 - which is C3
*/
#include <stdio.h>

#include "EqualTemperament.h"
//Chnage these 2 constants to match your system
const long DAC_RESOLUTION = EqualTemperament::DAC_12;
const int DAC_OUTPUT_VOLTAGE = 5;

//Utility to convert relative pitch values to DAC input values
EqualTemperament ET(DAC_RESOLUTION,  DAC_OUTPUT_VOLTAGE );

#include <Wire.h>//Include the Wire library to talk I2C

//This is the I2C Address of the MCP4725, by default (A0 pulled to GND).
//Please note that this breakout is for the MCP4725A0.
#define MCP4725_ADDR 0x62 //NOTE: For devices with A0 pulled HIGH, use 0x61

const int dacGND = A2; //the pin connected to the DAC GND
const int dacVCC = A3; //the pin connected to the DAC VCC
const int gatePin = 13; //the pin connected to the GATE input of your synth

String cmd = "OFF";
String prevCmd = "OFF";
long cv = 0;

void setup() {
  Wire.begin();

  //Power up the DAC
  pinMode( dacGND, OUTPUT);
  pinMode( dacVCC, OUTPUT);
  digitalWrite( dacGND, LOW);//Set A2 as GND
  digitalWrite( dacVCC, HIGH);//Set A3 as Vcc

  //To send the GATE signal
  pinMode(gatePin, OUTPUT);
  digitalWrite(gatePin, LOW);

  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}


void loop() {
  //Read serial input
  if (Serial.available() > 0) {
    cmd = Serial.readString();
  }

  if ( cmd.compareTo("OFF") == 0 ) {
    digitalWrite(gatePin, LOW); //GATE off
    //Serial.println("OFF");
  } else if( cmd.compareTo(prevCmd) != 0  ){ //Then its a note command
    cv = ET.relativePitchToDAC( cmd.toInt() );  

    //Set CV using the DAC
    digitalWrite(gatePin, LOW);
    Wire.beginTransmission(MCP4725_ADDR);
    Wire.write(64);                     // cmd to update the DAC
    Wire.write(cv >> 4);        // the 8 most significant bits...
    Wire.write((cv & 15) << 4); // the 4 least significant bits...
    Wire.endTransmission();
    delay(10);
    digitalWrite(gatePin, HIGH); //GATE on
  }
  prevCmd = cmd;
}

