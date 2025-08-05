#pragma once
#include"world/world.h"
#include"io.h"
extern bool 	running	;
extern map 		world	;
extern Window 	window	;
extern scene* 	current	;
extern short	updatems;
void init();
void close();
void keyboard(unsigned char key,int x,int y);
void keyboardup(unsigned char key,int x,int y);
void special(int key,int x,int y);
void specialup(int key,int x,int y);
void timer(int value);
void mouse(int button,int state,int x,int y);
void motion(int x,int y);
void render();