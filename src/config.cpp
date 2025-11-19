#include"config.h"
#include<string.h>
#include<fstream>
#include <iostream>
#include <filesystem>
std::string savefile = AT("save.cfg");
config::config() {
	firstlaunch=true;
	healthsleep=false;
	healthbreak=false;
	viewdistance=1;
	fov=80;
	resolution=1;
	internalres=true;
	scale=4;
	itemserver="kosumi.ddns.net:64001";
	player="";
	load();
}


void config::load() {
	
	std::ifstream file(savefile.c_str());
	if (file.is_open()) {
		firstlaunch=false;
		// config format: cat.key=value, cat.key works as key, cat is just to organize
		for (std::string line; std::getline(file, line);) {
			if (line[0] == '#') {
				continue;
			}
			size_t pos = line.find('=');
			if (pos == std::string::npos) {
				continue;
			}
			std::string cat = line.substr(0, pos);
			std::string key = line.substr(pos + 1);
			if (cat=="health.sleep") healthsleep=key=="true";
			if (cat=="health.break") healthbreak=key=="true";
			if (cat=="gr.distance") viewdistance=atoi(key.c_str());
			if (cat=="gr.fov") fov=atoi(key.c_str());
			if (cat=="gr.resolution") resolution=atoi(key.c_str());
			if (cat=="gr.internalres") internalres=key=="true";
			if (cat=="gr.scale") scale=atoi(key.c_str());
			if (cat=="gr.crtmode") crtmode=key=="true";
			if (cat=="svr.item") itemserver=key;
			if (cat=="game.player") player=key;
			scale=scale>10||scale<1?1:scale;
		}
			
		file.close();
	}
}

void config::save() {
	std::ofstream file(savefile.c_str(), std::ios::out | std::ios::trunc);
	if (file.is_open()) {
		file << "# health options\n# sleep warning, auto-close and not allowing to open game\n";
		file << "health.sleep=" << (healthsleep?"true":"false") << std::endl;
		file << "# break warning, freezes game for 30 minutes every 2 hours\n";
		file << "health.break=" << (healthbreak?"true":"false") << std::endl;
		file << "gr.distance=" << viewdistance << std::endl;
		file << "gr.fov=" << fov << std::endl;
		file << "gr.resolution=" << resolution << std::endl;
		file << "gr.internalres=" << (internalres?"true":"false") << std::endl;
		file << "gr.scale=" << scale << std::endl;
		file << "svr.item=" << itemserver << std::endl;
		file << "game.player=" << player << std::endl;
		file.close();
	}
}
