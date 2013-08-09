// Snake Arduino Project
// Author: Alby Chawula
// Date: 7/8/2013
// 
// This program is designed to allow the user to play snake
// by taking input from buttons and outputting to an LED matrix
//*************************************************************


/***************************
GLOBAL VARIABLE DECLARATIONS

LED/button pin numbers are
stored as arrays
****************************/
int joystickPins[2] = {};    
int matrixPins[6] = {};
int clockPins[3] = {};

int joystickState[4] = {0}; // stores the state of the 4 positions of the joystick
                            // index numbers: 0 = up, 1 = down, 2 = left, 3 = right
                            
// Set the size of the matrix array being used
const int MAX_WIDTH = 32;
const int MAX_LENGTH = 32;
int grid[MAX_LENGTH][MAX_WIDTH];

int score = 0;

/*******************************************************************************/

// FUNCTION PROTOTYPES
void FindInput(void);           // Takes user input and assigns 
int PinRead(int pin);           // Reads input from a specific pin
void UpdateGrid();              // Updates the grid array 

void setup()
{
  int i;
  
  // Initialize the LED Matrix pins
  for(i = 0; i < 6; i++)
  {
    pinMode(matrixPins[i], OUTPUT);    
  }
  
  // Initialize the joystick pins
  for(i = 0; i < 2; i+)
  {
    pinMode(joystickPins[i], INPUT);
  }
  
  // Initialize the clock pins
  for(i = 0; i < 3; i++)
  {
    pinMode(clockPins[i], INPUT);
  }
}




// Main loop of program that loops until the desired time
// At desired time snake starts up
void loop()
{
  
  if(time)
  {
    // Starts the snake game when it is a specific time
    // the time is found using the clock
    snake();
  }
  
 
}

void snake()
{
  // Varible declaration for snake
  // Variables needed are grid, counters, 
  

  
  const int WAITTIME = 500; // Delay in ms between each refresh
    
  int counter = 4;          // counter and lowNum are used to identify different
  int lowNum = 1;           // segments of the snake
  unsigned int before;
  
  double hCoord;        // The coordinate of the head in the form x.y
  double aCoord;        // The coordinate of the apple in the form x.y
  
  boolean boounds;
  boolean gameOver = false;
  
  int snakeDirection;
  
  /****************************************/
  
  // Displays a start screen before the game begins
  Startup();  
  
  // main loop the game goes through
  do
  {
    before = millis();
    
    // INSERT CALCULATION CODE HERE //
    // Take input each loop iteration
    snakeDirection = FindInput();
    
    // Move the head to a new location based on the direction it is heading
    hCoord = MoveHead(snakeDirection);
        
    // test for collisions with the apple, the boundary and the snake
    // 
    // CollisionTest returns 0, 1 or 2
    int collisions = CollisionTest(hCoord, aCoord);
    
    
    // different options depending on collisions
    switch(collisions)
    {
      case(0):
        // no collisions
        EmptyCell(lowNum);
        lowNum++;
        break;
      case(1):
        // apple collision
        
        // Generate a new location for the apple ensuring that
        // it does not occur in the same place as te snae is
        GenerateApple();
        
        score++;
        break;
      case(2):
        // snake collision or wall collision
        gameOver == true;
        break;
      default: //wtf
    }
    
    // This section of code ensures that the screen is updated at a regular pace
    // The time is taken before and after the calculations and the difference
    // is compared with the desired delay time
    SpeedLimiter(before, WAITTIME);
    
    // Update the grid and the display
    UpdateGrid(hCoord, counter++);
    UpdateGrid(aCoord, -1);
    UpdateLED();
    
  }while(gameOver == false);
  
  
}


// Function that displays the start screen of snake
void Startup()
{
  
  
  
  // Initial setup of the grid for the game to start
  UpdateGrid(10.10, 3);
  UpdateGrid(10.11, 2);
  UpdateGrid(10.12, 1);
  
  return;
}

