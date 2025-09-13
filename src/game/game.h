#include"world/world.h"
#include"world/mainmenu.h"
#include"io.h"
extern bool 	running	;
extern map 		world	;
extern mainmenu menum	;
extern scene* 	current	;
extern short	updatems;
void init();
void close();
void timer(int value);
void mouse(int button,int state,int x,int y);
void render();