#include"../gr.h"
#include<vector>
class scene{
public:
	virtual void init();
	virtual void render();
	virtual void update();
	virtual void close();
};
struct tile{
	short 	height;
	bool 	hasbillboard;
};
struct entity{
	//4x4 quadrants
	int 	qx,qz;
	float 	x,y,z;
	int 	id;
};
class map:public scene{
	std::vector<tile> tiles;
	std::vector<char> buildings;
	std::vector<vec3> vertices;
	void updateChunks();

public:

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
	
};

class mainmenu:public scene{
public:
	void init()		override;
	void update() 	override;
	void render()	override;
	void close()	override;
};