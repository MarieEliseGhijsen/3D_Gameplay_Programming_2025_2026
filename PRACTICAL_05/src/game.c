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

//8 vertices for cube
Vertex vertex[8]; //8 unique vertices
GLubyte triangles[36]; // 12 triangles x 3 vertices

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
	game->scale = 1.0f;

    //cube vertices
    //front face
	vertex[0].coordinate[0] = -0.5f;
	vertex[0].coordinate[1] = -0.5f;
	vertex[0].coordinate[2] =  0.5f;
   	vertex[0].color[0] = 1.0f; 
	vertex[0].color[1] = 0.0f;
	vertex[0].color[2] = 0.0f;

	vertex[1].coordinate[0] =  0.5f;
	vertex[1].coordinate[1] = -0.5f;
	vertex[1].coordinate[2] =  0.5f;
	vertex[1].color[0] = 0.0f; 
	vertex[1].color[1] = 1.0f; 
	vertex[1].color[2] = 0.0f;

	vertex[2].coordinate[0] =  0.5f;
	vertex[2].coordinate[1] =  0.5f;
	vertex[2].coordinate[2] =  0.5f;
	vertex[2].color[0] = 0.0f; 
	vertex[2].color[1] = 0.0f; 
	vertex[2].color[2] = 1.0f;

	vertex[3].coordinate[0] = -0.5f;
	vertex[3].coordinate[1] =  0.5f;
	vertex[3].coordinate[2] =  0.5f;
	vertex[3].color[0] = 1.0f; 
	vertex[3].color[1] = 1.0f; 
	vertex[3].color[2] = 0.0f;

    //back face
	vertex[4].coordinate[0] = -0.5f;
	vertex[4].coordinate[1] = -0.5f;
	vertex[4].coordinate[2] = -0.5f;
	vertex[4].color[0] = 1.0f; 
	vertex[4].color[1] = 0.0f; 
	vertex[4].color[2] = 1.0f;

	vertex[5].coordinate[0] =  0.5f;
	vertex[5].coordinate[1] = -0.5f;
	vertex[5].coordinate[2] = -0.5f;
	vertex[5].color[0] = 0.0f; 
	vertex[5].color[1] = 1.0f; 
	vertex[5].color[2] = 1.0f;

	vertex[6].coordinate[0] =  0.5f;
	vertex[6].coordinate[1] =  0.5f;
	vertex[6].coordinate[2] = -0.5f;
	vertex[6].color[0] = 1.0f; 
	vertex[6].color[1] = 1.0f; 
	vertex[6].color[2] = 1.0f;

	vertex[7].coordinate[0] = -0.5f;
	vertex[7].coordinate[1] =  0.5f;
	vertex[7].coordinate[2] = -0.5f;
	vertex[7].color[0] = 0.0f; 
	vertex[7].color[1] = 0.0f;
	vertex[7].color[2] = 0.0f;

    //indices for cube - 12 triangles
    //front
	triangles[0]=0; triangles[1]=1; triangles[2]=2;
	triangles[3]=0; triangles[4]=2; triangles[5]=3;

    //back
	triangles[6]=4; triangles[7]=5; triangles[8]=6;
	triangles[9]=4; triangles[10]=6; triangles[11]=7;

    //left
	triangles[12]=0; triangles[13]=3; triangles[14]=7;
	triangles[15]=0; triangles[16]=7; triangles[17]=4;

    //right
	triangles[18]=1; triangles[19]=5; triangles[20]=6;
	triangles[21]=1; triangles[22]=6; triangles[23]=2;

    //top
	triangles[24]=3; triangles[25]=2; triangles[26]=6;
	triangles[27]=3; triangles[28]=6; triangles[29]=7;

    //bottom
	triangles[30]=0; triangles[31]=1; triangles[32]=5;
	triangles[33]=0; triangles[34]=5; triangles[35]=4;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Generate and bind VBO
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Generate and bind index buffer
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
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

	//Z-axis rotation (A/S)
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    	{
        	game->rotationZ += ROTATION_SPEED * deltaTime;
    	}
    	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    	{
        	game->rotationZ -= ROTATION_SPEED * deltaTime;
    	}

	//scaling (Q/W)
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    	{
        	game->scale += 1.0f * deltaTime;
    	}
    	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    	{
        	game->scale -= 1.0f * deltaTime;
    	}

	if (game->scale < 0.1f)
	{
		game->scale = 0.1f;
	}

    // Normalize rotations fmodf is a modulus operation in math.h
    // In VBA example the rotation was clamped with a series of if statements
    game->rotationY = fmodf(game->rotationY, 360.0f);
    game->rotationX = fmodf(game->rotationX, 360.0f);
	game->rotationZ = fmodf(game->rotationZ, 360.0f);
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
    
    	//translate and rotate
    	glTranslatef(0.0f, 0.0f, -6.0f);

    	glRotatef(game->rotationX, 1.0f, 0.0f, 0.0f);
    	glRotatef(game->rotationY, 0.0f, 1.0f, 0.0f);
	glRotatef(game->rotationZ, 0.0f, 0.0f, 1.0f);

	glScalef(game->scale, game->scale, game->scale);

    // Bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

    // Enable client states
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    //set up vertex and color pointers
	glColorPointer(3, GL_FLOAT, sizeof(Vertex), (char *)NULL + 12);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (char *)NULL + 0);

    //draw the cube
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char *)NULL + 0);

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