#include"gr.h"
bool accelerated=false;


Window::Window(int width, int height, const char* title) {INITWINDOW}
Window::~Window() {CLOSEWINDOW}

void Window::update() {
	
	INITDRAW
	for (int i = 0; i < draws; i++) {
		drawfuncs[i](objects[i], this);
	}
	drawfuncs = nullptr;
	objects = nullptr;
	draws = 0;
	ENDDRAW
}
color clearcolor={254,0,0,0};
void Window::bind(void (*func)(Object*,Window*), Object* obj) {
	drawfuncs = (void(**)(Object*,Window*))realloc(drawfuncs, sizeof(void(*)(Object*)) * (draws + 1));
	drawfuncs[draws] = func;
	objects = (Object**)realloc(objects, sizeof(Object*) * (draws + 1));
	objects[draws] = obj;
	draws++;
}
void World::render(Window* window) {
}