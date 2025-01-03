#include <./include/game.h>

// Global flag to control update state
bool updatable = false;

/**
 * Initializes the game state and OpenGL settings
 * Sets up projection matrix, creates display lists, and initializes timing
 */
void initialize(Game *game)
{
    // Set initial game state
    game->isRunning = 1;

    // Set background color to black (R=0, G=0, B=0, A=0)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Setup perspective projection matrix
    glMatrixMode(GL_PROJECTION); // Switch to projection matrix mode
    glLoadIdentity();            // Reset projection matrix

    // Set up perspective: 45 Degrees field of view, 4:3 aspect ratio, near=1.0, far=500.0
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW); // Switch back to modelview matrix

    // Create a new display list for the cube
    game->index = glGenLists(1);        // Generate one display list
    glNewList(game->index, GL_COMPILE); // Start recording display list

    // Begin defining cube geometry using quads
    glBegin(GL_QUADS);
    {
        // Front face of cube (Blue)
        glColor3f(0.0f, 0.0f, 1.0f);     // Set color to blue
        glVertex3f(1.0f, 1.0f, -5.0f);   // Top-right vertex
        glVertex3f(-1.0f, 1.0f, -5.0f);  // Top-left vertex
        glVertex3f(-1.0f, -1.0f, -5.0f); // Bottom-left vertex
        glVertex3f(1.0f, -1.0f, -5.0f);  // Bottom-right vertex

        // Back face of cube (Green)
        glColor3f(0.0f, 1.0f, 0.0f);      // Set color to green
        glVertex3f(1.0f, 1.0f, -15.0f);   // Top-right vertex
        glVertex3f(-1.0f, 1.0f, -15.0f);  // Top-left vertex
        glVertex3f(-1.0f, -1.0f, -15.0f); // Bottom-left vertex
        glVertex3f(1.0f, -1.0f, -15.0f);  // Bottom-right vertex

        // TODO: Add remaining faces to complete the cube
    }
    glEnd();     // End geometry definition
    glEndList(); // End display list compilation

    // Initialize timing for animation
    game->lastTime = glfwGetTime();
}

/**
 * Updates game logic and animation
 * Handles rotation timing and angle calculations
 */
void update(Game *game)
{
    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - game->lastTime;

    // Update rotation continuously using deltaTime
    game->rotationAngle += 45.0f * deltaTime; // 45 degrees per second

    // Keep rotation angle between 0 and 360 degrees
    if (game->rotationAngle > 360.0f)
    {
        game->rotationAngle -= 360.0f;
    }

    // Update last time
    game->lastTime = currentTime;

    static double lastLogTime = 0.0;
    if (currentTime - lastLogTime >= 1.0)
    {
        printf("Update : rotationAngle = %.2f\n", game->rotationAngle);
        lastLogTime = currentTime;
    }
}

/**
 * Renders the scene
 * Clears buffers, applies transformations, and draws the cube
 */
void draw(Game *game)
{
    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();

    // Clear color and depth buffers for new frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update last time
    game->lastTime = currentTime;

    static double lastLogTime = 0.0;
    if (currentTime - lastLogTime >= 1.0)
    {
        printf("Drawing Cube\n");
        lastLogTime = currentTime;
    }

    glLoadIdentity();                        // Reset modelview matrix
    glRotatef(game->rotationAngle, 0, 1, 1); // Apply rotation around Y and Z axis
    glCallList(game->index);                 // Draw cube using display list

    // Swap front and back buffers to display the rendered frame
    glfwSwapBuffers(game->window);
}

/**
 * Main game loop
 * Initializes GLFW, creates window, and runs the game loop
 */
void run(Game *game)
{
    // Initialize GLFW library
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Create a windowed mode window and its OpenGL context
    game->window = glfwCreateWindow(800, 600, "OpenGL Cube", NULL, NULL);
    if (!game->window)
    {
        glfwTerminate();
        printf("Failed to create GLFW window\n");
        exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(game->window);

    // Initialize game state and OpenGL settings
    initialize(game);

    // Main game loop
    while (!glfwWindowShouldClose(game->window))
    {
        update(game);     // Update game logic
        draw(game);       // Render frame
        glfwPollEvents(); // Process window events
    }

    // Cleanup resources
    destroy(game);
    glfwDestroyWindow(game->window);
    glfwTerminate();
}

/**
 * Cleanup function
 * Releases allocated resources
 */
void destroy(Game *game)
{
    printf("Cleaning up\n");
    glDeleteLists(game->index, 1); // Delete the display list
}