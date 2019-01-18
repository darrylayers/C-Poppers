#include "raylib.h"
#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"


//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

#define NUM_MAX_POPPERS 500
#define NUM_POPPERS 20        // current safe maximum is 110, less that 100 suggested
#define POPPER_SIZE 20

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct Sprite{
    int id;
    Vector2 position;
    Color color;
    double speed;
    bool alive;
} Sprite;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static int screenWidth = 1280;
static int screenHeight = 720;
static int deadSprites = 0;
static int aliveSprites = 0;
static bool gameOver = 0;
static bool win = 0;
static Sprite poppers[NUM_MAX_POPPERS];

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);                          // Initialize game
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
    InitGame();
    
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
void InitGame(void)
{
    // These fields are set here because
    // they need to be reset on game restart
    deadSprites = 0;
    aliveSprites = 0;
    gameOver = 0;
    win = 0;
    
    // Create and load each sprite into the poppers array
    for(int i = 0; i < NUM_POPPERS; i++) 
    {
        Sprite sprite;
        sprite.id = i;
        sprite.speed = 1.3;
        sprite.alive = 1;
        
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
        
        //Check if the ball was clicked at if the game is not over
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
        (CheckCollisionPointCircle(GetMousePosition(), 
        poppers[i].position, POPPER_SIZE)) && !checkGameOver()){    

            // Kill the sprite and increment the dead sprite count
            if(poppers[i].alive == 1){
                poppers[i].alive = 0;
                deadSprites++;    
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
        
        // Create a string to display the score text
        char score[3];
        
        // Convert the deadSprite count to a string
        sprintf(score, "%d", deadSprites);
        
        // Draw the score
        DrawText("SCORE: ", 0, 0, 25, BLACK); 
        DrawText(score, 100, 0, 25, BLACK);

        // Write the win message when appropriate
        if (deadSprites >= NUM_POPPERS) {
            win = 1;
            DrawText("YOU WIN!", screenWidth / 2 - 160,  screenHeight / 2 - 165, 70, BLACK);
        }
           
        // Write the loss message when appropriate           
        if(checkGameOver() && !win) {
            DrawText("YOU LOSE", screenWidth / 2 - 160,  screenHeight / 2 - 165, 70, BLACK); 
        }             
            
        // Write the restart message when appropriate            
        if((gameOver) || (deadSprites >= NUM_POPPERS)){
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
                    InitGame();    
            } 
        }            
    }    
}

//------------------------------------------------------------------------------------
// Draw game (one frame)
//------------------------------------------------------------------------------------
void DrawGame(void){
    
    BeginDrawing();
    
        // Clear the background
        ClearBackground(RAYWHITE);
        
        // Draw the loss line
        DrawLine(0, 600, 1280, 600, RED);   
        
        // For each popper in the game...
        for(int i = 0; i < NUM_POPPERS; i++) {
            
            // If the one we are looking at is alive...
            if (poppers[i].alive == 1) {
                
              // Draw it to the screen 
              DrawCircleV(poppers[i].position, POPPER_SIZE, poppers[i].color);   
              
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
