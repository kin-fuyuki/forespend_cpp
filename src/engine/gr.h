#pragma once
#define SDL
#ifdef SDL
#include "platform/sdl2.h"
#endif
#define GAME_GRAPHICS 1


extern bool accelerated;
class Window;


typedef struct color {
	unsigned char r,g,b,a;
};

typedef struct buffer{
	const short width,height;
	char mode;
	color*data;
	buffer(int width, int height, char mode, color* data):width(width),height(height),mode(mode),data(data){};
	buffer():width(0),height(0),mode(0),data(0){};
};

class Object{
public:
	int UUID;
	virtual void render(Window* window)=0;
	static void draw(Object* self,Window* window) {self->render(window);}
	Object** children;
};

#if GAME_GRAPHICS >=0
class Object2D: public Object{
	public:
		float x, y;	float roll;
		buffer pixelbuffer;
		Object2D(){};
		Object2D(float x, float y):x(x),y(y),pixelbuffer(x,y,0, (color*)malloc(x*y * sizeof(color))){};
		void render(Window* window) {}
};
class world:public Object2D{
public:
	color fog;
	Object2D camera;
	Object2D* stuff;

	world(){};
	void render(Window* window) override;
};
#elif GAME_GRAPHICS >=1

class Object3D: public Object2D{
	public:
		float z;	float pitch, yaw;
		Object3D(){};
		Object3D(float x, float y,float z):Object2D(x,y){this->z = z;};
		void render(Window* window) {}
};
class model:public Object3D{
	public:
		float *vertices;
		int *faces[3];	
		void render(Window* window) {}
};
class world3D:public Object3D{
public:
	color fog;
	Object3D camera;
	model* stuff;

	world3D(){};
	void render(Window* window) override;
};
#endif

class Window {
	public:
		void (**drawfuncs)(Object*,Window*)=nullptr;
		Object** objects=nullptr;
		int draws = 0;
		windowtype window; renderertype renderer;
		
		Window(int width, int height, const char* title);
		~Window();
		void update();
		void bind(void (*func)(Object*,Window*), Object* obj);
		texturetype framebuffer;

};