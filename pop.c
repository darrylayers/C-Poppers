#include "raylib.h"
#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"


//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

#define NUM_MAX_POPPERS 1000
#define NUM_POPPERS 200
#define POPPER_SIZE 20

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct Sprite{
    int id;
    Vector2 position;
    Color color;
    double speed;
} Sprite;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static int screenWidth = 1280;
static int screenHeight = 720;
static Sprite poppers[NUM_MAX_POPPERS];


//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UpdateDrawFrame(void);  // Update and Draw (one frame)
static int getRandom(int bound);    // Get a random int between 0 and bound

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
    return 0;
}

//------------------------------------------------------------------------------------
// Get a random int between 0 and bound
//------------------------------------------------------------------------------------
int getRandom(int bound) 
{ 
    return (rand() % (bound));
} 

//------------------------------------------------------------------------------------
// Initialize game
//------------------------------------------------------------------------------------
void InitGame(void)
{

    for(int i = 0; i < NUM_POPPERS; i++) 
    {
        Sprite sprite;
        sprite.id = i;
        Vector2 position = {getRandom(1280), -60};
        sprite.position = position;
        if ((i % 2) == 0) {
            sprite.color = PINK;
            sprite.speed = 1.0; 
        }
        else if (i == 10) {
            sprite.color = RED;
            sprite.speed = 1.3;
        } else {
           sprite.color = DARKBLUE;
           sprite.speed = 1.8;              
        }
        poppers[i] = sprite;       
    }
}

//------------------------------------------------------------------------------------
// Update game (one frame)
//------------------------------------------------------------------------------------
void UpdateGame(void){
    
    for(int i = 0; i < NUM_POPPERS; i++) {
        
        Sprite sprite = poppers[i];
        double modifier = 1.0;
        Vector2 position = sprite.position;
        Color color = sprite.color;
        
        //Check if the ball was clicked
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
        (CheckCollisionPointCircle(GetMousePosition(), position, POPPER_SIZE))){
            
            if (ColorToInt(color) == ColorToInt(PINK)) {
                sprite.color = RED;   
                sprite.speed = 4.8;              
            } else if (ColorToInt(color) == ColorToInt(RED)){
                sprite.color = DARKBLUE;
                modifier = 1.8;                 
            } else {
                sprite.color = PINK;
                sprite.speed = 1.0;  
            }
        }        
  
        //Check to see if impacted wall
        if (sprite.position.x >= 1260 && sprite.position.y <= 700) {
            Vector2 newPos = {(sprite.position.x), (sprite.position.y + 1*(sprite.speed))};
            sprite.position = newPos; 
            poppers[i] = sprite;            
        } 
        else if (sprite.position.y >= 700) {
            Vector2 newPos = {(sprite.position.x), (sprite.position.y)};
            sprite.position = newPos; 
            poppers[i] = sprite;            
        }          
        else if ((sprite.position.x >= 1260) && (sprite.position.y >= 700)){
            Vector2 newPos = {(sprite.position.x), (sprite.position.y)};
            sprite.position = newPos; 
            poppers[i] = sprite;     
        } else {
            //If no impact, behave normally
            Vector2 newPos = {(sprite.position.x += 1), (sprite.position.y + 1*(sprite.speed))};
            sprite.position = newPos; 
            poppers[i] = sprite;
        }     
    }
}

//------------------------------------------------------------------------------------
// Draw game (one frame)
//------------------------------------------------------------------------------------
void DrawGame(void){
    BeginDrawing();
        ClearBackground(RAYWHITE);
        for(int i = 0; i < NUM_POPPERS; i++) 
        {
            Sprite sprite = poppers[i];
            DrawCircleV(sprite.position, POPPER_SIZE, sprite.color);
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

