#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>



/*
  initializeDisplay subroutine initializes 5 pins needed for controlling 7-segment
  displays in a cascade as follows:
  Arduino pin 9 = serial to parallel component reset
  Arduino pin 10 = serial to parallel component shiftClock
  Arduino pin 11 = serial to parallel component latchClock
  Arduino pin 12  = serial to parallel component outEnable
  Arduino pin 13  = serial to parallel component serialInput
  And also initializes the LCD-display
  

*/
void initializeDisplay(void);

/*
  WriteByte subroutine writes number 0,1,...,9 to
  7-segment display. If there are many 7-segment
  displays cascaded, last parameter can be used to
  control when number is actually shown at the outputs of
  display element.
  
  Parameters:
  uint8_t number: accepts values 0,1,..,9
  
  bool last: either true or false. True = number is displayed
  at the output, false is used if this function is called
  multiple times to write all cascaded numbers to 7-segment
  displays.
*/
void writeByte(uint8_t number, bool last);


/*
  writeHighAndLowNumber subroutine writes a number 0,1,..,99
  to 2 cascaded 7-segment displays. This subroutine uses
  WriteByte subroutine to write 2 numbers to the display.
  
  Parameters:
  
  uint8_t tens: number 0,1,..,9
  uint8_t ones: number 0,1,..,9
  
*/
void writeHighAndLowNumber(uint8_t tens,uint8_t ones);
/*
  showonsegsegment displays any number between 0 and 99 on two adjacent 7-segment displays.
*/
void showOnSevenSegment(uint16_t numero);


/*
showOnLcd displays any number on lcd screen
*/
void showOnLcd(uint16_t numero);
/*
textOnLcd lets you input any text on the lcd screen.uint8_t
if it is too long the text will start to roll on the screen
*/

void textOnLcd(String text); //beta

/*
 showNumber shows any number between 0-99 on both displays
*/
void showNumber(uint16_t numero);

/*
 Prints top scores to lcd.
*/
void showScoreBoard(uint16_t scores[3]);
/*Puts lcd to powersave mode*/

void lcdPowerSave(bool enable);
/* turn of 7segments*/
void sevenSegmentPowerSave(bool enable);

/*puts arduino and lcd tp powersave mode*/
void enterSleepMode();
// Tyhjä ISR, mutta pakollinen herätykseen
void wakeUp();



#endif
