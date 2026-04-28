#include <iostream>

#include <GL/glew.h>

#include <./include/stb_image.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "./include/vector3f.h"
#include "./include/matrix3f.h"
#include "./include/quaternion.h"

using namespace std;
using namespace sf;

#define M_PI 3.14159

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	Window window;
	bool isRunning = false;
	void initialize();
	void update();
	void render();
	void unload();

	Clock clock;
	Time elapsed;
};