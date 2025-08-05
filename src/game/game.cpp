#include "game.h"
#include "../term.h"

void init() {
	startup(CSTR(NAME),CSTR(VERSION));
	window.init();
}
void close() {
	window.close();
}
void special(int key,int x,int y) {
	setkey(key,true);
	echo ("key %i",key);
	
}
void specialup(int key,int x,int y) {
	setkey(key,false);
	
}

void keyboard(unsigned char key,int x,int y) {
	echo ("key %i",key);
	setkey(key,true);
}
void keyboardup(unsigned char key,int x,int y) {
	setkey(key,false);
}
void mouse(int button,int state,int x,int y) {
	
}
void motion(int x,int y) {
	
}
// measure time
unsigned long now=0;
unsigned long prv=0;
unsigned long delta=0;
short updatems=16;
void timer(int value) {
	//get cycles
	now=__rdtsc();
	delta=now-prv;
	glutPostRedisplay();
	glutTimerFunc(updatems,timer,0);
	prv=now;
}
void render() {
	glClear(GLCLEAR);
	if (pressed(4) && pressed(116)){
		current->close();
		window.close();
		
		exit(0);}

	current->update();
	current->render();
	clearchanged();
	window.draw();

    glutSwapBuffers();
}
