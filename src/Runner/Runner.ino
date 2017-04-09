#include <LiquidCrystal.h>

// SET GAME SETTINGS
//////////////////////////////////////////////////////////////////
int difficulty = 2;
int min_wall_delay = 4;
int inPin = 13;
int debounceDelay = 25;
int refreshTime = 16000; 

//Run on LCD 
bool LCD = true;

//Set Debug Mode
bool DEBUG = false;

// GAME VARIABLES
//////////////////////////////////////////////////////////////////

//Level Array
int level[2][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

//LCD Designs
byte empty[8] = {0b00000, 0b00000, 0b00000, 0b00000,
                 0b00000, 0b00000, 0b00000, 0b00000};

byte wall[8] = {0b11111, 0b11111, 0b11111, 0b11111,
                0b11111, 0b11111, 0b11111, 0b11111};
                
byte player[8] = {0b00100, 0b01010, 0b00100, 0b00100,
                  0b01110, 0b10101, 0b00100, 0b01010};

//LCD Pin Settings
LiquidCrystal lcd(2, 3, 8, 9, 10, 11);

//Level Object ID
// 0 = empty
// 1 = wall
// 2 = player
int objectID[3] = {0, 1, 2};

//Collision Types
int crash = 0;
int maneuver = 1;

//Control Variables
int wall_delay_counter = 0;
int player_location = 0;


// INITIALIZE GAME
//////////////////////////////////////////////////////////////////
void setup() {
  // Set Player Location
  level[0][0] = objectID[2];

  //Initialize Input Pin
  pinMode(inPin, INPUT);

  //Enable Screen & Set AutoScroll

  //Prevent Difficulty Divide by 0 Error
  if(difficulty == 0)
    difficulty = 1;

  //LCD Settings
  if(LCD == true) {
    lcd.begin(16, 2); 
    lcd.createChar(0, empty);
    lcd.createChar(1, wall);
    lcd.createChar(2, player);
  }
  
  //Debug Mode Settings
  if(DEBUG == true)
    Serial.begin(9600); 
}

// MAIN GAME LOOP
//////////////////////////////////////////////////////////////////
void loop() {

  //Check For Crash Collision
  Collision_Check(crash);

  //Shift Array Left
  Shift_Level();

  //Create A New Wall
  if(wall_delay_counter >= min_wall_delay) {
    Create_Wall();
    wall_delay_counter = 0;
  }

  //Display
  Display();

  //Wait For User Input & Determine Difficulty
  UserInput();

  //Reset Delay
  wall_delay_counter++;
}

// CHECK FOR COLLISION
//////////////////////////////////////////////////////////////////
// Function does the following:
//    * If input is 0 (crash), checks to see if player crashes into wall
//    * If input is 1 (maneuver), checks to see if player moved into wall
//////////////////////////////////////////////////////////////////
void Collision_Check(int type) {

  //Player Runs Into Wall
  if (type == crash) {
    if(level[player_location][1] == objectID[1])
      FailureScreen();
  }

  //Player Maneuvers Into Wall
  if (type == maneuver) {
    
    if(player_location == 0) {
      if(level[1][0] == objectID[1])
        FailureScreen();
    }
    
    if(player_location == 1) {
      if(level[0][0] == objectID[1])
        FailureScreen();
    }
  }
}

// SHIFT GAME ARRAY TO THE LEFT
//////////////////////////////////////////////////////////////////
// Function does the following:
//    * Shifts the player up one element in the array
//    * Shifts the array to the left
//    * Sets last elements in array as empty
//////////////////////////////////////////////////////////////////
void Shift_Level() {

  //Move Player Up One Element
  level[player_location][1] = level[player_location][0];
    
  //Shift Level & Detemie if player Lost
  for(int shift = 1; shift < 16; shift++) {

    //Shift Array Left
    level [0][shift-1] = level[0][shift];
    level [1][shift-1] = level[1][shift];
  }

  //Set Last Array Elements To Empty
  level [0][15] = objectID[0];
  level [1][15] = objectID[0];
}

// CREATE A RANDOM WALL
//////////////////////////////////////////////////////////////////
// Function does the following:
//    * Decides weather to place a new wall block
//    * Decides where to place a new wall block
//    * Adds wall id to the array
//////////////////////////////////////////////////////////////////
void Create_Wall() {
    
  //Randomly Decide Wether to Create A Wall
  if(random(5) <=  3){
    //Create Wall On Upper Array
    if(random(2) == 0)
      level[0][15] = objectID[1];
    //Create Wall On Lower Array
    else
      level[1][15] = objectID[1];
  }
}

// DISPLAY GAME
//////////////////////////////////////////////////////////////////
// Function does the following:
//    * Display info in level array onto LCD Screen
//    * Display level array via USART via Debug Mode
//////////////////////////////////////////////////////////////////
void Display() {
  
  //LCD Display Mode
  if(LCD == true) {
    for(int i = 0; i < 16; i++){
      lcd.setCursor(i, 0);
      lcd.write(level[0][i]); 
    }
    for(int i = 0; i < 16; i++){
      lcd.setCursor(i, 1);
      lcd.write(level[1][i]); 
    }
  }

  //Debug Display Mode
  if(DEBUG == true) {
    for(int i = 0; i < 16; i++){
      Serial.print(level[0][i]);
    }
    Serial.print("\n");
    for(int i = 0; i < 16; i++){
      Serial.print(level[1][i]);
    }
    Serial.print("\n");
    Serial.println("");  
  }
}

// USER INPUT
//////////////////////////////////////////////////////////////////
// Function does the following:
//    * Reads player input
//    * Applies button debounce to prevent input error
//    * Determins if there is a collision issue
//    * Shifts player to the next array element
//    * Determins game speed by difficulty value
//////////////////////////////////////////////////////////////////
void UserInput() {

  //Input Variables
  int counter = 0;

  //Debounce Variables
  int buttonState;
  int lastButtonState = LOW;
  int lastDebounceTime = 0;

  do{
    //Read Input
    int reading = digitalRead(inPin);

    //Detect Change In Button State & Set Timer
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }

    //Handle Debounce & Player Movement
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;

        //Move Player
        if (buttonState == 1) {
          if(player_location == 0) {
            Collision_Check(1) ;
            player_location = 1;
            level[0][0] = objectID[0];
            level[1][0] = objectID[2];
            Display();
          }
          else{
            Collision_Check(1);
            player_location = 0;
            level[0][0] = objectID[2];
            level[1][0] = objectID[0];
            Display();
          }
        }
      }
    }

  //Handle Variables
  lastButtonState = reading;
  counter++;
  }while(counter < refreshTime/difficulty);
}

// FAILURE SCREEN
//////////////////////////////////////////////////////////////////
// Function does the following:
//    * Clears LCD
//    * Prints "Failure" onto screen
//    * Waits till user presses button
//    * Resets game
//////////////////////////////////////////////////////////////////
void FailureScreen() {

  //Read Input
  int reading = LOW;

  //Clear LCD & 
  if(LCD == true) {
    for(int i = 0; i < 16; i++){
      lcd.setCursor(i, 0);
      lcd.write(objectID[0]); 
    }
    for(int i = 0; i < 16; i++){
      lcd.setCursor(i, 1);
      lcd.write(objectID[0]); 
    }
    lcd.setCursor(4, 0);
    lcd.print("Failed!");
  }

  //Delay Until Button Pressed
  delay(1000);
  while (reading == 0){
    reading = digitalRead(inPin);
    Serial.print(reading);
    delay(100);
  }

  // Reset Level
  for(int i = 0; i < 16; i++){
      level[0][i] = objectID[0];
  }
  for(int i = 0; i < 16; i++){
      level[1][i] = objectID[0];
  }

  // Set Player Location
  player_location = 0;
  level[0][0] = objectID[2];
}

