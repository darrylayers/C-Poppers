#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

#define NUM_MAX_POPPERS 110
#define NUM_POPPERS 3        
#define POPPER_SIZE 45

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct Sprite{
    int id;
    Vector2 position;
    Color color;
    double speed;
    bool alive;
    char operation[8];
    int result;
} Sprite;

//TODO: Give each sprite a rec member for easy checking an less calculations


//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static int screenWidth = 1280;
static int screenHeight = 720;
static int correctPops = 0;
static int incorrectPops = 0;
static int aliveSprites = 0;
static int globalResult = 0;
static bool gameOver = 0;
static bool win = 0;
static Sprite poppers[NUM_MAX_POPPERS];

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(bool init);                          // Initialize game
static void UpdateGame(void);                        // Update game (one frame)
static void DrawGame(void);                          // Draw game (one frame)
static void UpdateDrawFrame(void);                   // Update and Draw (one frame)
static Vector2 verifyNoCollision(Vector2 position);  // Check new points for collisions with old ones
static bool checkGameOver(void);                     // Check to see game settings are set to end the game

//------------------------------------------------------------------------------------
// Main function and game loop
//------------------------------------------------------------------------------------
int main()
{
    // Window init
    InitWindow(screenWidth, screenHeight, "Pop that!");
    
    // Initialize game
    InitGame(1);
    
    SetTargetFPS(60);
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        UpdateDrawFrame();
    }
    // Close window and OpenGL context
    CloseWindow(); 
    
    //Nop
    //Nop
    //Nop insert 3 to be safe amirite
    return 0;
}

//------------------------------------------------------------------------------------
// Initialize game
//------------------------------------------------------------------------------------
void InitGame(bool init)
{
    // if param: init true, then this 
    // is the true start of the game
    // and not a restart
    if(!init) {
        // These fields are set here because
        // they need to be reset on game restart
        correctPops = 0;
        incorrectPops = 0;
        aliveSprites = 0;
        gameOver = 0;
        win = 0;
    }
    
    // Create and load each sprite into the poppers array
    for(int i = 0; i < NUM_POPPERS; i++) 
    {
        Sprite sprite;
        sprite.id = i;
        sprite.speed = 1.0;
        sprite.alive = 1;

        // add the default template to the operation array
        // operand1 + operand2
        // each operand can be two chars, so 7 chars needed
        int op1_1 = GetRandomValue(1,9);
        int op1_2 = GetRandomValue(1,9);
        int op2_1 = GetRandomValue(1,9);
        int op2_2 = GetRandomValue(1,9);
                
        sprite.operation[0] = op1_1 + '0';
        sprite.operation[1] = op1_2 + '0';
        sprite.operation[2] = ' ';
        sprite.operation[3] = '+';
        sprite.operation[4] = ' ';
        sprite.operation[5] = op2_1 + '0';
        sprite.operation[6] = op2_2 + '0';
        sprite.operation[7] = '\0';

        int num = (sprite.operation[0]-'0')*10+(sprite.operation[1]-'0');
        int num2 = (sprite.operation[5]-'0')*10+(sprite.operation[6]-'0');

        sprite.result = num + num2;

        // Distribute the 3 colors below to the sprites by id
        if((sprite.id % 2) == 0){
            sprite.color = RED;
        }
        else if((sprite.id % 3) == 0){
            sprite.color = PINK;
        } 
        else if (((sprite.id % 1) == 0)){
            sprite.color = DARKBLUE;
        }
        
        // Generate random x and y values
        int x = GetRandomValue(50, 1230);
        int y = GetRandomValue(-200, -20);
        
        // Create a new position with the previously
        // created x and y coordinates
        Vector2 position = {x, y};  
        
        // Check to see if the new random position
        // is not a duplicate, if it is a non-duplicate
        // is returned and set as the new sprite position
        sprite.position = verifyNoCollision(position);
        
        // Add the sprite to the array of sprites
        poppers[i] = sprite;
        
        // One more bad boy alive!
        aliveSprites++;
    }
}

