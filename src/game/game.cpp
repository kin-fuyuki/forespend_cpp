#include "game.h"

void init() {
	startup("forespend","0.0f");
	window = new Window(PERFECT_LOW, "forespend - 0.0f");
	world = new map();
	world->pixelbuffer = buffer(PERFECT_LOW, 0);
}
void loop() {
	const char frametime=16;
	
	while (running)
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
	//clearcolor changing through hue, currcolor is 0=r 1=g 2=b
	switch (currcolor) {
		case 0:
			if (++clearcolor.r > 255){ currcolor = 1; clearcolor.b=0; break; }
			if (--clearcolor.b < 0){ clearcolor.b=0; }
			break;
		case 1:
			if (++clearcolor.g > 255){ currcolor = 2; clearcolor.r=0; break; }
			if (--clearcolor.r < 0){ clearcolor.r=0; }
			break;
		case 2:
			if (++clearcolor.b > 255){ currcolor = 0; clearcolor.g=0; break; }
			if (--clearcolor.g < 0){ clearcolor.g=0; }
			break;
	}
	//success("clearcolor: %d %d %d",clearcolor.r,clearcolor.g,clearcolor.b);
}
void draw() {
	world->render(window);
	window->update();
}
void close() {
	
}