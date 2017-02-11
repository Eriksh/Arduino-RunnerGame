Arduino-RunnerGame
==========
About
---
This was a quick game I developed on the arduino to familiarize myself with the LCD.

Installing & Running
---
Setup an arduino circuit similar to what you see below:
[[https://github.com/Eriksh/Arduino-RunnerGame/blob/master/Arduino-RunnerGame/RunnerCircuit.png|alt="arduino circuit"]]

Afterwards, download "Runner.ino", open it in arduino IDE and compile/upload it
ont your chip.

Game Settings
---
difficulty     - changes game difficulty by decreasing game refresh time
min_wall_delay - minimum openings between walls, prevents players from coming accross situations they cant avoid
inPin          - input pin number
LCD            - enable LCD display
DEBUG          - enable debug display (visible over USART)
