#include "world.h"



model cube;
map::map() {}
map::map(int width, int height) : pixelbuffer(width, height,0) {
	cube.vertices= new vec3[8]
	{
		{0,0,0},
		{1,0,0},
		{0,1,0},
		{1,1,0},
		{0,0,1},
		{1,0,1},
		{0,1,1},
		{1,1,1}
	};
	cube.faces= new Triangle[12]
	{
		{0,1,2},
		{1,3,2},
		{4,5,6},
		{5,7,6},
		{0,1,4},
		{1,5,4},
		{2,3,6},
		{3,7,6},
		{0,2,4},
		{2,6,4},
		{1,3,5},
		{3,7,5}
	};
}
map::~map() {}


void map::render(Window* window) {
	SDL_UpdateTexture(window->framebuffer, nullptr, pixelbuffer.data, pixelbuffer.width*4);
	SDL_RenderCopy(window->renderer, window->framebuffer, nullptr, nullptr);
}