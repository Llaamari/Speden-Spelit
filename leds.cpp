#include "leds.h"

int notes[] = {262,294,330,349};          //C4, D4, E4, F4      Notes for buttons during play
int gameOver[] = {294, 277, 262, 247};    //D4, CS4, E4, B3     Notes for gameover sound
int highScore[] = {523, 659, 784, 1047};  //C5, E5, G5, C6      Notes for high score fanfare
int startSound[] = {392, 784};              //G4, G5            Notes for start game sounds
int ascendingNotes[] = {440, 494, 523, 587, 659, 698, 784, 880, 988, 1047, 1175, 1319, 1397, 1568, 1760};  //A4, B4, C5, D5 etc
//Ascending notes to signal increasing speed
int noteDuration[] = {4,4,4,1};           //Note durations for gameover and high score sounds effects

//If needed to change led pins, do it here. Default A2, A3, A4, A5
#define pin0 A2
#define pin1 A3
#define pin2 A4
#define pin3 A5

void initializeLeds()
{
// see requirements for this function from leds.h
//Initialized as outputs and set to low, A1 for sound
  pinMode(A1, OUTPUT);
  pinMode(pin0, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin0, LOW);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
}

void setLed(byte ledNumber)
{
// see requirements for this function from leds.h
//Clear all leds and sound, then set exactly one led based on input
  clearAllLeds();
  noTone(A1);
  soundPlay(ledNumber); //Clears previous state of leds and based on number received as input set that led and sound on
  delay(250);
  noTone(A1);
}


void clearAllLeds()
{
// see requirements for this function from leds.h
//Set all leds to off
  digitalWrite(pin0, LOW);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
}

void setAllLeds()
{
// see requirements for this function from leds.h
//Set all leds on
  digitalWrite(pin0, HIGH);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
}


void show1()
{
// see requirements for this function from leds.h
//Set all leds to off, set counter byte to 0.
//bitRead checks individual bits and sets leds to on when matching bit is 1 and off when 0
  clearAllLeds();
  byte laskuri = 0;
  while(laskuri < 16)
  {
    if(bitRead(laskuri,0) == 1)
    {
      digitalWrite(pin0, HIGH);
    }
    else
    {
      digitalWrite(pin0, LOW);
    }
    if(bitRead(laskuri,1) == 1)
    {
      digitalWrite(pin1, HIGH);
    }
    else
    {
      digitalWrite(pin1, LOW);
    }
    if(bitRead(laskuri,2) == 1)
    {
      digitalWrite(pin2, HIGH);
    }
    else
    {
      digitalWrite(pin2, LOW);
    }
    if(bitRead(laskuri,3) == 1)
    {
      digitalWrite(pin3, HIGH);
    }
    else
    {
      digitalWrite(pin3, LOW);
    }
    laskuri += 1;
    delay(500);
  }
  clearAllLeds();
}

void show2(int rounds)
{
// see requirements for this function from leds.h  
//Sets leds on in order one by one after a delay. After all are set on after same delay turns all off, reduces delay and start over until
//inputted number of rotations are done. Delay has a minimum for keeping effect clearly visible
  clearAllLeds();
  int viive = 500;
  for(int i = 0; i < rounds; i++)
  {
    delay(viive);
    digitalWrite(pin0, HIGH);
    delay(viive);
    digitalWrite(pin1, HIGH);
    delay(viive);
    digitalWrite(pin2, HIGH);
    delay(viive);
    digitalWrite(pin3, HIGH);
    delay(viive);
    clearAllLeds();
    if(viive > 100)
    {
      viive -= 100;
    }
    else
    {
      viive = 100;
    }
  }
}

void soundGameOver()
//Sad trombone sound when game is lost
//Playes notes of gameOver sound. Because tone is nonblocking, each tone has a matching delay so that notes do actually
//play for desired lenght
{
  for (int i = 0; i < 4; i++)
  {
    tone(A1, gameOver[i], 1000 / noteDuration[i]);
    delay(1000 / noteDuration[i]);
  }
  noTone(A1);
}

void soundHighScore()
//Fanfare for new high score
//Same basic functionality as above, just different notes
{
  for (int i = 0; i < 4; i++)
  {
    tone(A1, highScore[i], 1000 / noteDuration[i]);
    delay(1000 / noteDuration[i]);
  }
  noTone(A1);
}

void soundStartGame()
{
  //Count down to start the game. Set leds on one by one with 3 first playing the same sound as each is lit.
  //Sound off for a delay each time. Fourth led is lit with different sound and after delay all leds and sound turned of and game can begin.
  clearAllLeds();
  int viive = 500;
  for (int i = 0; i < 3; i++)
  {
    if (i == 0)
    {
      digitalWrite(pin0, HIGH);
    }
    if (i == 1)
    {
      digitalWrite(pin1, HIGH);
    }
    if (i == 2)
    {
      digitalWrite(pin2, HIGH);
    } 
    tone(A1, startSound[0]);
    delay(viive);
    noTone(A1);
    delay(viive);
  }
  digitalWrite(pin3, HIGH);
  tone(A1, startSound[1]);
  delay(viive);
  clearAllLeds();
  noTone(A1);
}

void soundPlay(byte buttonNumber)
{
  //Clears previous state of leds and based on number received as input set that led and sound on
  clearAllLeds();
  switch (buttonNumber)
  {
    case 0:
      digitalWrite(pin0, HIGH);
      tone(A1, notes[0]);
      break;
    case 1:
      digitalWrite(pin1, HIGH);
      tone(A1, notes[1]);
      break;
    case 2:
      digitalWrite(pin2, HIGH);
      tone(A1, notes[2]);
      break;
    case 3:
      digitalWrite(pin3, HIGH);
      tone(A1, notes[3]);
      break;
  }
}

void soundStop()
{
  //Sets leds to off and clears sound
  clearAllLeds();
  noTone(A1);
}

void soundAscending(int score)  //Sound quality inconsistent after score 150
{
  //Takes in current score, and converts the score to index of note to play
  //after short sound turns it off again
  int noteNumber = score / 10 - 1;  //First time score 10 -> index 0
  if (noteNumber < 0)               //Just in case limits index to indexes available
  {
    noteNumber = 0;
  }
  if (noteNumber > 14)
  {
    noteNumber = 14;
  }
  tone(A1, ascendingNotes[noteNumber]);
  delay(80);
  noTone(A1);
}