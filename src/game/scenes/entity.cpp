#include "entity.h"
#include "scenes.h"
#include<fstream>
#include <iostream>
#include <filesystem>
#include <raylib.h>

Texture2D singletex={0,-1,0,0,0};
entity::entity(){

}
void entity::init(){
	
std::string defaultplayer="";
	tiny::fatal("player config: %s",cfg.player.c_str());
if (cfg.player!="")
defaultplayer = PATHGAME + std::string("player/") + cfg.player.c_str() + std::string("/");
else
defaultplayer = AT("res/defaultplayer/");
	if(singletex.width==-1)
	singletex=LoadTexture((defaultplayer+"player.png").c_str());
	framew=singletex.width;
	framwh=singletex.height;
	fs.push_back(&singletex);
	tiny::fatal ("mode: %s",singletex.format==PIXELFORMAT_UNCOMPRESSED_R8G8B8A8?"RGBA8":
		singletex.format==PIXELFORMAT_UNCOMPRESSED_R8G8B8?"RGB8":
		"other");
			FILE *p_file = NULL;
	std::string path = PATHGAME+defaultplayer + "player.tdf";
	tiny::TDF_FILE file;
	file.filepath = (char*)path.c_str();
	file.read();
	
	std::string name=*(std::string*)((file.data->at("name")).datapointer);
	desc=*(std::string*)((file.data->at("description")).datapointer);
	singlesprite=*(bool*)((file.data->at("singlesprite")).datapointer);
	scalex=*(float*)((file.data->at("width")).datapointer)+0.001;
	scaley=*(float*)((file.data->at("height")).datapointer)+0.001;
	tiny::fatal("scale x %i y %i",scalex,scaley);
	file.close();
	
}
playerentity::playerentity(){
	
}
