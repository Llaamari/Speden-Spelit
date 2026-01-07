#ifndef LEDS_H
#define LEDS_H
#include <Arduino.h>

/*
  initializeLeds() subroutine intializes analog pins A2,A3,A4,A5
  to be used as outputs. Speden Spelit leds are connected to those
  pins.  
*/
void initializeLeds(void);

/*
  setLed(byte) sets correct led number given as 0,1,2 or 3
  led number 0 corresponds to led connected at Arduino pin A2
  led number 1 => Arduino pin A3
  led number 2 => Arduino pin A4
  led number 3 => Arduino pin A5
  
  parameters:
  byte ledNumber is 0,1,2 or 3
*/
void setLed(byte ledNumber);

/*
  clearAllLeds(void) subroutine clears all leds
*/
void clearAllLeds(void);

/*
  setAllLeds subroutine sets all leds
*/
void setAllLeds(void);

/*
  show1() subroutine shows numbers 0,1,...,15 as binary numbers
  waits a bit and repeats number "show"
*/
void show1(void);

/*
  show2(int) subroutine shows leds 0,1,2,3,0,1,2,3,.... with increasing
  changing rate. And when top change speed has been reached

    Parameters:
  
  int rounds: This parameter determines how many times 0,1,2,3 sequence
              is shown. 
*/
void show2(int);

/*
  Sound effect for losing the game
*/
void soundGameOver();

/*
Fanfare for new high score
*/
void soundHighScore();

/*
Plays a 3 times low sounds and once higher and turns on leds one at a time with sounds, finally all off*/
void soundStartGame();

/*Plays one note matching the number of button received and sets matching led on*/
void soundPlay(byte buttonNumber);

/*Turns tone and leds off*/
void soundStop();

/*Takes in current score and plays a note based on tens of score, ascending*/
void soundAscending(int);
#endif