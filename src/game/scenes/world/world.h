#include "../scenes.h"
#include <raylib.h>
#include <raymath.h>
#include "../../../libs/simplex.h"

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
extern	int texturesupdt;
extern	int flip;
extern	bool f1,f3;
extern	bool drawnworld;
extern	bool headdown;
extern	bool drawinv;
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

/*
/usr/bin/ld: build/objs/linux/game/scene/_render.cpp.o:(.bss+0x18): multiple definition of `menu'; build/objs/linux/main.cpp.o:(.bss+0xd0): first defined here
clang++: error: linker command failed with exit code 1 (use -v to see invocation)
make: *** [Makefile:56: out/linux/bin/l5.64] Error 1
/usr/bin/x86_64-w64-mingw32-ld: build/objs/windows/game/scene/_render.cpp.o:/home/komi/src/cpp/forespend_cpp/src/game/scene/_render.cpp:13: multiple definition of `menu'; build/objs/windows/main.cpp.o:/home/komi/src/cpp/forespend_cpp/src/main.cpp:5: first defined here
clang++: error: linker command failed with exit code 1 (use -v to see invocation)
*/
extern	short			page;
extern	std::vector<menupage*> menu;

extern	FastNoiseLite	heatnoise;
extern	FastNoiseLite	humiditynoise;
extern	FastNoiseLite	populationnoise;
extern	FastNoiseLite	texturenoise;
extern	Matrix 			model;
extern	Model			worldmodel;
extern	float 			worldSizeV[2];
extern	Camera3D*		cameras[2];
extern	bool			thirdperson;
extern	Font			debugfont;


extern	Texture2D		tilesheet;
extern	Texture2D 		tilemapp;
extern	Image 			sheet;
extern	Image 			skybox;
extern	Image			tilemaptx;
extern	Texture 		playercursor;
extern	Texture 		skytexture;
extern	Texture			toolbar;
extern	Texture			inventory;

extern	Shader			toolbarshader;
extern	Material 		basicmat;
extern	Material		generator;
extern	Material 		tilemat;