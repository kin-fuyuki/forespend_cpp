#include "game.h"
#include<enginend/term.h>

void init() {
	startup(CSTR(NAME),CSTR(VERSION));
	window.init();
}
void close() {
	window.close();
}
void keyboard(unsigned char key,int x,int y) {
	setkey(key,true);
}
void keyboardup(unsigned char key,int x,int y) {
	setkey(key,false);
}
void mouse(int button,int state,int x,int y) {
	
}
void motion(int x,int y) {
	
}
void render() {
	glClear(GLCLEAR);
	current->update();
	current->render();
	clearchanged();
	window.draw();
}
