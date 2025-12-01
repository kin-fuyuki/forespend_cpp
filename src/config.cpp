#include"config.h"
#include "term.h"
#include <cstdlib>
#include<string.h>
#include<fstream>
#include <iostream>
#include <filesystem>
std::string savefile = AT("conf.tdf");
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
config::~config() {
	f.close();
}


void config::load() {
	
    f.filepath =(char*) savefile.c_str();
	f.read();
	firstlaunch=false;
	try {
	healthsleep= f.getbool({"health","sleep"});
	healthbreak= f.getbool({"health","break"});
	viewdistance= f.getfloat({"gr","distance"});
	fov= f.getfloat({"gr","fov"});
	resolution= f.getint({"gr","resolution"});
	internalres= f.getbool({"gr","internalres"});
	itemserver= f.getstring({"svr","item"});
	player= f.getstring({"game","player"});
	
	scale= f.getint({"gr","scale"});
	scale=scale>10||scale<1?1:scale;}
	catch(tiny::TDF_ERR e) {
		fatal((char*)e.what());
		exit(-1);
	}
}

void config::save() {
	f.setbool({"health","sleep"},healthsleep);
	f.setbool({"health","sleep"}, healthsleep);
	f.setbool({"health","break"}, healthbreak);
	f.setfloat({"gr","distance"}, viewdistance);
	f.setfloat({"gr","fov"}, fov);
	f.setint({"gr","resolution"}, resolution);
	f.setbool({"gr","internalres"}, internalres);
	f.setint({"gr","scale"}, scale);
	f.setstring({"svr","item"}, itemserver);
	f.setstring({"game","player"}, player);
}