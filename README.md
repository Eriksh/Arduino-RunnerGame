Arduino-RunnerGame
==========
About
---
This was a quick game I developed on the arduino to familiarize myself with the LCD.
It is very basic, any movements you make the player do will not be seen until the 
next refresh cycle.

Installing & Running
---
1. Download "Runner.ino" and compile/upload it onto your chip.
2. Setup an arduino circuit similar to what you see below
  ![RunnerCircuit.png](/Circuit.png?raw=true "Arduino Circuit")

Game Settings
---
* difficulty     - changes game difficulty by decreasing game refresh time
* min_wall_delay - minimum delay before the next wall
* inPin          - input pin number
* LCD            - enable LCD display
* DEBUG          - enable debug display (visible over USART)
