# Speden Spelit (Arduino Game Project)

## Overview

**Speden Spelit** is an Arduino-based reaction game developed as part of the *Application Project* course in the Information Technology degree program at Oulu University of Applied Sciences (OAMK).

The project was implemented by a team of four students and focuses on embedded programming, hardware interaction and modular software design using interrupts, timers, LEDs, buttons, displays and sound.

The game challenges the player to react quickly by pressing the correct button corresponding to a randomly lit LED. As the game progresses, the speed increases, making the game more challenging.

---

## Hardware & Technologies

- Arduino microcontroller
- 4 game buttons + 1 start button
- 4 LEDs
- Buzzer / sound output
- LCD display (I2C)
- Two 7-segment displays
- EEPROM (for high score storage)

Key technical concepts:
- Interrupt Service Routines (ISR)
- Timers and timer interrupts
- Button debouncing
- Modular C++ design for embedded systems

---

## Project Structure

```text
├── SpedenSpelit.ino        # Main game logic
├── SpedenSpelit.h
├── buttons.h / buttons.cpp
├── leds.h / leds.cpp
├── display.h / display.cpp
```

## Module Descriptions

### Buttons Module (`buttons.h / buttons.cpp`)

- Handles 4 game buttons and 1 start button
- Uses hardware interrupts on Arduino digital pins
- Implements software debouncing
- Updates a global `buttonNumber` variable
- Identifies which button was pressed inside an ISR

### Display Module (`display.h / display.cpp`)

- Controls:
  - One LCD display via I2C
  - Two 7-segment displays
- Displays:
  - Current score
  - High score
  - Game-related messages
- LCD enables extended information compared to 7-segment displays

### LEDs & Sound Module (`leds.h / leds.cpp`)

- Controls 4 LEDs and sound effects
- Only one LED is active at a time during gameplay
- LED pin mapping is abstracted from game logic
- Includes sound effects:
  - Game start countdown
  - Game over sound
  - High score fanfare
  - Speed-up sound as score increases
- Includes LED animations:
  - `show1()` – binary counting with LEDs
  - `show2()` – progressive LED animation with increasing speed

### Game Logic (SpedenSpelit.ino / SpedenSpelit.h)

- Initializes hardware, timers and interrupts
- Uses a timer ISR to:
  - Randomly select the next LED
  - Control game speed
- Main loop:
  - Checks for timer events
  - Checks button presses
  - Verifies correct/incorrect input
- Game ends on incorrect button press
- Final score is displayed after game over

**High Score System**
- Top 3 scores are stored in EEPROM
- Scoreboard can be viewed before or after a game
- Scores are updated and sorted automatically

## Additional Features

- Weekly team meetings held throughout the project
- Flowcharts created for:
  - Game logic
  - Timer ISR
  - Button interrupt service routine
- Work time tracking used to monitor project effort
- Sound effects implemented as an extra goal
- LCD scoreboard added as an additional feature

## Development Process

- Project planning and documentation completed before implementation
- Tasks divided by module
- Each module implemented and tested independently
- Final integration combined all modules into a complete game
- Weekly milestones followed according to the project plan

## Demo Video

🎥 **Project demonstration video:**<br>
https://drive.google.com/file/d/1BP8mjp6vmrVlAaKURobh-cAQ5VSyjZxa/view?usp=drivesdk

## Course Context

This project was completed as part of:
- Application Project (9 ECTS)
- Arduino and microcontroller programming studies
- Focus on interrupts, timers and low-level hardware interaction

## Authors

Project implemented by a team of four students:<br>
[Laura](https://github.com/Llaamari)<br>
[Juha](https://github.com/Sahid1981)<br>
[Valtteri](https://github.com/TTEVAR)<br>
[Arttu](https://github.com/Ard3J)
