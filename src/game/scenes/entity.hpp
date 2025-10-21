#pragma once
#include"../../config.hpp"
#include "../../term.h"
#include"../io.h"
#include<raylib.h>
#include <rlgl.h>
#include <string>
#include<vector>
#include<map>
class data{
	std::string path;
};
struct entity{
std::string	name;
std::string	desc;
	
	int 	qx,qz;
	float 	x,y,z;
	float 	yaw,pitch,roll;
	int 	id;
	char	frames;
	char	frame;
	char	sides;
	char	framepside;
	bool	singlesprite;
std::vector<Texture2D*>	fs;
	entity();
	void init();
	//entity(float x,float y,float z);
	//entity(float x,float y,float z, std::string sprite);
	/*
	
	x+ = 0 x- = 1 --|-- z+ = 2 z- = 3
	xy+ = 4 x+y- = 5 --|-- x-y+ = 6 xy- = 7
	
	*/
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