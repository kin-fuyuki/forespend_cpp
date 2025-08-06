#include"../../config.h"
#include<raylib.h>
#include<vector>

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
	int 	id;
};
class map:public scene{
	std::vector<char>	tiles;
	std::vector<char>	buildings;
	Mesh				mesh;
	void updateChunks();

public:
	bool mustupdate;
	map();
	~map();
	
	short	size;
	short	world;
	bool	paused;
	std::vector<entity> entities;
	entity				player;
	
	void init()		override;
	void update()	override;
	void render()	override;
	void close() 	override;

	void updatechunkmesh();

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