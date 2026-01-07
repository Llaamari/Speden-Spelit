#include <SoftwareWire.h>
#include "leds.h"
#include "buttons.h"
#include "display.h"
#include "SpedenSpelit.h"
#include <EEPROM.h>





unsigned long lastButtonPressTime = 0;
volatile int buttonNumber = -1;         
volatile bool newTimerInterrupt = false; 
volatile int timerInterruptCount = 0;    
const int maxButtons = 4;
const int maxSequenceLength = 999;
byte sequence[maxSequenceLength];       
int sequenceLength = 0;                 
int playerIndex = 0;                    
bool awaitingInput = false;             
int correctPresses = 0;
bool gameRunning = false;
const uint16_t BASE_OCR1A = 15624;     






void setup() 
{
  Serial.begin(9600);
  cli(); 
  initializeDisplay();
  initButtonsAndButtonInterrupts();
  initializeLeds();
  initializeTimer();
  randomSeed(analogRead(A0));
  sei(); 
  initEEPROM(); //jos EEPROM on tyhja alusta top scoret
  showOnSevenSegment(100);
  delay(3000);
  textOnLcd("<-Press to start");
}


void loop() 
{
    
    if (buttonNumber >= 0) 
    {
      lastButtonPressTime = millis();
      int btn = buttonNumber;
        buttonNumber = -1;

        // Aloitusnäppäin
        if (btn == 99) 
        {  
          if (!gameRunning) 
          {
            startTheGame();
          }
        }
        // Scoreboard-näppäin pelin ulkopuolella
        else if (btn ==scoreButton && !gameRunning)
        {
            showScoreBoard();
        }
        // Idleanimaatio 1 pelin ulkopuolella
        else if (btn == 0 && !gameRunning)
        {
          show1();
        }
        // Idleanimaatio 2 pelin ulkopuolella
        else if (btn == 1 && !gameRunning)
        {
          int repetitions = random(2,6);
          show2(repetitions);
        }
        // Peli­näppäimet pelin aikana
        else if (gameRunning && btn >= 0 && btn < maxButtons) 
        {
          checkGame(btn);
        }
    }

    // Sekvenssin lisäys ajastimen mukaan
    if (newTimerInterrupt) 
    {
        newTimerInterrupt = false;

        if (gameRunning && sequenceLength < maxSequenceLength) 
        {
          
        byte newLed;
        do {
            newLed = random(maxButtons);
        } while (sequenceLength > 0 && newLed == sequence[sequenceLength - 1]);

        sequence[sequenceLength] = newLed;
        sequenceLength++;
        clearAllLeds();
        setLed(newLed);
        awaitingInput = true;
        }
    }
    
  if (!gameRunning && (millis() - lastButtonPressTime > 60000)) 
  { 
    enterSleepMode(); 
    lastButtonPressTime = millis(); 
  }

}

//tarkista pelaajan painallus
void checkGame(byte btnPressed) 
{
    if (!awaitingInput) return; 
    if (btnPressed == sequence[playerIndex]) 
    {
      playerIndex++; 
      correctPresses++;
      showNumber(correctPresses);

      
        if (playerIndex == sequenceLength) {
            awaitingInput = false; // Odota seuraavaa arvontaa
        }

    } 
    else 
    {
        //Peli ohi eli GAME OVER
      
      gameRunning = false;
      awaitingInput = false;
      soundGameOver();
      clearAllLeds();
      textOnLcd("Game Over");
      delay(1000);
      showNumber(correctPresses); // Näytä pelaajan pisteet
      
      updateScoreBoard((uint16_t)correctPresses); // Tallenna score

    delay(2000); 

    // Resetoi peli
    sequenceLength = 0;
    playerIndex = 0;
    correctPresses = 0;
    timerInterruptCount = 0;
    OCR1A = BASE_OCR1A;
    TIMSK1 &= ~(1 << OCIE1A);

    textOnLcd("<-press to start");
  }
}


// Aloita peli 
void startTheGame() 
{


  soundStartGame();
  delay(1000);

  initializeGame();

  noInterrupts();              //testi
  newTimerInterrupt = false;   
  timerInterruptCount = 0;
  OCR1A = BASE_OCR1A;        
  TCNT1 = 0;                   
  TIFR1 |= (1 << OCF1A);       
  TIMSK1 |= (1 << OCIE1A);     
  interrupts();

}

//pelin arvojen alustus
void initializeGame()
{
  sequenceLength = 0;
  playerIndex = 0;
  correctPresses = 0;
  gameRunning = true;
  awaitingInput = true;
  byte firstLed = random(maxButtons);
  sequence[0] = firstLed;
  sequenceLength = 1;
  clearAllLeds();
  setLed(firstLed);
  showNumber(0);    
}

// Ajastimen asetukset
void initializeTimer() 
{
  TCCR1A = 0;  
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);             
  TCCR1B |= (1 << CS12) | (1 << CS10); 
  OCR1A = BASE_OCR1A;
  TIMSK1 |= (1 << OCIE1A);            
}


// Ajastimen keskeytys
ISR(TIMER1_COMPA_vect) 
{
  timerInterruptCount++;
  if (timerInterruptCount >= 10) 
  {
    timerInterruptCount = 0;
    uint32_t newOCR = (uint32_t)OCR1A * 9u / 10u;   
    OCR1A = (uint16_t)newOCR;
  }
  newTimerInterrupt = true; 
}

// Lue top 3 EEPROM:ista
void readHighScores(uint16_t scores[maxHighScores]) {
  for (int i = 0; i < maxHighScores; i++) {
    int addr = i * 2;
    scores[i] = EEPROM.read(addr) | (EEPROM.read(addr + 1) << 8);
  }
}

// Kirjoita top 3 EEPROM:iin
void writeHighScores(uint16_t scores[maxHighScores]) {
  for (int i = 0; i < maxHighScores; i++) {
    int addr = i * 2;
    EEPROM.write(addr, scores[i] & 0xFF);
    EEPROM.write(addr + 1, (scores[i] >> 8) & 0xFF);
  }
}

// Päivitä top 3 jos uusi pistemäärä mahtuu joukkoon
void updateScoreBoard(uint16_t newScore) {
  uint16_t scores[maxHighScores];
  readHighScores(scores);

  for (int i = 0; i < maxHighScores; i++) {
    if (newScore > scores[i]) {
      soundHighScore();
      // Siirrä pienempiä alaspäin
      for (int j = maxHighScores - 1; j > i; j--) {
        scores[j] = scores[j - 1];
      }
      scores[i] = newScore;
      break;
    }
  }

  writeHighScores(scores);
}

// Näytä scoreboard LCD:ssä
void showScoreBoard() {
  uint16_t scores[maxHighScores];
  readHighScores(scores);
  showScoreBoard(scores);
 
  delay(4000);

  // Palaa alkuvalikkoon
  textOnLcd("<-Press to start");
}

void initEEPROM()
{
  uint16_t scores[maxHighScores];
  bool needsInit = false;

  for( int i = 0; i <maxHighScores; i++)
  {
    int addr = i * 2;
    uint16_t val = EEPROM.read(addr) | (EEPROM.read(addr+1) << 8);
    if (val == 0xFFFF)
    {
      needsInit = true; // EEPROM default
      break;
    }
  }
  
  if (needsInit)
  {
    for(int i = 0; i < maxHighScores; i++)
    {
      scores[i] = 0;
    }
    writeHighScores(scores);
  }
}
