#ifndef GAME_H
#define GAME_H

#ifdef _WIN32
#define CALLBACK __stdcall
#endif

#include <GL/glew.h>    // GLEW for OpenGL extensions
#include <GLFW/glfw3.h> // GLFW for window management and OpenGL context
#include <GL/glu.h>     // OpenGL Utility Library for perspective projection
#include <stdio.h>      // Standard I/O operations
#include <stdlib.h>     // Standard library functions
#include <stdbool.h>    // Boolean type support
#include <math.h>       // For clamping values

// Game state structure to maintain all necessary game data
typedef struct Game
{
    GLFWwindow *window; // Pointer to GLFW window
    bool isRunning;     // Game running state flag
    double lastTime;    // Time tracking for animation
    float rotationY;    // Current rotation angle of the model
    float rotationX;    // Current rotation angle of the model
    float rotationZ;    // Current rotation angle of the model
	float scale;
} Game;

// Function prototypes for game lifecycle management
void initialize(Game *game);                      // Initialize game state and OpenGL
void handleInput(GLFWwindow *window, Game *game); // Handle player input
void update(Game *game);                          // Update game logic
void draw(Game *game);                            // Render the scene
void run(Game *game);                             // Main game loop
void destroy(Game *game);                         // Cleanup resources

#endif // GAME_H