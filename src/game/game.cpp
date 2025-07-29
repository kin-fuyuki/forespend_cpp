#include "game.h"

void init() {
	startup("forespend","0.0f");
	window = new Window(PERFECT_LOW, "forespend - 0.0f");
	world = new map(PERFECT_LOW);
}
void loop() {
	const char frametime=16;
	
	while (window->eventupdate())
	{
		int start = SDL_GetTicks();
		update();
		draw(); // :3c vewy simpwe lupis
		int end = SDL_GetTicks();
		int wait = frametime - (end - start);
		if (wait > 0) SDL_Delay(wait);
	}
    
}

char currcolor = 0;
void update() {
	switch (currcolor) {
    case 0:
        if (clearcolor.r < 255) {
            clearcolor.r++;
        }
        if (clearcolor.b > 0) {
            clearcolor.b--;
        }
        if (clearcolor.r == 255 && clearcolor.b == 0) {
            currcolor = 1;
        }
        break;
    case 1:
        if (clearcolor.g < 255) {
            clearcolor.g++;
        }
        if (clearcolor.r > 0) {
            clearcolor.r--;
        }
        if (clearcolor.g == 255 && clearcolor.r == 0) {
            currcolor = 2;
        }
        break;
    case 2:
        if (clearcolor.b < 255) {
            clearcolor.b++;
        }
        if (clearcolor.g > 0) {
            clearcolor.g--;
        }
        if (clearcolor.b == 255 && clearcolor.g == 0) {
            currcolor = 0;
        }
        break;
}

}
void draw() {
	window->bind(world->draw, world);
	window->update();
}
void close() {
	
}