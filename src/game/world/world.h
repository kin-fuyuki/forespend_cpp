#include"../../config.h"
#include"../io.h"
#ifdef mobile
#define GRAPHICS_API_OPENGL_ES3
#else
#define GRAPHICS_API_OPENGL_33
#endif
#include<raylib.h>
#include <rlgl.h>
#include<vector>
#include<map>
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