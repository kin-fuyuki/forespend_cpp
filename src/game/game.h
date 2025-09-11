#pragma once
#include"world/world.h"
#include"io.h"
#include"../config.h"
extern bool 	running	;
extern map 		world	;
extern mainmenu menum	;
extern scene* 	current	;
extern config 	cfg		;
extern short	updatems;
void init();
void close();
void timer(int value);
void mouse(int button,int state,int x,int y);
void render();