#include <./include/Debug.h>
#include <./include/Game.h>

#include <fstream>
#include <sstream>

Game::Game() : window(VideoMode(800, 600), "OpenGL Cube Vertex and Fragment Shaders")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning)
	{

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}
}

typedef struct
{
	float coordinate[3];
	float color[3];
} Vertex;

Vertex vertex[8];
GLubyte triangles[36];

/* Variable to hold the VBO identifier and shader data */

GLuint index,	// Index to draw
	vsid,		// Vertex Shader ID
	fsid,		// Fragment Shader ID
	progID,		// Program ID
	vbo = 1,	// Vertex Buffer ID
	positionID, // Position ID
	colorID;	// Color ID

std::string loadShaderFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream buffer;

    if (!file.is_open())
    {
        std::cout << "ERROR: Cannot open shader file: " << path << std::endl;
        return "";
    }

    buffer << file.rdbuf();

    return buffer.str();
}

void Game::initialize()
{
	isRunning = true;

	GLint isCompiled = 0;
	GLint isLinked = 0;

	DEBUG_MSG("INIT Glew");

	glewInit();
	glEnable(GL_DEPTH_TEST);

	/* Vertices counter-clockwise winding */

	DEBUG_MSG("Setup Vertices");

	vertex[0].coordinate[0] = -0.5f;
	vertex[0].coordinate[1] = -0.5f;
	vertex[0].coordinate[2] = -0.5f;

	vertex[1].coordinate[0] =  0.5f;
	vertex[1].coordinate[1] = -0.5f;
	vertex[1].coordinate[2] = -0.5f;
	
	vertex[2].coordinate[0] =  0.5f;
	vertex[2].coordinate[1] =  0.5f;
	vertex[2].coordinate[2] = -0.5f;

	vertex[3].coordinate[0] = -0.5f;
	vertex[3].coordinate[1] =  0.5f;
	vertex[3].coordinate[2] = -0.5f;

	vertex[4].coordinate[0] = -0.5f;
	vertex[4].coordinate[1] = -0.5f;
	vertex[4].coordinate[2] =  0.5f;

	vertex[5].coordinate[0] =  0.5f;
	vertex[5].coordinate[1] = -0.5f;
	vertex[5].coordinate[2] =  0.5f;

	vertex[6].coordinate[0] =  0.5f;
	vertex[6].coordinate[1] =  0.5f;
	vertex[6].coordinate[2] =  0.5f;

	vertex[7].coordinate[0] = -0.5f;
	vertex[7].coordinate[1] =  0.5f;	vertex[7].coordinate[2] =  0.5f;

	/* Colors counter-clockwise winding */

	DEBUG_MSG("Setup Colors");

	vertex[0].color[0] = 0.1f;
	vertex[0].color[1] = 1.0f;
	vertex[0].color[2] = 0.0f;

	vertex[1].color[0] = 0.2f;
	vertex[1].color[1] = 1.0f;
	vertex[1].color[2] = 0.0f;

	vertex[2].color[0] = 0.3f;
	vertex[2].color[1] = 1.0f;
	vertex[2].color[2] = 0.0f;

	vertex[3].color[0] = 0.4f;
	vertex[3].color[1] = 1.0f;
	vertex[3].color[2] = 0.0f;

	vertex[4].color[0] = 0.5f;
	vertex[4].color[1] = 1.0f;
	vertex[4].color[2] = 0.0f;

	vertex[5].color[0] = 0.6f;
	vertex[5].color[1] = 1.0f;
	vertex[5].color[2] = 0.0f;

	vertex[6].color[0] = 0.7f;
	vertex[6].color[1] = 1.0f;
	vertex[6].color[2] = 0.0f;

	vertex[7].color[0] = 0.8f;
	vertex[7].color[1] = 1.0f;
	vertex[7].color[2] = 0.0f;

	/* Vertex Indexes */

	DEBUG_MSG("Setup Indexes");

	//front
	triangles[0]=0; 
	triangles[1]=1; 
	triangles[2]=2;

	triangles[3]=0; 
	triangles[4]=2; 
	triangles[5]=3;

	//back
	triangles[6]=5; 
	triangles[7]=4; 
	triangles[8]=7;

	triangles[9]=5; 
	triangles[10]=7; 
	triangles[11]=6;

	//left
	triangles[12]=4; 
	triangles[13]=0; 
	triangles[14]=3;

	triangles[15]=4; 
	triangles[16]=3; 
	triangles[17]=7;

	//right
	triangles[18]=1; 
	triangles[19]=5; 
	triangles[20]=6;

	triangles[21]=1; 
	triangles[22]=6; 
	triangles[23]=2;

	//top
	triangles[24]=3; 
	triangles[25]=2; 
	triangles[26]=6;

	triangles[27]=3;
	triangles[28]=6; 
	triangles[29]=7;

	//bottom
	triangles[30]=4;
	triangles[31]=5;
	triangles[32]=1;

	triangles[33]=4;
	triangles[34]=1;
	triangles[35]=0;

	DEBUG_MSG("VBO Steps");

	/* Create a new VBO using VBO id */
	glGenBuffers(1, &vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	DEBUG_MSG("Starting Shaders");

	/* Vertex Shader which would normally be loaded from an external file */
	//const char *vs_src = "#version 400\n\r"
	//					 "in vec4 sv_position;"
	//					 "in vec4 sv_color;"
	//					 "out vec4 color;"
	//					 "void main() {"
	//					 "	color = sv_color;"
	//					 "	gl_Position = sv_position;"
	//					 "}"; // Vertex Shader Src

	DEBUG_MSG("Loading shaders from files");
	
	std::string vsCode = loadShaderFile("./shaders/vertex.glsl");
	std::string fsCode = loadShaderFile("./shaders/fragment.glsl");
	
	if (vsCode.empty() || fsCode.empty())
	{
    		std::cout << "Shader load failed!" << std::endl;
    		return;
	}
	
	const char* vs_src = vsCode.c_str();
	const char* fs_src = fsCode.c_str();
	
	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER);				 // Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar **)&vs_src, NULL); // Set the shaders source
	glCompileShader(vsid);									 // Check that the shader compiles

	// Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE)
	{
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	/* Fragment Shader which would normally be loaded from an external file */
	//const char *fs_src = "#version 400\n\r"
	//					 "in vec4 color;"
	//					 "out vec4 fColor;"
	//					 "void main() {"
	//					 "	fColor = color + vec4(1.0f, 0.0f, 0.0f, 1.0f);"
	//					 "}"; // Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

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
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	  // Create program in GPU
	glAttachShader(progID, vsid); // Attach Vertex Shader to Program
	glAttachShader(progID, fsid); // Attach Fragment Shader to Program
	glLinkProgram(progID);

	// Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1)
	{
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	// Find variables in the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
}

// Initialize a vector with given values
void initVector3f(Vector3f_ *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

// Initialize a matrix with values
void initMatrix3fWithValues(Matrix3f *m, float A11, float A12, float A13,
                             float A21, float A22, float A23,
                             float A31, float A32, float A33) {
    m->A11 = A11; m->A12 = A12; m->A13 = A13;
    m->A21 = A21; m->A22 = A22; m->A23 = A23;
    m->A31 = A31; m->A32 = A32; m->A33 = A33;
}

// Rotate matrix around Z-axis
Matrix3f rotateZ(float angle) {
    float radians = angle * (M_PI / 180.0f);
    Matrix3f result;
    initMatrix3fWithValues(&result,
        cosf(radians), -sinf(radians), 0.0f,
        sinf(radians), cosf(radians), 0.0f,
        0.0f, 0.0f, 1.0f
    );
    return result;
}

// Rotate matrix around Y-axis
Matrix3f rotateY(float angle) {
    float radians = angle * (M_PI / 180.0f);
    Matrix3f result;
    initMatrix3fWithValues(&result,
        cosf(radians), 0.0f, -sinf(radians),
        0.0f, 1.0f, 0.0f,
        sinf(radians), 0.0f, cosf(radians)
    );
    return result;
}

// Matrix multiplication with a Vector3f_
Vector3f_ multiplyMatrix3fByVector3f(const Matrix3f *m, const Vector3f_ *v) {
    return (Vector3f_){
        m->A11 * v->x + m->A12 * v->y + m->A13 * v->z,
        m->A21 * v->x + m->A22 * v->y + m->A23 * v->z,
        m->A31 * v->x + m->A32 * v->y + m->A33 * v->z
    };
}

void Game::update()
{
	elapsed = clock.getElapsedTime();
	
	Matrix3f m;
	m = rotateZ(0.1f);
	
	for(int i = 0; i < 8; i++)
	{
		Vector3f_ v;
		
		initVector3f (&v, vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2]);
		
		v = multiplyMatrix3fByVector3f(&m, &v);
		
		vertex[i].coordinate[0] = v.x;
		vertex[i].coordinate[1] = v.y;
		vertex[i].coordinate[2] = v.z;
	}
	
	Matrix3f mY;
	mY = rotateY(0.1f);
	
	for(int i = 0; i < 8; i++)
	{
		Vector3f_ v;
		
		initVector3f (&v, vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2]);
		
		v = multiplyMatrix3fByVector3f(&mY, &v);
		
		vertex[i].coordinate[0] = v.x;
		vertex[i].coordinate[1] = v.y;
		vertex[i].coordinate[2] = v.z;
	}

	//// Change vertex data
	//vertex[0].coordinate[0] += -0.0001f;
	//vertex[0].coordinate[1] += -0.0001f;
	//vertex[0].coordinate[2] += -0.0001f;
	//
	//vertex[1].coordinate[0] += -0.0001f;
	//vertex[1].coordinate[1] += -0.0001f;
	//vertex[1].coordinate[2] += -0.0001f;
	//
	//vertex[2].coordinate[0] += -0.0001f;
	//vertex[2].coordinate[1] += -0.0001f;
	//vertex[2].coordinate[2] += -0.0001f;
	//
	//vertex[3].coordinate[0] += -0.0001f;
	//vertex[3].coordinate[1] += -0.0001f;
	//vertex[3].coordinate[2] += -0.0001f;
	//
	//vertex[4].coordinate[0] += -0.0001f;
	//vertex[4].coordinate[1] += -0.0001f;
	//vertex[4].coordinate[2] += -0.0001f;
	//
	//vertex[5].coordinate[0] += -0.0001f;
	//vertex[5].coordinate[1] += -0.0001f;
	//vertex[5].coordinate[2] += -0.0001f;
	//
	//vertex[6].coordinate[0] += -0.0001f;
	//vertex[6].coordinate[1] += -0.0001f;
	//vertex[6].coordinate[2] += -0.0001f;
	//
	//vertex[7].coordinate[0] += -0.0001f;
	//vertex[7].coordinate[1] += -0.0001f;
	//vertex[7].coordinate[2] += -0.0001f;

#if (DEBUG >= 2)
	DEBUG_MSG("Update up...");
#endif
}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Drawing...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, vertex, GL_STATIC_DRAW);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain
		model components that 'are' and 'are not' to be drawn )	*/

	// Set pointers for each parameter
	// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (float *)NULL + 3);
	glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (float *)NULL + 3);


	// Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char *)NULL + 0);

	window.display();
}

void Game::unload()
{
	cout << "Cleaning up" << endl;

#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);
	glDeleteBuffers(1, &vbo);
}