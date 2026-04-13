#include "./include/game.h"

// Global flag to control update state
bool updatable = false;

// Global constants
const int SCREEN_WIDTH = 800;       // Screen Width
const int SCREEN_HEIGHT = 600;      // Screen Height
const float ROTATION_SPEED = 45.0f; // Degrees per second

typedef struct
{
    float coordinate[3];
    float color[3];
} Vertex;

/* Global variables */
GLuint vbo[1];
GLuint index;

// 5 vertices (Apex and Base of Pyramid)
Vertex vertex[5];
GLubyte triangles[18];

void initialize(Game *game)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Create window
    game->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL VBO using GLFW", NULL, NULL);
    if (!game->window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make OpenGL context current
    glfwMakeContextCurrent(game->window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwDestroyWindow(game->window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Initialize game state
    game->isRunning = true;
    game->lastTime = glfwGetTime();
    game->rotationX = 0.0f;
    game->rotationY = 0.0f;
    game->rotationZ = 0.0f;

    // Pyramid vertices
    // Base vertices
    vertex[0].coordinate[0] = -1.0f;
    vertex[0].coordinate[1] = -1.0f;
    vertex[0].coordinate[2] = -5.0f;
    vertex[0].color[0] = 0.0f;
    vertex[0].color[1] = 1.0f;
    vertex[0].color[2] = 0.0f;

    vertex[1].coordinate[0] = 1.0f;
    vertex[1].coordinate[1] = -1.0f;
    vertex[1].coordinate[2] = -5.0f;
    vertex[1].color[0] = 0.2f;
    vertex[1].color[1] = 0.8f;
    vertex[1].color[2] = 0.2f;

    vertex[2].coordinate[0] = 1.0f;
    vertex[2].coordinate[1] = -1.0f;
    vertex[2].coordinate[2] = -3.0f;
    vertex[2].color[0] = 0.0f;
    vertex[2].color[1] = 0.5f;
    vertex[2].color[2] = 0.0f;

    vertex[3].coordinate[0] = -1.0f;
    vertex[3].coordinate[1] = -1.0f;
    vertex[3].coordinate[2] = -3.0f;
    vertex[3].color[0] = 0.0f;
    vertex[3].color[1] = 0.3f;
    vertex[3].color[2] = 0.0f;

    // Apex vertex
    vertex[4].coordinate[0] = 0.0f;
    vertex[4].coordinate[1] = 1.0f;
    vertex[4].coordinate[2] = -4.0f;
    vertex[4].color[0] = 1.0f;
    vertex[4].color[1] = 0.0f;
    vertex[4].color[2] = 0.0f;

    // Indices for pyramid triangles
    // Base triangles
    triangles[0] = 0; triangles[1] = 1; triangles[2] = 2;
    triangles[3] = 0; triangles[4] = 2; triangles[5] = 3;
    
    // Side triangles
    triangles[6] = 0; triangles[7] = 1; triangles[8] = 4;
    triangles[9] = 1; triangles[10] = 2; triangles[11] = 4;
    triangles[12] = 2; triangles[13] = 3; triangles[14] = 4;
    triangles[15] = 3; triangles[16] = 0; triangles[17] = 4;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Generate and bind VBO
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 5, vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Generate and bind index buffer
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 18, triangles, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void handleInput(GLFWwindow *window, Game *game)
{
    // Check if window should close
    if (glfwWindowShouldClose(window))
    {
        game->isRunning = false;
        return;
    }

    // Get current time and calculate time elapsed
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - game->lastTime;

    // Y-axis rotation (Left/Right arrows)
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        game->rotationY += ROTATION_SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        game->rotationY -= ROTATION_SPEED * deltaTime;
    }

    // X-axis rotation (Up/Down arrows)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        game->rotationX += ROTATION_SPEED * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        game->rotationX -= ROTATION_SPEED * deltaTime;
    }

    // Normalize rotations fmodf is a modulus operation in math.h
    // In VBA example the rotation was clamped with a series of if statements
    game->rotationY = fmodf(game->rotationY, 360.0f);
    game->rotationX = fmodf(game->rotationX, 360.0f);
}

void update(Game *game)
{
    // Get current time
    double currentTime = glfwGetTime();

    // Update last time
    game->lastTime = currentTime;

    // Optional: Periodic logging of rotation
    static double lastLogTime = 0.0;
    if (currentTime - lastLogTime >= 1.0)
    {
        printf("Update : rotationX = %.2f rotationY = %.2f\n", game->rotationX, game->rotationY);
        lastLogTime = currentTime;
    }
}

void draw(Game *game)
{
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Translate and rotate
    glTranslatef(0.0f, 0.0f, -6.0f);
    glRotatef(game->rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(game->rotationY, 0.0f, 1.0f, 0.0f);

    // Bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

    // Enable client states
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Set up vertex and color pointers
    glColorPointer(3, GL_FLOAT, sizeof(Vertex), (char *)NULL + 12);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (char *)NULL + 0);

    // Draw the pyramid
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, (char *)NULL + 0);

    // Disable client states
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    // Swap buffers and poll events
    glfwSwapBuffers(game->window);
    glfwPollEvents();
}

void destroy(Game *game)
{
    printf("Cleaning up...\n");
    
    // Delete buffers
    glDeleteBuffers(1, vbo);
    glDeleteBuffers(1, &index);

    // Destroy window and terminate GLFW
    glfwDestroyWindow(game->window);
    glfwTerminate();
}

void run(Game *game)
{
    // Initialize game
    initialize(game);

    // Main game loop
    while (game->isRunning)
    {
        handleInput(game->window, game);
        update(game);
        draw(game);
    }

    // Cleanup
    destroy(game);
}