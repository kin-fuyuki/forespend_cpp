#pragma once
#include"world/world.h"
extern bool running;
extern Window* window;
extern map* world;

void init();
void loop();
void update();
void draw();
void close();