#pragma once
#include"world/world.h"
#include"io.h"
extern bool 	running;
extern map 		world;
extern Window 	window;
extern scene* 	current;
void init();
void close();
void keyboard(unsigned char key,int x,int y);
void keyboardup(unsigned char key,int x,int y);
void mouse(int button,int state,int x,int y);
void motion(int x,int y);
void render();