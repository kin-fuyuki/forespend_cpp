#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#define STARTINGRESX	380
#define STARTINGRESY	240
#define STARTINGRES		STARTINGRESX, STARTINGRESY
#include 	<string>
#include	<map>
#ifdef mobile
#define FS(x)
#define PATHGAME "/data/data/kn.endgames.forespend/files/"
#else
#define PATHGAME ""

#endif
#define AT(x) PATHGAME x

#include "libs/tdf.h"

#include "libs/term.h"
extern std::string NAME;
extern std::string VERSION;
extern std::string PATH;
extern short renderw;
extern short renderh;
#define CSTR(x)			(char*)(x.c_str())

struct config {
	/*
	break: 30 minutes game blocking every 2 hours
	sleep: block game during 11pm and 7am
	*/
	
	bool healthsleep, healthbreak, internalres,scale,
	crtmode;
	float viewdistance, fov,resolution;
	std::string itemserver,player;
	config();
	~config();
	void load();
	void save();
private:
bool firstlaunch;
tiny::TDF_FILE f;
};


#endif