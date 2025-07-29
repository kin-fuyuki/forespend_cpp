#include "world.h"


map::map() {}
map::map(int width, int height) : pixelbuffer(width, height,0) {
}
map::~map() {}




Triangle triangle(vec3(0,0,0), vec3(1,0,0), vec3(0,1,0));
void map::render(Window* window) {
	SDL_UpdateTexture(window->framebuffer, nullptr, pixelbuffer.data, pixelbuffer.width*4);
	SDL_RenderCopy(window->renderer, window->framebuffer, nullptr, nullptr);
	short pos = 100;
	short idx=100;
	for (short x = 0; x < 100; x++) {
		for (short y = 0; y < x; y++) {
			short px = x + pos;
			short py = y + pos;
			
			this->pixelbuffer.data[py * 380 + px] = { 255,255,255,255 };
		}
	}
}