// Checks to see if the head of the snake is inside the bounds of the field
// returns a boolean: true means the head is inside. false means outside
boolean BoundaryCheck(double hCoord);
{
  int xVal = (int)hCoord);
  int yVal = (int)((hCoord - (int)hCoord) * 100);
 
 boolean test = false;
 
  if(xVal >= 0 && xVal < MAX_WIDTH)
 {
   if(yVal >= 0 && yVal < MAX_LENGTH)
   {
     test = true;
   }
 } 
 return(test);
}


// Tests to see if the head of the snake intercepts with itself
// returns a boolean value
boolean SnakeCollide(double hCoord)
{
  xVal = (int)hCoord;
  yVal = (int)((hCoord - xVal) * 100);
  
  if(grid[xVal][yVal] > 0)
  {
    return(true);
  }
  else
  {
    return(false);
  }
}

// Checks to see if any collisions occur
// If collisions occur the type is returned as an int
// 0 = no collisions, 1 = apple collision, 2 = end of game collision
int CollisionTest(double hCoord, double aCoord)
{
    int collisions;
    
    if{Abs(hCoord - aCoord) < 0.01) // Eating an apple
       collisions = 1
    }
    else if(BoundaryCheck(hCoord) == false) // Going outside the boundary 
    {
      collisions = 3; 
    }
    else if(SnakeCollide(hCoord) // If the snake collides with itself
    {
      collisions = 3;
    }
    else // No collisions occur 
    {
      collisions = 0;
    }
    
    return(collisions);
}


// Ensures that the screen is updated at a regular rate
void SpeedLimiter(unsigned long before, const int WAITTIME)
{
  
    unsigned int calcTime = millis() - before;
    if(calcTime < WAITTIME)
    {
      delay(WAITTIME - calcTime);
    }
    
    return;
}














double SearchGrid(int value)

// Searches the grid for a specified value
// and returns the coordinate as a double
{
  for(int i = 0; i < MAX_LENGTH; i++)
  {
    for(int j = 0; j < MAX_WIDTH; j++)
    {
      if(grid[i][j] == value)
      {
        double coord = i + (j / 100.0);
        return(coord);
      }
    }
  }
}


// Finds the location of a snake segment in the grid and changes the value
// of that grid location to 0 (an empty space)
void EmptyCell(int value)
{
  double coord = SearchGrid(value);
  UpdateGrid(coord, 0);
}


// Takes a grid coordinate and updates that location
// with a new value
void UpdateGrid(double coord, int value)
{
  int xVal = (int)coord;
  int yVal = (int)((coord - xVal) * 100);

  grid[xVal][yVal] = value;

  return;  
}

// Moves the coordinate of the head based on the direction
// selected by the user. 
// Returns the new coordinate of the head 
double MoveHead(int input, double hCoord)
{
  
  if(input == 0)
  {
    hCoord = hCoord + 0.1;
  }
  else if(input == 1)
  {
    hCoord = hCoord - 0.1;
  }
  else if(input == 2)
  {
    hCoord = hCoord - 1;
  }
  else if(input == 3)
  {
    hCoord = hCoord + 1;
  }
  
  return(hCoord);
}

// Updates the LEDs in the matrix to the most current state
// can eithr be on or off
void UpdateLED()
{
  for(int i = 0; i < MAX_LENGTH; i++)
 {
   for(int j = 0; j < MAX_WIDTH; j++)
   {
     if(grid[i][j] != 0)
     {
       digitalWrite(pin[i][j], HIGH);   // MAY NEED TO CHANGE PIN NAMES
     }
     else
     {
       digitalWrite(pin[i][j], LOW);
     }
   }
 } 
}

// Changes the direct variable based on which button 
// the player is pressing: up, down, left, right
void FindInput()
{
  int snakeDirection;
  
  
  // cycles through each direction testing to see if it
  // is high or low
  for(int i = 0; i < 4; i++)
  {
    joystickState[i] = joystickPin
  }
  
  
  for(int i = 0 ; i < 4; i++)
  {
    if(joystickState[i] == HIGH)
    {
      snakeDirection = i;
    }
  }
    
  return(snakeDirection);
}

