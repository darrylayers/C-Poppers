//TODO look into seeding the random function with the time
//TODO vertically center text on restart button 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // For string concatenation
#include "raylib.h"     // Raylib graphics engine
#include "pop.h"        // Pop header file

// This define is used to find the exact center location to draw text
#define CENTER(str, size) \
    (GetScreenWidth() / 2) - (MeasureText(str, size)) / 2

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

#define NUM_MAX_SPRITES 30
#define NUM_SPRITES 2       
#define SPRITE_SIZE 45

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct Sprite{
    Rectangle rec;
    Vector2 position;
    Color color;
    double speed;
    bool alive;
    int id;
    char operation[8];
    int result;
    int lifeCycle;
} Sprite;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

int correctPops = 0;
int incorrectPops = 0;
int aliveSprites = 0;
int globalResult = 0;
bool gameOver = 0;
bool win = 0;
Sprite sprites[NUM_MAX_SPRITES];

//------------------------------------------------------------------------------------
// Main function and game loop
//------------------------------------------------------------------------------------
int main()
{
    //Declare the size of the screen
    int screenWidth = 1280;
    int screenHeight = 720;
    
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
    
    // Create and load each sprite into the sprites array
    for(int i = 0; i < NUM_SPRITES; i++) 
    {
        Sprite sprite;
        sprite.id = i;
        sprite.speed = 1.0;
        sprite.alive = 1;
        sprite.color = DARKGREEN;
        sprite.lifeCycle = 1;

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

        // Generate random x and y values
        int x = GetRandomValue(80, 1200);
        int y = GetRandomValue(-500, -20);
        
        // Create a new position with the previously
        // created x and y coordinates
        Vector2 position = {x, y};  
        
        // Check to see if the new random position
        // is not a duplicate, if it is a non-duplicate
        // is returned and set as the new sprite position
        sprite.position = verifyNoCollision(position);
        
        // Add the sprite to the array of sprites
        sprites[i] = sprite;
        
        // One more bad boy alive!
        aliveSprites++;
    }
}

