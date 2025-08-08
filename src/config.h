#pragma once
#define STARTINGRESX	380
#define STARTINGRESY	240
#define STARTINGRES		STARTINGRESX, STARTINGRESY
#include 	<string>
#include	<map>
extern std::string NAME;
extern std::string VERSION;
extern std::string PATH;
#define CSTR(x)			(char*)(x.c_str())

struct config {
	/*
	break: 30 minutes game blocking every 2 hours
	sleep: block game during 11pm and 7am
	*/
bool firstlaunch;
	bool healthsleep, healthbreak;
	short viewdistance, fov,resolution;
	config();
	void load();
	void save();
	
};