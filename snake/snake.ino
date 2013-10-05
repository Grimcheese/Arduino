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
                            // Each index of the array can be either HIGH or LOW (1 or 0)
 

// The segments of the snake are represented as numbers starting from
// 1. The lowest number represents the tail of the snake

// Set the size of the matrix array being used
const int MAX_WIDTH = 32;
const int MAX_LENGTH = 32;
int grid[MAX_LENGTH][MAX_WIDTH];

int score = 0; // A score counter that increments when the snake eats an apple.

/*******************************************************************************/

void setup()
{
  int i;
  
  // Initialize the LED Matrix pins
  for(i = 0; i < 6; i++)
  {
    pinMode(matrixPins[i], OUTPUT);    
  }
  
  // Initialize the joystick pins
  for(i = 0; i < 2; i++)
  {
    pinMode(joystickPins[i], INPUT);
  }
  
  // Initialize the clock pins
  for(i = 0; i < 3; i++)
  {
    pinMode(clockPins[i], INPUT);
  }
  
  // clears the grid so that all the cells are 0
  // this means all the LEDS will be off if the grid is
  // rendered at this point in time
  for(int i = 0; i < MAX_LENGTH; i++)
  {
    for(int j = 0; j < MAX_WIDTH; j++)
    {
      grid[i][j] = 0;
    }
  }
}




// Main loop of program that loops until the desired time
// At desired time snake starts up
void loop()
{
  int time = 1;
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
    
  int counter = 4;          // counter represents the head of the snake on the grid
  int lowNum = 1;            // lowNum represents the tail of the snake on the grid
  unsigned int before;      // before marks the time before the calculations take place
                            // before is used as part of the delay calculation
  
  double hCoord;        // The coordinate of the head in the form x.y
  double aCoord;        // The coordinate of the apple in the form x.y
  
  boolean bounds;      
  boolean gameOver = false;
  
  int snakeDirection = 1;
  
  /****************************************/
  
  // Displays a start screen before the game begins and sets
  // initial grid values
  Startup(hCoord, aCoord);  
  
  // main loop the game goes through
  do
  {
    before = millis();
    
    // Take input each loop iteration
    snakeDirection = FindInput(snakeDirection);
    
    // Move the head to a new location based on the direction it is heading
    hCoord = MoveHead(snakeDirection, hCoord);
        
    // test for collisions with the apple, the boundary and the snake
    // 
    // CollisionTest returns 0, 1 or 2
    int collisions = CollisionTest(hCoord, aCoord);
    
    
    // different options depending on collisions
    switch(collisions)
    {
      case 0:
        // no collisions
        EmptyCell(lowNum);
        lowNum++;
        break;
      case 1:
        // apple collision
        
        // Generate a new location for the apple ensuring that
        // it does not occur in the same place as te snae is
        aCoord = GenerateApple();
        
        score++;
        break;
      case 2:
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
void Startup(double hCoord, double aCoord)
{
  // Initial setup of the grid for the game to start
  hCoord = 10.10;
  aCoord = GenerateApple();
  
  UpdateGrid(hCoord, 3);
  UpdateGrid(10.11, 2);
  UpdateGrid(10.12, 1);
  
  return;
}

double GenerateApple()
{
  boolean valid;
  
  int x, y;
  
  do
  {
    double coord = GenerateCoord();
    x = XSplit();
    y = YSplit();
    if(grid[x][y] > 0)
    {
      valid = false;
    }  
    else
    {
      valid = true;
    }
  }while(valid = false);
  
  grid[x][y] = -1;
  return(coord);
  
}

// Generates a random coordinate within the bounds of
// the Matrix
double GenerateCoord()
{
  int x = random(0, MAX_LENGTH);
  int y = random(0, MAX_WIDTH);
  
  double coord = x + (y / 100.0);
  
  return(coord);
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


int XSplit(double coord)
{
  int x = (int)coord;
  return(x);
}

int YSplit(double coord)
{
  int x = XSplit(coord);
  int y = (coord - XSplit) * 100;
  
  return(y);
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
       drawLine(i,j, i, j);   // MAY NEED TO CHANGE PIN NAMES
     }
     else
     {
       // turn off LED located at i, j
     }
   }
 } 
}

// Changes the snakeDirection variable based on which button 
// the player is pressing: up, down, left, right
int FindInput(int snakeDirection)
{ 
  // cycles through each direction testing to see if it
  // is high or low
  for(int i = 0; i < 4; i++)
  {
    joystickState[i] = digitalRead(joystickPin
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


// Extra function that will print letters on the 
// LED matrix not required for the snake game but
// could be useful

// Will print a letter based on the starting location of the line
// and the letters that preceeded it
int PrintLetter(char letter, double startLoc, int offset)
{
  int offset;
  
  x1 = XSplit(startLoc);
  x1 = x1 + offset;
  
  y1 = YSplit(startLoc);
  
  switch(letter)
  {
    case 'a' :
        x2 = x1 + 3;
        y2 = y1 + 4;
        x3 = x1 + 4;
        y3 = y1 + 1;
    
        drawLine(x1, y1, x2, y2);
        drawLine(x2, y2, x3, y1);
        drawLine(x1, y3, x3, y3);
        
        offset = 6;
      break;
    case 'b' :
      break;
    case 'c' :
      break;
    case 'd' :
      break;
    case 'e' :
      break;
    case 'f' :
      break;
    case 'g' :
      break;
    case 'h' :
      x2 = x1 + 3;
      y2 = y1 + 4;
      y3 = y1 + 2;
    
      drawLine(x1, y1, x1, y2);
      drawLine(x2, y1, x2, y2);
      drawLine(x1, y3, x2, y3);
      
      offset = 4;
      break;
    case 'i' :
      drawLine(x1, y1, x2, y1);
      drawLine(x1, y2, x2, y2);
      drawLine(x3, y1, x3, y2);
      break;
    case 'j' :
      break;
    case 'k' :
      break;
    case 'l' :
      break;
    case 'm' :
      break;
    default:
  }
  
  return(offset);
}




