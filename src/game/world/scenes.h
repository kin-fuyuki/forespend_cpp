#pragma once
#include"../../config.h"
#include "../../term.h"
#include"../io.h"
#ifdef mobile
#define GRAPHICS_API_OPENGL_ES3
#else
#define GRAPHICS_API_OPENGL_21
#endif
#include<raylib.h>
#include <rlgl.h>
#include<vector>
#include<map>
extern	int scale;
extern float relresx,relresy;
extern bool changescene;
extern short nextscene;
extern 	Font			menufont;
extern config 	cfg		;
class scene{
public:

    virtual void init() {}
    virtual void render() {}
    virtual void update() {}
    virtual void close() {}
};
struct entity{
	int 	qx,qz;
	float 	x,y,z;
	float 	yaw,pitch,roll;
	int 	id;
};
class data{
	
};
class playerstats:public data{
public:
	float health;
	float stamina;
	float mana;
	float energy;
	float fatigue;
	float radioactivity;
	float xp;
};
class map:public scene{
	Color skycolor;
	unsigned char*	tiles;
	std::vector<unsigned char>	buildings;
	int							data[4];

public:
	map();
	~map();
	
	bool 	mustupdate;
	short	size;
	short	world;
	bool	paused;
	std::vector<entity> entities;
	entity				player;
	playerstats			stats;
	Camera3D			camera;
	
	void init()		override;
	void update()	override;
	void render()	override;
	void close() 	override;

	void updatechunks();
};

class mainmenu:public scene{
public:
	mainmenu();
	~mainmenu();
	void init()		override;
	void update() 	override;
	void render()	override;
	void close()	override;
};

class menuitem{
public:
	int x;
	int y;
	int w;
	int h;
	char ID;
	virtual ~menuitem()=default;
	menuitem(int x,int y,int w,int h,char ID):x(x),y(y),w(w),h(h),ID(ID){};
};
class menutext:public menuitem{
public:
	std::string text;
	menutext(int x,int y,int w,int h,char ID,std::string text):menuitem(x,y,w,h,ID),text(text){};
};
class menubutton:public menutext{
public:
	bool hover,down;
	void (*process)();
	menubutton(int x,int y,int w,int h,char ID,std::string text,void (*process)()):menutext(x,y,w,h,ID,text),process(process){
		hover=down=false;
		
	};
};
class menupage{
	public:
	std::vector<menuitem*> items;
	menupage(std::vector<menuitem*> items,int ID):items(items),ID(ID){};
	char ID;
};


extern	short* p;
extern menupage options;
extern menupage graphics;
extern menupage sound;
extern menupage controls;
//&options,&graphics,&sound,&controls