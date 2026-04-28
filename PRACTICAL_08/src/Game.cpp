#include <./include/Debug.h> // Include Debug header file
#include <./include/Cube.h>	 // Include Cube header file
#include <./include/Game.h>	 // Include Game header file

/* STB_IMAGE_IMPLEMENTATION should be defined only once */
#define STB_IMAGE_IMPLEMENTATION // Define STB_IMAGE_IMPLEMENTATION only once
#include <./include/stb_image.h> // Include single file header for loading images

Game::Game() : window(VideoMode(800, 600), "OpenGL Texture Mapping StarterKit")
{
}

Game::~Game() {}

/* Variable to hold the VBO identifier and shader data */

GLuint vsid,	// Vertex Shader ID
	fsid,		// Fragment Shader ID
	progID,		// Program ID
	vbo,		// Vertex Buffer ID
	vib,		// Vertex Index Buffer
	to,			// Texture ID 1 to 32
	positionID, // Position ID
	colorID,	// Color ID
	textureID,	// Texture ID
	uvID,		// UV ID
	mvpID;		// Model View Projection ID

//Filename for texture
//const string filename = "./assets/grid.tga";
//const string filename = "./assets/coordinates.tga";
//const string filename = "./assets/cube.tga";
const string filename = "./assets/grid_wip.tga";
//const string filename = "./assets/minecraft.tga";
//const string filename = "./assets/texture.tga";
//const string filename = "./assets/texture_2.tga";
//const string filename = "../assets/uvtemplate.tga";

int width;			  // width of texture
int height;			  // height of texture
int comp_count;		  // Component of texture
const int number = 4; // 4 = RGBA

// Texture image data
unsigned char *img_data;

// Model View Projection Matrices
mat4 mvp, projection, view, model;

