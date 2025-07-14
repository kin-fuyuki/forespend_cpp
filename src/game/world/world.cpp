#include "world.h"


map::map() {}
map::~map() {}




Triangle triangle(vec3(0,0,0), vec3(1,0,0), vec3(0,1,0));
void map::render(Window* window) {
	echo("rendering map");
	this->pixelbuffer.clear();
	DRAWBUFFER;
	echo("rendered map");
}