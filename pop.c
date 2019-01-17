#include "raylib.h"
#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"


//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

#define NUM_MAX_POPPERS 1000
#define NUM_POPPERS 10
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
static Sprite poppers[NUM_MAX_POPPERS];
static int xPos[NUM_MAX_POPPERS];           // used to make sure sprites do not spawn in same spot
static int clicks = 0;


//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

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
// Initialize game
//------------------------------------------------------------------------------------
void InitGame(void)
{

    for(int i = 0; i < NUM_POPPERS; i++) 
    {
        Sprite sprite;
        sprite.id = i;
        sprite.alive = 1;
        int rand = GetRandomValue(0, 1280);
        
        
        Vector2 position = {rand, -60};  
        sprite.position = position;
        sprite.color = RED;
        sprite.speed = 1.3;
        poppers[i] = sprite;

        printf("Created sprite %d \n", sprite.id);
        fflush(NULL);
        
    }
}

//------------------------------------------------------------------------------------
// Update game (one frame)
//------------------------------------------------------------------------------------
void UpdateGame(void){
    
    for(int i = 0; i < NUM_POPPERS; i++) {
        
        Vector2 position = poppers[i].position;

        //Check if the ball was clicked
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
        (CheckCollisionPointCircle(GetMousePosition(), position, POPPER_SIZE))){    
            
            poppers[i].alive = 0;
            clicks++;
        }  
        else {      

            //Check to see if impacted wall
            if (poppers[i].position.x >= 1260 && poppers[i].position.y <= 700) {
                Vector2 newPos = {(poppers[i].position.x), (poppers[i].position.y + poppers[i].speed)};
                poppers[i].position = newPos;           
            } 
            else if (poppers[i].position.y >= 700) {
                Vector2 newPos = {(poppers[i].position.x), (poppers[i].position.y)};
                poppers[i].position = newPos;        
                }          
            else if ((poppers[i].position.x >= 1260) && (poppers[i].position.y >= 700)){
                Vector2 newPos = {(poppers[i].position.x), (poppers[i].position.y)};
                poppers[i].position = newPos;  
            } else {
                //If no impact, behave normally
                Vector2 newPos = {(poppers[i].position.x), (poppers[i].position.y + 1*(poppers[i].speed))};
                poppers[i].position = newPos;
            }
        
        }
        char score[3];
        sprintf(score, "%d", clicks);
        DrawText("SCORE: ", 0, 0, 25, BLACK); 
        DrawText(score, 100, 0, 25, BLACK);

        if (clicks >= NUM_POPPERS) {
            DrawText("YOU WIN!", 640,  360, 70, BLACK);
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
            //printf("%d \n", sprite.alive);
            //fflush(NULL);
            if (sprite.alive == 1) {
              DrawCircleV(sprite.position, POPPER_SIZE, sprite.color);              
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
