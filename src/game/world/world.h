#include "scenes.h"
#include <raymath.h>
#include "../../libs/simplex.h"

void updatetextures();

extern	int texturesupdt;
extern	float y;
extern	bool drawnworld;
extern	bool headdown;
extern	float headbob;
extern	bool onsurface;
#define BOBMAX 0.15f
extern	float yaccel;
extern	short ptx,ptz;
extern	unsigned char currenttile;


extern	int flip;
extern	float playerrotx, playerroty;

extern	Model worldmodel;
extern	Material generator;
extern	Image tilemaptx;
extern	FastNoiseLite humiditynoise;
extern	FastNoiseLite heatnoise;
extern	FastNoiseLite populationnoise;
extern	float daycol[4];
extern	char daytime;
extern	float hourcycle;
extern	Material tilemat;
extern	Texture2D tilesheet;
extern	int tilemaploc,sizeloc,colsloc,modelloc,MVPloc,sheetloc,fliploc,fragcolorloc,camloc;
extern	Matrix model;
extern	Material basicmat;
extern	Texture2D tilemapp;
extern	float worldSizeV[2];
extern	FastNoiseLite texturenoise;
extern	Image sheet;
extern	Image skybox;
extern	Texture skytexture;
extern	Texture playercursor;