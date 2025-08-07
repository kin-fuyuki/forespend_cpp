#include"../../config.h"
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
	//4x4 quadrants
	int 	qx,qz;
	float 	x,y,z;
	float 	yaw,pitch,roll;
	int 	id;
};
class map:public scene{
	std::vector<unsigned char>	tiles;
	Image tilemap;
	std::vector<unsigned char>	buildings;
	int							data[4];
	void updateChunks();

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