//------------------------------------------------------------------------------------
// Update game (one frame)
//------------------------------------------------------------------------------------
void UpdateGame(void){
    
    // For all of the sprites...
    for(int i = 0; i < NUM_SPRITES; i++) {
        
        int pos = sprites[i].position.y;
        Rectangle rec = {sprites[i].position.x, sprites[i].position.y, 80, 20};
        
        if (sprites[i].lifeCycle == 2) {
            sprites[i].speed = 1.2;
        } else if (sprites[i].lifeCycle >= 3) {
            sprites[i].speed = 1.8;
        }
        
        //Check if the ball was clicked at if the game is not over
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
        (CheckCollisionPointRec(GetMousePosition(), rec)) &&
            !checkGameOver() && (pos < 580)){    

            // Kill the sprite and increment the dead sprite count
            if(sprites[i].alive == 1){
                sprites[i].alive = 0;
                
                if(sprites[i].result == globalResult){
                   correctPops++; 
                } else {
                    incorrectPops++;
                }
            }
        }  
        else {      
            //Check to see if impacted a wall
            if ((sprites[i].position.x >= 1260) || (sprites[i].position.y >= 700)){
                
                // Free the sprite in its current position
                Vector2 newPos = {(sprites[i].position.x), (-60)};
                Vector2 newPos2 = {(sprites[i].position.x), (sprites[i].position.y)};

                if((sprites[i].lifeCycle >= 3)) {

                    sprites[i].position = newPos2; 
                    
                }   else {
                    sprites[i].position = newPos;
                }
                
                // Load the new position into the sprite to freeze it
                  
                sprites[i].lifeCycle++; 
                
                if(sprites[i].lifeCycle == 2){
                    sprites[i].color = YELLOW;
                } else if (sprites[i].lifeCycle == 3) {
                    sprites[i].color = RED;
                }
               
            
            } 
            // If no impact, behave normally
            else {
                // Create a new position for the sprite
                Vector2 newPos = {(sprites[i].position.x), (sprites[i].position.y + 1*(sprites[i].speed))};
                
                // Load the new position into the sprite to move it
                sprites[i].position = newPos;
            }
        }
        
        if ((correctPops + incorrectPops) == NUM_SPRITES) {
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
        DrawText("SCORE: ", 30, 0, 25, BLACK); 
        DrawText(score, 130, 0, 25, BLACK);
        
        DrawText("Wrong: ", 30, 20, 25, BLACK); 
        DrawText(wrong, 130, 20, 25, BLACK);        
        

        checkGameOver();

        // Write the loss message when appropriate           
        if (correctPops >= NUM_SPRITES){
            win = 1;
            DrawText("YOU WIN!", CENTER("YOU WIN!", 70),  GetScreenHeight() / 2 - 165, 70, BLACK);
            
        } else if(gameOver && !win) {
            

            DrawText("YOU LOSE", CENTER("YOU LOSE", 70),  GetScreenHeight() / 2 - 165, 70, BLACK); 
            
            char youLose[18] = "Final score: ";
            char finalScore[5];
            double score = ((double)correctPops / NUM_SPRITES);
            sprintf(finalScore, "%.2f", score);     
            strcat (youLose,finalScore);
            
            DrawText(youLose, CENTER(youLose, 70),  GetScreenHeight() / 2 - 100, 70, BLACK);        // 100 is the offset to make room
                                                                                                    // between the "YOU LOSE" string and                                             
                                                                                                    // the final score text
           
        }

        
            
        // Write the restart message when appropriate            
        if((gameOver) || (correctPops >= NUM_SPRITES)){
            DrawText("Restart?", CENTER("Restart?", 70),  GetScreenHeight() / 2, 70, BLACK);
                       
            // Draw the restart button rectangle
            DrawRectangleLines(CENTER("Restart?", 70) - 10, GetScreenHeight() / 2, 350, 70, BLACK);
            
            // Create the rectange bounds for the restart button
            Rectangle rectangle = {CENTER("Restart?", 70) - 10, GetScreenHeight() / 2, 350, 70};  // x y width height
            
            
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
    
    BeginDrawing();
    
        // Clear the background
        ClearBackground(RAYWHITE);
        
        // Draw the loss line
        DrawLine(0, 600, 1280, 600, RED);   
        
        // Draw the help text
        char directions[89] = "Pop the correct expressions without missing any before they all hit the red line to win!";
        
        DrawText(directions, CENTER(directions, 25), 600, 25, BLACK); 
        
        // For each popper in the game...
        for(int i = 0; i < NUM_SPRITES; i++) {
            
            // If the one we are looking at is alive...
            if (sprites[i].alive == 1) {
            
                // Draw it to the screen                
                Rectangle rectangle = {sprites[i].position.x, sprites[i].position.y, 80, 20};
                
                DrawRectangleRec(rectangle, sprites[i].color);
                
                // Draw each equation in the appropriate location
                DrawText(sprites[i].operation, sprites[i].position.x+3,  sprites[i].position.y, 20, BLACK);
              
            }
        }        
        
        bool drawn = 0; // flag to determine when the result is drawn
        // Find the first viable popper
        for(int i = 0; (i < NUM_SPRITES) && !drawn; i++) {
            // If the one we are looking at is alive...
            if (sprites[i].alive == 1) {
                
                DrawText("Pop the expression", 30, 40, 25, BLACK); 
                DrawText("that results to: ", 30, 60, 25, BLACK); 
                
                char s[4]; 
                sprintf(s,"  %d", sprites[i].result );
                DrawText(s, 210, 60, 25, RED); 
                
                drawn = 1;
                
                globalResult = sprites[i].result;
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
        
        if(CheckCollisionCircles(position, SPRITE_SIZE, sprites[i].position, SPRITE_SIZE)) {
            
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
        int pos = sprites[i].position.y;
        
        // Check to see if the y value of the point
        // falls below the loss line at y = 600
        // NOTE: Add a 20 value buffer to account
        // for circle radius
        if ((pos > 580) && sprites[i].lifeCycle >= 3){
            
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
