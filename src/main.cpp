#include"game/game.h"

Window* window;
map* world;
bool running = true;

int main(int argc, char* argv[]) {
// main is only really useful to pass arguments
	init();
	loop();
	close();

	return 0;
}