void Game::initialise()
{
	DEBUG_MSG("\n******** Initialisation Procedure STARTS ********\n");

	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	if (glewInit())
	{
		throw runtime_error("\nGLEW Init Failed\n");
	}

	// Copy UV coordinates to all faces (initially only one face is defined in Cube.h)
	// This loop iterates over each face (except the first one) and copies the UV coordinates
	// from the first face to the UV array for that face.
	for (int i = 1; i < 6; i++)
	{
		// Calculate the starting position of UV coordinates for the current face in the UV array
		int uv_start_position = i * 4 * 2; // Each face has 4 vertices, each vertex has 2 UV coordinates

		// Copy UV coordinates from the first face to the current face
		memcpy(&uvs[uv_start_position], &uvs[0], 2 * 4 * sizeof(GLfloat)); // Each vertex has 2 UV coordinates,
																		   // and there are 4 vertices per face
	}

	// 	for (int i = 1; i < 6; i++) {
	//		memcpy(&uvs[i * 4 * 2], &uvs[0], 2 * 4 * sizeof(GLfloat));
	//	}

	// Output GPU information to the debug console
	DEBUG_MSG("\n******** GPU information STARTS ********\n");
	// Retrieve and output GPU vendor information
	DEBUG_MSG(glGetString(GL_VENDOR));
	// Retrieve and output GPU renderer information
	DEBUG_MSG(glGetString(GL_RENDERER));
	// Retrieve and output GPU version information
	DEBUG_MSG(glGetString(GL_VERSION));
	// Retrieve and output GPU shading language version information
	DEBUG_MSG(glGetString(GL_SHADING_LANGUAGE_VERSION));
	DEBUG_MSG("\n******** GPU information ENDS ********\n");

	// Generate Vertex Buffer
	glGenBuffers(1, &vbo); // Gen Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Vertices (3) x,y,z , Colours (4) RGBA, UV/ST (2)
	glBufferData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLOURS) + (2 * UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &vib); // Gen Vertex Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);

	// Indices to be drawn
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);

	DEBUG_MSG("Vertex Shader Definition");

	// Define and compile Vertex Shader
	const char *vs_src =
		"\n#version 130\n\n"
		""
		//"layout(location = 0) in vec3 sv_position; //Use for individual Buffers\n"
		//"layout(location = 1) in vec4 sv_color; //Use for individual Buffers\n"
		//"layout(location = 2) in vec2 sv_texel; //Use for individual Buffers\n"
		""
		"in vec3 sv_position;\n"
		"in vec4 sv_color;\n"
		"in vec2 sv_uv;\n\n"
		""
		"out vec4 color;\n"
		"out vec2 uv;\n\n"
		""
		"uniform mat4 sv_mvp;\n"
		""
		"void main() {\n"
		"	color = sv_color;\n"
		"	uv = sv_uv;\n"
		//"	gl_Position = vec4(sv_position, 1);"
		"	gl_Position = sv_mvp * vec4(sv_position, 1);\n"
		"}\n\n"; // Vertex Shader Src

	DEBUG_MSG("\n******** Vertex Shader src STARTS ********\n");
	DEBUG_MSG(string(vs_src));
	DEBUG_MSG("\n******** Vertex Shader src ENDS ********\n");

	DEBUG_MSG("Setting Up Vertex Shader");

	// Compile Vertex Shader
	vsid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsid, 1, (const GLchar **)&vs_src, NULL);
	glCompileShader(vsid);

	// Check if Vertex Shader is Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE)
	{
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		throw runtime_error("\nERROR: Vertex Shader Compilation Error\n");
	}

	// Define and compile Fragment Shader
	const char *fs_src =
		"\n#version 130\n\n"
		""
		"uniform sampler2D f_texture;\n"
		""
		"in vec4 color;\n"
		"in vec2 uv;\n"
		""
		"out vec4 fColor;\n"
		""
		"void main() {\n"
		"	vec4 lightColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
		//"	fColor = vec4(0.50f, 0.50f, 0.50f, 1.0f);\n"
		//"	fColor = texture2D(f_texture, uv);\n"
		//"	fColor = color * texture2D(f_texture, uv);\n"
		"	fColor = lightColor * texture2D(f_texture, uv);\n"
		//"	fColor = color + texture2D(f_texture, uv);\n"
		//"	fColor = color - texture2D(f_texture, uv);\n"
		//"	fColor = color;\n"
		"}\n\n"; // Fragment Shader Src

	DEBUG_MSG("\n******** Fragment Shader src STARTS ********\n");
	DEBUG_MSG(string(fs_src));
	DEBUG_MSG("\n******** Fragment Shader src ENDS ********\n");

	DEBUG_MSG("Setting Up Fragment Shader");

	// Compile Fragment Shader
	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar **)&fs_src, NULL);
	glCompileShader(fsid);

	// Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE)
	{
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		throw runtime_error("\nERROR: Fragment Shader Compilation Error\n");
	}

	// Create and link shader program
	DEBUG_MSG("\n******** Shader Linking STARTS ********\n");
	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();
	glAttachShader(progID, vsid);
	glAttachShader(progID, fsid);
	glLinkProgram(progID);

	// Check if Shader Program is linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1)
	{
		DEBUG_MSG("Vertex and Fragment Shader Linked");
	}
	else
	{
		throw runtime_error("\nERROR: Vertex and Fragment Shader Link Error\n");
	}
	DEBUG_MSG("\n******** Shader Linking ENDS ********\n");
	// Use Shader Program on GPU
	glUseProgram(progID);

	// Load texture image data
	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		throw runtime_error("\nERROR: Texture not loaded " + filename + "\n");
	}

	// Enable 2D texturing
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to);
	glBindTexture(GL_TEXTURE_2D, to);

	// Texture wrapping
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Texture filtering
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Transfer texture image data to GPU
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(
		GL_TEXTURE_2D,	  // 2D Texture Image
		0,				  // Mipmapping Level
		GL_RGBA,		  // GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA
		width,			  // Width
		height,			  // Height
		0,				  // Border
		GL_RGBA,		  // Bitmap
		GL_UNSIGNED_BYTE, // Specifies Data type of image data
		img_data		  // Image Data
	);

	// Find attribute and uniform locations in shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
	uvID = glGetAttribLocation(progID, "sv_uv");
	textureID = glGetUniformLocation(progID, "f_texture");
	mvpID = glGetUniformLocation(progID, "sv_mvp");

	// Set up Projection Matrix
	projection = perspective(
		45.0f,		 // Field of View 45 degrees
		4.0f / 3.0f, // Aspect ratio: 4:3
		5.0f,		 // Display Range Min : 0.1f unit
		100.0f		 // Display Range Max : 100.0f unit
	);

	// Set up Camera Matrix
	view = lookAt(
		vec3(0.0f, 4.0f, 10.0f), // Camera (x,y,z), in World Space
		vec3(0.0f, 0.0f, 0.0f),	 // Camera looking at origin
		vec3(0.0f, 1.0f, 0.0f)	 // 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
	);

	// Initialize Model matrix as Identity Matrix
	model = mat4(
		1.0f // Identity Matrix
	);

	// Enable Depth Test for accurate rendering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	DEBUG_MSG("\n******** Initialisation Procedure ENDS ********\n");
}