//------------------------------------------------------------------------------------
// Update game (one frame)
//------------------------------------------------------------------------------------
void UpdateGame(void){
    

        
    // For all of the sprites...
    for(int i = 0; i < NUM_POPPERS; i++) {
        
        int pos = poppers[i].position.y;
        Rectangle rec = {poppers[i].position.x, poppers[i].position.y, 80, 20};
        
        //Check if the ball was clicked at if the game is not over
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
        (CheckCollisionPointRec(GetMousePosition(), rec)) &&
            !checkGameOver() && (pos < 580)){    

            // Kill the sprite and increment the dead sprite count
            if(poppers[i].alive == 1){
                poppers[i].alive = 0;
                
                if(poppers[i].result == globalResult){
                   correctPops++; 
                } else {
                    incorrectPops++;
                }
            }
        }  
        else {      
            //Check to see if impacted a wall
            if ((poppers[i].position.x >= 1260) || (poppers[i].position.y >= 700)){
                
                // Free the sprite in its current position
                Vector2 newPos = {(poppers[i].position.x), (poppers[i].position.y)};
                
                // Load the new position into the sprite to freeze it
                poppers[i].position = newPos;  
            
            } 
            // If no impact, behave normally
            else {
                
                // Create a new position for the sprite
                Vector2 newPos = {(poppers[i].position.x), (poppers[i].position.y + 1*(poppers[i].speed))};
                
                // Load the new position into the sprite to move it
                poppers[i].position = newPos;
            }
        }
        
        if ((correctPops + incorrectPops) == NUM_POPPERS) {
            gameOver = 1;
            win = 0;
        }
        
        // Create a string to display the score text
        char score[3];
        char wrong[3];
        
        // Convert the deadSprite count to a string
        sprintf(score, "%d", correctPops);
        
        sprintf(wrong, "%d", incorrectPops);
        
        // Draw the score
        DrawText("SCORE: ", 0, 0, 25, BLACK); 
        DrawText(score, 100, 0, 25, BLACK);
        
        DrawText("Wrong: ", 0, 20, 25, BLACK); 
        DrawText(wrong, 100, 20, 25, BLACK);        
        

        checkGameOver();

        // Write the win message when appropriate
        if (correctPops >= NUM_POPPERS) {
            win = 1;
            DrawText("YOU WIN!", 470,  screenHeight / 2 - 165, 70, BLACK);
        }
           
        // Write the loss message when appropriate           
        if(gameOver && !win) {
            DrawText("YOU LOSE", 467,  screenHeight / 2 - 165, 70, BLACK); 
        }             
            
        // Write the restart message when appropriate            
        if((gameOver) || (correctPops >= NUM_POPPERS)){
            DrawText("Restart?", screenWidth / 2 - 160,  screenHeight / 2, 70, BLACK);
            
            // Draw the restart button rectangle
            DrawRectangleLines(screenWidth / 2 - 170, screenHeight / 2, 350, 70, BLACK);
            
            // Create the rectange bounds for the restart button
            Rectangle rectangle = {screenWidth / 2 - 170, screenHeight / 2, 350, 70};
            
            // Create a restart position to allow the user to click to restart
            Vector2 restartPos = {screenWidth / 2 - 160,screenHeight / 2};
            
            // Check for the user clicking to restart
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
                (CheckCollisionPointRec(GetMousePosition(), rectangle))){   
                    
                    // Restart!
                    InitGame(0);    
            } 
        }            
    }    
}

//------------------------------------------------------------------------------------
// Draw game (one frame)
//------------------------------------------------------------------------------------
void DrawGame(void){
    
    Vector2 size = {80,20};

    BeginDrawing();
    
        // Clear the background
        ClearBackground(RAYWHITE);
        
        // Draw the loss line
        DrawLine(0, 600, 1280, 600, RED);   
        
        // Draw the help text
        DrawText("Pop the correct expressions without missing any before they all hit the red line to win!", 120, 600, 25, BLACK); 
        
        // For each popper in the game...
        for(int i = 0; i < NUM_POPPERS; i++) {
            
            // If the one we are looking at is alive...
            if (poppers[i].alive == 1) {
            
                // Draw it to the screen                
                Rectangle rectangle = {poppers[i].position.x, poppers[i].position.y, 80, 20};
                
                DrawRectangleRec(rectangle, poppers[i].color);
                
                // Draw each equation in the appropriate location
                DrawText(poppers[i].operation, poppers[i].position.x+3,  poppers[i].position.y, 20, BLACK);
              
            }
        }        
        
        bool drawn = 0; // flag to determine when the result is drawn
        // Find the first viable popper
        for(int i = 0; (i < NUM_POPPERS) && !drawn; i++) {
            // If the one we are looking at is alive...
            if (poppers[i].alive == 1) {
                
   
                DrawText("Pop the expression", 0, 40, 25, BLACK); 
                DrawText("that results to: ", 0, 60, 25, BLACK); 
                
                char s[3]; 
                sprintf(s,"%d", poppers[i].result );
                DrawText(s, 200, 60, 25, RED); 
                
                drawn = 1;
                
                globalResult = poppers[i].result;
            }
        }
        


    EndDrawing();
}

//------------------------------------------------------------------------------------
// Update and Draw (one frame)
//------------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{  
    UpdateGame();
    DrawGame();
}

//------------------------------------------------------------------------------------
// Check new points for collisions with old ones, if a duplicate is found, return
// a non-duplicate, if no duplicate is found, return the origin parameter position
//------------------------------------------------------------------------------------
Vector2 verifyNoCollision(Vector2 position) {

    // For each of the alive sprites...
    for (int i = 0; i < aliveSprites; i++) {

        // Check to see if the current position in check shares any point with
        // an existing point in the game
        
        if(CheckCollisionCircles(position, POPPER_SIZE, poppers[i].position, POPPER_SIZE)) {
            
            // Generate random x and y values
            int x = GetRandomValue(30, 1250);
            int y = GetRandomValue(-200, -20);
            
            // Create a new position with the previously
            // created x and y coordinates
            Vector2 anotherPos = {x, y}; 
           
            // Assign the new position the the old
            position = anotherPos;
            
            // Restart the examining process
            i = 0;    
        }
    }
    return position;
}

//------------------------------------------------------------------------------------
// Check to see game settings are set to end the game
//------------------------------------------------------------------------------------
bool checkGameOver() {
    
    // Copy the var to counter
    // so we do not manipulate
    // the original value
    int counter = aliveSprites;
    
    // For each of the sprites that are alive...
    for (int i = 0; i <= aliveSprites; i++) {
        
        // Grab the y position of the one we are
        // current looking at
        int pos = poppers[i].position.y;
        
        // Check to see if the y value of the point
        // falls below the loss line at y = 600
        // NOTE: Add a 20 value buffer to account
        // for circle radius
        if (pos > 580){
            
            // Decrement counter to account
            // for the dead sprite
            counter--;
        } 
    }
    
    // When the counter is equal to 0,
    // all sprites are dead and the game is over
    if (counter == 0) {
        
        // Set game over flag
        gameOver = 1;
        
        // Return true, game is over
        return 1;
    }
    
    // Return false, game continues!
    return 0;
}
