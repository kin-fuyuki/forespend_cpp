#include "scenes.h"
#include <raymath.h>
#include "../../libs/simplex.h"

void updatetextures();

extern	int tilemaploc,sizeloc,colsloc,modelloc,MVPloc,sheetloc,fliploc,fragcolorloc,camloc;
/*
uniform float health;
uniform float stamina;
uniform float mana;
uniform float energy;
uniform float fatigue;
uniform float radioactivity;
uniform float xp;
*/
extern	int	timeloc,healthloc,staminaloc,manaloc,energyloc,fatigueloc,radioloc,xploc;
extern	unsigned char scale;
extern	int texturesupdt;
extern	int flip;
extern	bool f1;
extern	bool drawnworld;
extern	bool headdown;
extern	float headbob;
extern	float y;
extern	bool onsurface;
#define BOBMAX 0.15f
extern	float yaccel;
extern	short ptx,ptz;
extern	unsigned char currenttile;

extern	float sped;
extern	float playerrotx, playerroty;

extern	float daycol[4];
extern	char daytime;
extern	float hourcycle;

extern	FastNoiseLite	heatnoise;
extern	FastNoiseLite	humiditynoise;
extern	FastNoiseLite	populationnoise;
extern	FastNoiseLite	texturenoise;
extern	Matrix 			model;
extern	Model			worldmodel;
extern	float 			worldSizeV[2];

extern	Font			debugfont;

extern	Texture2D		tilesheet;
extern	Texture2D 		tilemapp;
extern	Image 			sheet;
extern	Image 			skybox;
extern	Image			tilemaptx;
extern	Texture 		playercursor;
extern	Texture 		skytexture;
extern	Texture			toolbar;

extern	Shader			toolbarshader;
extern	Material 		basicmat;
extern	Material		generator;
extern	Material 		tilemat;