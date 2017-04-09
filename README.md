Arduino-RunnerGame
==========
About
---
This was a quick game I developed on the arduino to familiarize myself with the LCD.

Installing & Running
---
1. Download "Runner.ino" and compile/upload it onto your chip.
2. Setup an arduino circuit similar to what you see below
  ![RunnerCircuit.png](/Circuit.png?raw=true "Arduino Circuit")

Game Settings
---
* difficulty     - changes game difficulty by decreasing game refresh time
* min_wall_delay - minimum number of cycles before the next wall
* inPin          - input pin number
* debounceDelay  - debounce delay time
* refreshTime    - game refresh time 
* LCD            - enable LCD display
* DEBUG          - enable debug display (visible over USART)
