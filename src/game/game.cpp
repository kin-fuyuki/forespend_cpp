#include "game.h"
#include "../term.h"

void init() {
	startup(CSTR(NAME),CSTR(VERSION));
	cfg.load();
	SetConfigFlags(FLAG_VSYNC_HINT|FLAG_WINDOW_RESIZABLE);
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(STARTINGRESX,STARTINGRESY,NAME.c_str());
	current->init();
}
void close() {
	current->close();
	CloseWindow();
	cfg.save();
}
void mouse(int button,int state,int x,int y) {
	
}

unsigned long now=0;
unsigned long prv=0;
unsigned long delta=0;
short updatems=1;
void timer(int value) {
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

	if (IsKeyDown(KEY_LEFT_ALT)&& IsKeyDown(KEY_F4)){
		
		close();}
	
	current->update();
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