#include "game.h"
#include "../term.h"

void init() {
	startup(CSTR(NAME),CSTR(VERSION));
	
	SetConfigFlags(FLAG_VSYNC_HINT|FLAG_WINDOW_RESIZABLE);
	InitWindow(STARTINGRESX,STARTINGRESY,NAME.c_str());
	//start raylib window and stuff
}
void close() {
	CloseWindow();
}

/*void keyboard(unsigned char key,int x,int y) {
	echo ("key %i",key);
	setkey(key,true);
}
void keyboardup(unsigned char key,int x,int y) {
	setkey(key,false);
}*/
void mouse(int button,int state,int x,int y) {
	
}

// measure time
unsigned long now=0;
unsigned long prv=0;
unsigned long delta=0;
short updatems=1;
void timer(int value) {
	//get cycles
	now=__rdtsc();
	delta=now-prv;
	echo("delta %i",delta);
	prv=now;
}
Color clra={255,0,0,255};
short col=1;
void updateclra();
void render() {
	updateclra();

	if (IsKeyDown(KEY_LEFT_ALT)&& IsKeyDown(KEY_F4)
		//pressed(4) && pressed(116)
){
		current->close();
		exit(0);}
	
	BeginDrawing();
	ClearBackground(clra);
	current->render();
	EndDrawing();
	clearchanged();
}

void updateclra() {
	switch (col)
	{
	case 0:
		if (clra.r == 255) {
			col = 1;
			clra.b = 0;
		} else {
			clra.r++;
			if (clra.b != 0) clra.b--;
		}
		break;

	case 1:
		if (clra.g == 255) {
			col = 2;
			clra.r = 0;
		} else {
			clra.g++;
			if (clra.r != 0) clra.r--;
		}
		break;

	case 2:
		if (clra.b == 255) {
			col = 0;
			clra.g = 0;
		} else {
			clra.b++;
			if (clra.g != 0) clra.g--;
		}
		break;

	default:
		break;
	}
}