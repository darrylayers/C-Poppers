#include "raylib.h"
#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"


//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

#define NUM_MAX_POPPERS 1000
#define NUM_POPPERS 500

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
    int one = 0;
    int two = -60;
    for(int i = 0; i < NUM_POPPERS; i++) 
    {
        Sprite sprite;
        sprite.id = i;
        
        //Vector2 position = {getRandom(1000), -60};

        
        if ((i % 6) == 0) {
            for(int j = 0; j < 150; j++) {
                one++;
            }
        }
        else if ((i % 3) == 0) {
            for(int j = 0; j < 150; j++) {
                one++;
            }
        }
        else if ((i % 2) == 0) {
            for(int j = 0; j < 10; j++) {
                one++;
            }
           // two = 328;
        } else {
            one = getRandom(1000);
           // two = getRandom(1000);            
        }      
        
        Vector2 position = {one, two};
        sprite.position = position;
        if ((i % 3) == 0) {
            sprite.color = PINK;
            sprite.speed = 13.8; 
        }
        else if (i == 10) {
            sprite.color = RED;
            sprite.speed = 20.0;
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
            (CheckCollisionPointCircle(GetMousePosition(),
            position, 40))){
                

            
            if (ColorToInt(color) == ColorToInt(PINK)) {
                sprite.color = RED;   
                modifier = 20.0;                
            } else if (ColorToInt(color) == ColorToInt(RED)){
                sprite.color = DARKBLUE;
                modifier = 1.8;                 
            } else {
                sprite.color = PINK;
                modifier = 13.8;              
            }
        }        
        
        
         
        
        
        Vector2 ballPosition5 = {(sprite.position.x), (sprite.position.y + 1*(modifier)) };
        sprite.position = ballPosition5; 
        poppers[i] = sprite;
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
            DrawCircleV(sprite.position, 40, sprite.color);
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

