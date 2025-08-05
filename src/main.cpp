#include"game/game.h"

map world;
mainmenu menu;
Window window;

std::string NAME=		"forespend"				;
std::string VERSION=	"0.01g"					;
scene* current=&menu;

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	init();
	close();
	return 0;
}
