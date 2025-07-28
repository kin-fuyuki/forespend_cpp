#include"game/game.h"

Window* window;
map* world;

int main(int argc, char* argv[]) {
// main is only really useful to pass arguments
	init();
	loop();
	close();

	return 0;
}
