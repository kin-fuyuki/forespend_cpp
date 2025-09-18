#include "entity.h"
#include "../../libs/yyjson.h"
#include<fstream>
#include <iostream>
#include <filesystem>
#include <raylib.h>
std::string defaultplayer = "res/defaultplayer/";

Texture2D singletex={0,-1,0,0,0};
entity::entity(){
	FILE *p_file = NULL;
	p_file = fopen(std::string(defaultplayer+"player.json").c_str(),"rb");
	fseek(p_file,0,SEEK_END);
	int size = ftell(p_file);
	fclose(p_file);
	std::ifstream file(std::string(defaultplayer+"player.json").c_str());
	
	if (file.is_open()) {
		char filec[size];
		file.read(filec, sizeof(filec));
		yyjson_doc *doc = yyjson_read(filec, strlen(filec), 0);
		file.close();
		yyjson_val *root = yyjson_doc_get_root(doc);
		yyjson_val *namee = yyjson_obj_get(root, "name");
		yyjson_val *descc = yyjson_obj_get(root, "description");
		yyjson_val *single = yyjson_obj_get(root, "singlesprite");
		
		name=yyjson_get_str(namee);
		desc=yyjson_get_str(descc);
		singlesprite=yyjson_get_bool(single);
		yyjson_doc_free(doc);
	}
	singletex=LoadTexture((defaultplayer+"player.png").c_str());
	fs.push_back(&singletex);
	
}