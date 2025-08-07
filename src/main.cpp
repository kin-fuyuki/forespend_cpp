#define GRAPHICS_API_OPENGL_33
#include"game/game.h"

map world;
mainmenu menu;

std::string NAME=		"forespend"				;
std::string VERSION=	"0.01g"					;
std::string PATH=		"../"					;
scene* current=&world;

int main(int argc, char* argv[]) {
	init();
	while (!WindowShouldClose()) {	
	current->update();
	render();
	}
	close();
	return 0;
}