void Game::run()
{
	// Initialize the game
	initialise();

	// Create an event object for handling window events
	Event event;

	// Main game loop
	while (isRunning)
	{
		// Check if the game is running in debug mode and print debug message
#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		// Handle events such as window close or keyboard input
		while (window.pollEvent(event))
		{
			// Check if the window is being closed
			if (event.type == Event::Closed)
			{
				// Set the flag to stop the game loop
				isRunning = false;
			}
			// Check for keyboard input for model rotation
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				// Rotate the model to the left around the y-axis
				model = rotate(model, 0.1f, glm::vec3(0, 1, 0)); // Rotate
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				// Rotate the model to the right around the y-axis
				model = rotate(model, -0.1f, glm::vec3(0, 1, 0)); // Rotate
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				// Rotate the model upwards around the x-axis
				model = rotate(model, -0.1f, glm::vec3(1, 0, 0)); // Rotate
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				// Rotate the model downwards around the x-axis
				model = rotate(model, 0.1f, glm::vec3(1, 0, 0)); // Rotate
			}
		}

		// Update game state
		update();

		// Render the game scene
		render();
	}
}

void Game::update()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Updating... MVP");
#endif
	// Update the Model View Projection matrix by combining the projection, view, and model matrices
	mvp = projection * view * model;

#if (DEBUG >= 2)
	DEBUG_MSG("Finished Updating... MVP");
#endif
}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Render Loop...");
#endif

	// Clear the color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update vertex buffer object (VBO) data with vertices, colours, and UVs
	glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * VERTICES * sizeof(GLfloat), 4 * COLOURS * sizeof(GLfloat), colours);
	glBufferSubData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLOURS)) * sizeof(GLfloat), 2 * UVS * sizeof(GLfloat), uvs);

	// Send the transformation to the shader's MVP uniform
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	// Set the active texture unit to 0 ..... 32
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);

	// Set pointers for each parameter (with appropriate starting positions)
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (void *)(3 * VERTICES * sizeof(GLfloat)));
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (void *)(((3 * VERTICES) + (4 * COLOURS)) * sizeof(GLfloat)));

	// Enable vertex attribute arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(uvID);

	// Draw the elements using vertex arrays
	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);

	// Display the rendered frame
	window.display();

	// Disable vertex attribute arrays
	glDisableVertexAttribArray(positionID);
	glDisableVertexAttribArray(colorID);
	glDisableVertexAttribArray(uvID);
}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	// Delete the shader program
	glDeleteProgram(progID);

	// Delete the vertex buffer
	glDeleteBuffers(1, &vbo);

	// Delete the vertex index buffer
	glDeleteBuffers(1, &vib);

	// Free the loaded image data
	stbi_image_free(img_data);
}
