#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

const byte firstPin = 2; // ensimmäinen nappi
const byte lastPin = 5; // viimeinen nappi (pelinappulat 2–5)
const byte startPin = 6; // start-nappi

/* 
  initButtonsAndButtonInterrupts subroutine is called from Setup() function
  during the initialization of Speden Spelit. This function does the following:
  1) Initializes 4 button pins for the game = Arduino pins 2,3,4,5
  2) Initializes 1 button pin for starting the game = Aruino pin 6
  3) Enables PinChangeInterrupt on D-bus in a way that interrupt
     is generated whenever some of pins 2,3,4,5,6 is connected to LOW state
*/
void initButtonsAndButtonInterrupts(void);

// ISR määritellään .cpp:ssä
ISR(PCINT2_vect);

#endif
