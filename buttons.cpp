#include "buttons.h"

// Tämä muuttuja on jo määritelty SpedenSpelit.ino:ssa
extern volatile int buttonNumber;     // kertoo mikä nappi painettiin

// Jokaiselle napille oma viimeinen painallusaika (debounce)
volatile unsigned long lastPressTime[7] = {0};
const unsigned long debounceDelay = 200; // ms

void initButtonsAndButtonInterrupts(void) {
  // Alustetaan pelinapit (D2-D5)
  for (byte pin = firstPin; pin <= lastPin; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }
  // Start-nappi (D6)
  pinMode(startPin, INPUT_PULLUP);

  // Ota käyttöön Pin Change keskeytykset D-bussille (pinnit 0–7)
  PCICR  |= (1 << PCIE2);   // Enable PCINT for D-bus
  PCMSK2 |= (1 << PCINT18); // Pin 2
  PCMSK2 |= (1 << PCINT19); // Pin 3
  PCMSK2 |= (1 << PCINT20); // Pin 4
  PCMSK2 |= (1 << PCINT21); // Pin 5
  PCMSK2 |= (1 << PCINT22); // Pin 6
}

ISR(PCINT2_vect) {
  unsigned long now = millis();

  // Tarkistetaan pelinapit (D2-D5)
  for (byte pin = firstPin; pin <= lastPin; pin++) {
    if (digitalRead(pin) == LOW) { // nappi painettu
      if (now - lastPressTime[pin] > debounceDelay) { // debounce
        lastPressTime[pin] = now;
        buttonNumber = pin - firstPin; // esim. pin 2 -> button 0
        Serial.print("Nappia ");
        Serial.print(pin);
        Serial.println(" painettu");
      }
    }
  }

  // Tarkistetaan start-nappi (D6)
  if (digitalRead(startPin) == LOW) {
    if (now - lastPressTime[startPin] > debounceDelay) {
      lastPressTime[startPin] = now;
      buttonNumber = 99; // erikoisarvo startille
      Serial.println("Aloitusnappia painettu");
    }
  }
}
