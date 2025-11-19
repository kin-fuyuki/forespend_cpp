#include "entity.h"
#include "scenes.h"
#include "../../libs/yyjson.h"
#include<fstream>
#include <iostream>
#include <filesystem>
#include <raylib.h>

Texture2D singletex={0,-1,0,0,0};
entity::entity(){
std::string defaultplayer = "res/defaultplayer/";
	FILE *p_file = NULL;
	std::string path = PATHGAME+defaultplayer + "player.json";
	std::cerr << path << "\n";
	std::cerr << path << "\n";
	std::cerr << path << "\n";
	std::cerr << path << "\n";
	std::cerr << path << "\n";
	std::cerr << path << "\n";
	std::cerr << path << "\n";

	p_file = fopen(path.c_str(),"rb");
	fseek(p_file,0,SEEK_END);// sigsegv
	int size = ftell(p_file);
	fclose(p_file);
	std::ifstream file(path.c_str());
	frame=0;
	if (file.is_open()) {
		char *filec = new char[size + 1];
		filec[size] = '\0';
		file.read(filec, sizeof(filec));
		yyjson_doc *doc = yyjson_read(filec,size, 0);
		if (doc) {
			yyjson_val *root = yyjson_doc_get_root(doc);
			yyjson_val *namee = yyjson_obj_get(root, "name");
			yyjson_val *descc = yyjson_obj_get(root, "description");
			yyjson_val *single = yyjson_obj_get(root, "singlesprite");
			
			// Check for NULL before assigning to std::string
			const char *namestr = yyjson_get_str(namee);
			const char *descstr = yyjson_get_str(descc);
			
			name = namestr ? namestr : "";
			desc = descstr ? descstr : "";
			singlesprite = yyjson_get_bool(single);
			
			yyjson_doc_free(doc);
		}
		delete[] filec;
	}
	
}
void entity::init(){
	
std::string defaultplayer="";

	fatal("player config: %s",cfg.player.c_str());
if (cfg.player!="")
defaultplayer = PATHGAME + std::string("player/") + cfg.player.c_str() + std::string("/");
else
defaultplayer = AT("res/defaultplayer/");
	if(singletex.width==-1)
	singletex=LoadTexture((defaultplayer+"player.png").c_str());
	framew=singletex.width;
	framwh=singletex.height;
	fs.push_back(&singletex);
	fatal ("mode: %s",singletex.format==PIXELFORMAT_UNCOMPRESSED_R8G8B8A8?"RGBA8":
		singletex.format==PIXELFORMAT_UNCOMPRESSED_R8G8B8?"RGB8":
		"other");
}