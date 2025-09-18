#include "entity.h"
#include "../../libs/yyjson.h"
#include<fstream>
#include <iostream>
#include <filesystem>
std::string defaultplayer = "res/defaultplayer/";


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
	}
}