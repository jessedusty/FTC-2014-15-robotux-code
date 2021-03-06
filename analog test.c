#pragma config(Sensor, S1,     HTSPB,                sensorI2CCustom9V)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*   HiTechnic Experimenter's Kit for the SuperPro

Experiment - 1 Introduction
Read the status of a switch and display the status on an LED

*/

#include "drivers/hitechnic-superpro.h"

task main() {
  int pin1, pin2, pin3, pin4;

  // Set B0 for output
  HTSPBsetupIO(HTSPB, 0x1);

  while(true) {
    // Read a 10bit wide analogue value from A0
    pin1 = HTSPBreadADC(HTSPB, 0, 10);
    pin2 = HTSPBreadADC(HTSPB, 1, 10);
    pin3 = HTSPBreadADC(HTSPB, 2, 10);
    pin4 = HTSPBreadADC(HTSPB, 3, 10);

    nxtDisplayTextLine(1, "A0: %d", pin1);
    nxtDisplayTextLine(2, "A1: %d", pin2);
    nxtDisplayTextLine(3, "A2: %d", pin3);
    nxtDisplayTextLine(4, "A3: %d", pin4);


    // If A0 is less than 50% of the max value
    // turn off the LED, otherwise switch it on

    wait1Msec(50);
  }
}
