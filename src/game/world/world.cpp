#include "world.h"


map::map() {}
map::map(int width, int height) : pixelbuffer(width, height,0) {
}
map::~map() {}




Triangle triangle(vec3(0,0,0), vec3(1,0,0), vec3(0,1,0));
void map::render(Window* window) {
	this->pixelbuffer.clear();
	SDL_UpdateTexture(window->framebuffer, nullptr, pixelbuffer.data, pixelbuffer.width*4);\
	SDL_RenderCopy(window->renderer, window->framebuffer, nullptr, nullptr);
}