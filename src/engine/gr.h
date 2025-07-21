#pragma once
#define SDL
#ifdef SDL
#include "platform/sdl2.h"
#include "modes.h"
#endif
#define GAME_GRAPHICS 1
#include"term.h"

extern bool accelerated;
class Window;


typedef struct color {
	unsigned char r,g,b,a;
};
extern color clearcolor;
typedef struct buffer{
	short width,height;
	char mode;
	color*data;
	buffer(int width, int height, char mode):width(width),height(height),mode(mode),data(
		 (color*)malloc(width * height * sizeof(color))
	){for (int i = 0; i < width * height; i++){
		data[i] = {clearcolor.r,clearcolor.g,clearcolor.b,clearcolor.a};
	}};
	buffer():width(0),height(0),mode(0),data(0){};
	~buffer(){free(data);};
	void clear(){for (int i = 0; i < width * height; i++){
		echo("clearing %i",i);
		data[i] = {clearcolor.r,clearcolor.g,clearcolor.b,clearcolor.a};
		
		}};
	
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
		Object2D(float x, float y):x(x),y(y),pixelbuffer(x,y,0){};
		void render(Window* window) {}
};
class World:public Object2D{
public:
	color fog;
	Object2D camera;
	Object2D* stuff;

	World(){};
	void render(Window* window) override;
};
	#if GAME_GRAPHICS >=1

	class Object3D: public Object2D{
		public:
			float z;	float pitch, yaw;
			Object3D(){};
			Object3D(float x, float y,float z):Object2D(x,y){this->z = z;};
			void render(Window* window) {}
	};
	struct vec3{
		float x,y,z;
		vec3(float x, float y, float z):x(x),y(y),z(z){};
	};
	struct Triangle{
		vec3 a,b,c;
		Triangle(vec3 a, vec3 b, vec3 c):a(a),b(b),c(c){};
		Triangle():a(0,0,0),b(0,0,0),c(0,0,0){};
		
	};

	class model:public Object3D{
		public:
			float *vertices;
			Triangle *faces[3];	
			void render(Window* window) {}
	};
	class World3D:public Object3D{
	public:
		color fog;
		Object3D camera;
		model* stuff;

		World3D(){};
		void render(Window* window) override;
	};
	#endif
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