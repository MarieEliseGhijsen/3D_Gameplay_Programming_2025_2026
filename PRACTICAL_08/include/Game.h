#include <iostream>
#include <stdexcept>

#include <cstring>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


using namespace std;
using namespace sf;
using namespace glm;

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	Window window;
	bool isRunning = false;
	void initialise();
	void update();
	void render();
	void unload();

	Clock clock;
	Time elapsed;
};