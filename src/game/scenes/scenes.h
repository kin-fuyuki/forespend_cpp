#pragma once

#include"entity.h"
#ifdef mobile
#define GRAPHICS_API_OPENGL_ES3
#else
#define GRAPHICS_API_OPENGL_21
#endif
struct res{short w,h;std::string n;};
extern res resolutions[];

extern	int scale;
extern float relresx,relresy;
extern bool changescene;
extern short nextscene;
extern 	Font			menufont;
extern config 	cfg		;
extern Color TRUE,FALSE;
class scene{
public:

    virtual void init() {}
    virtual void render() {}
    virtual void update() {}
    virtual void close() {}
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
	playerentity				player;
	playerstats			stats;
	Camera3D			camera;
	Camera3D			third;
	
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
class menuslider:public menuitem{
	public:
	float value;
	menuslider(int x,int y,int w,int h,char ID,float value):menuitem(x,y,w,h,ID),value(value){};
};
class menuedit:public menutext{
	public:
	int cursor;
	unsigned short maxlines;
	menuedit(int x,int y,int w,int h,char ID,std::string text):menutext(x,y,w,h,ID,text),cursor(text.size()),maxlines(-1){};
};
class menuradio:public menuitem{
	public:
	bool value;
	menuradio(int x,int y,int w,int h,char ID,bool value):menuitem(x,y,w,h,ID),value(value){};
};
class menucolor:public menuitem{
	public:
	Color *value;
	menucolor(int x,int y,int w,int h,char ID,Color* value):menuitem(x,y,w,h,ID),value(value){};
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
//