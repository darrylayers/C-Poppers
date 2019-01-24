#ifndef _POP_H
#define _POP_H 1

static void InitGame(bool init);                     // Initialize game
static void UpdateGame(void);                        // Update game (one frame)
static void DrawGame(void);                          // Draw game (one frame)
static void UpdateDrawFrame(void);                   // Update and Draw (one frame)
static Vector2 verifyNoCollision(Vector2 position);  // Check new points for collisions with old ones
static bool checkGameOver(void);                     // Check to see game settings are set to end the game

#endif 