#ifndef CONFIG_H
#define CONFIG_H

#define STARTINGRESX	380
#define STARTINGRESY	240
#define STARTINGRES		STARTINGRESX, STARTINGRESY
#include <string>
extern std::string NAME;
extern std::string VERSION;
extern std::string PATH;
#define CSTR(x)			(char*)(x.c_str())

#endif