#include <./include/Game.h>

int main(void)
{
	try
	{
		Game game;
		game.run();
	}
	catch (const exception &e)
	{
		cerr << "Exception: " << e.what() << endl;
	}
	return 0;
}