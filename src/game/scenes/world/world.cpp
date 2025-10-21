#include "world.h"
#include <vector>
#include <string>
#define MAX_MATERIAL_MAPS
#define MAX_MESH_VERTEX_BUFFERS
Model worldmodel;
Material generator;
Image tilemaptx;
Material tilemat;
Texture2D tilesheet;
float daycol[4]={.8,.8,.8,.8};
char daytime=2;
float hourcycle=0.;
float worldSizeV[2];

int tilemaploc,sizeloc,colsloc,modelloc,MVPloc,
	sheetloc,fliploc,fragcolorloc,camloc,
	timeloc,healthloc,staminaloc,manaloc,
	energyloc,fatigueloc,radioloc,xploc;

Matrix model=Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
Material basicmat;
Texture2D tilemapp;
Image sheet;
Image skybox;
Texture skytexture;
Texture playercursor;
Texture toolbar;
Shader toolbarshader;
Font debugfont;
Font menufont;
Camera3D*		cameras[2];
map::map(){
	// 1024, 2048, 4096, 8192, 16384
	//
	
	size=1024;
	tiles=new unsigned char[size*size];
	tilemaptx.data=new unsigned char[size*size]{255};
	for (int i = 0; i < size*size; i++) {
		((unsigned char*)tilemaptx.data)[i]=255;
	}
	tilemaptx.format=PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	tilemaptx.width=size>>2;
	tilemaptx.height=size;
	tilemaptx.mipmaps = 1;
	mustupdate=true;
	data[0]=0;
	data[1]=0;
	data[2]=0;
	stats.health=stats.stamina=stats.mana=stats.energy=stats.fatigue=stats.radioactivity=stats.xp=1.0f;
}
void map::init(){
	player.init();
	worldSizeV[0] = (float)size;worldSizeV[1] = (float)size;
	camera.position = (Vector3){0.0f, 1.3f, 0.0f};
	camera.target = (Vector3){8.0f, 0.0f, 8.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = (float)cfg.fov;
	camera.projection = CAMERA_CUSTOM;
	third.position = (Vector3){0.0f, 2.f, 0.0f};
	third.target = (Vector3){8.0f, 0.0f, 8.0f};
	third.up = (Vector3){0.0f, 1.0f, 0.0f};
	third.fovy = (float)cfg.fov/1.5;
	third.projection = CAMERA_CUSTOM;
	skycolor=(Color){255,255,255,255};
	skybox=LoadImage("res/images/sky.png");
	cameras[0]=&camera;
	cameras[1]=&third;
	tilemapp=LoadTextureFromImage(tilemaptx);
	tilemat=LoadMaterialDefault();
	toolbarshader=LoadShader("res/shaders/toolbar.vs","res/shaders/toolbar.fs");
	playercursor=LoadTexture("res/images/playercursor.png");
	debugfont=LoadFont("res/fonts/kipsynth.ttf");
	toolbar=LoadTexture("res/images/menubar.png");
	
	rlSetClipPlanes(.2, 1000.0);

	tilesheet = LoadTexture("res/images/tilesheet.png");
	//SetTextureFilter(tilesheet, TEXTURE_FILTER_N);

	tilemat.shader=LoadShader("res/shaders/tile.vs","res/shaders/tile.fs");
	//
	tilemaploc  = GetShaderLocation(tilemat.shader, "tilemap");
	sheetloc= GetShaderLocation(tilemat.shader, "tilesheet");
	MVPloc = GetShaderLocation(tilemat.shader, "mvp");
	modelloc = GetShaderLocation(tilemat.shader, "modelmat");
	sizeloc = GetShaderLocation(tilemat.shader, "size");
	fragcolorloc = GetShaderLocation(tilemat.shader, "fragColor");
	camloc = GetShaderLocation(tilemat.shader, "campos");
	colsloc = GetShaderLocation(tilemat.shader, "cols");
	fliploc = GetShaderLocation(tilemat.shader, "flip");
	timeloc = GetShaderLocation(toolbarshader, "time");
	healthloc = GetShaderLocation(toolbarshader, "health");
	staminaloc = GetShaderLocation(toolbarshader, "stamina");
	manaloc = GetShaderLocation(toolbarshader, "mana");
	energyloc = GetShaderLocation(toolbarshader, "energy");
	fatigueloc = GetShaderLocation(toolbarshader, "fatigue");
	radioloc = GetShaderLocation(toolbarshader, "radioactivity");
	xploc = GetShaderLocation(toolbarshader, "xp");
	worldmodel=LoadModel("res/models/world.obj");
	//
	echo("meshcount %i",worldmodel.meshCount);
	echo("materialcount %i",worldmodel.materialCount);
	basicmat=LoadMaterialDefault();
	
	humiditynoise=FastNoiseLite((this->world*3)+1);
	heatnoise=FastNoiseLite((this->world*3)+2);
	populationnoise=FastNoiseLite((this->world*3));
	
	humiditynoise.SetFractalType(FastNoiseLite::FractalType::FractalType_None);
	humiditynoise.SetFractalOctaves(3);
	humiditynoise.SetFrequency(0.4);
	humiditynoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Value);
	humiditynoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
	
	
	heatnoise.SetFractalType(FastNoiseLite::FractalType::FractalType_None);
	heatnoise.SetFractalOctaves(1);
	heatnoise.SetFrequency(0.1);
	heatnoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Value);
	heatnoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
	populationnoise.SetFractalType(FastNoiseLite::FractalType::FractalType_None);
	populationnoise.SetFractalOctaves(1);
	populationnoise.SetFrequency(0.1);
	populationnoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Value);
	populationnoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
	sheet=LoadImageFromTexture(tilesheet);
	skytexture=LoadTextureFromImage(skybox);
	updatetextures();
	
}

map::~map(){
	UnloadModel(worldmodel);
	
	UnloadTexture(tilesheet);
	UnloadTexture(tilemapp);
	UnloadTexture(skytexture);
	UnloadTexture(playercursor);
	UnloadTexture(toolbar);
	
	UnloadImage(sheet);
	UnloadImage(skybox);
	UnloadImage(tilemaptx);
	
	UnloadMaterial(basicmat);
	UnloadShader(toolbarshader);
	
	UnloadFont(debugfont);
	UnloadFont(menufont);
}

void map::close(){
	unsigned char *data=new unsigned char[size*size*3];
	tilesheet = LoadTexture("res/images/tilesheetl4.png");
	sheet=LoadImageFromTexture(tilesheet);
	for (int i = 0; i < size*size; i++) {
		unsigned char tile = ((unsigned char*)tilemaptx.data)[i];
		int xtilecoord=tile%16;
		int ytilecoord=tile/16;
		data[i*3+0]=((unsigned char*)sheet.data)[(ytilecoord*16+xtilecoord)*4+0];
		data[i*3+1]=((unsigned char*)sheet.data)[(ytilecoord*16+xtilecoord)*4+1];
		data[i*3+2]=((unsigned char*)sheet.data)[(ytilecoord*16+xtilecoord)*4+2];
		
	}
	savebmp("tilemap.bmp",data,
	tilemaptx.format==PIXELFORMAT_UNCOMPRESSED_GRAYSCALE?tilemaptx.width:tilemaptx.width
	,tilemaptx.height);
}
std::vector<menupage*> menu={
		new menupage{//main
			{
				new menubutton{ 0,20,360,20, 0, "return",
					[](){ page=-1; }},
					
				new menubutton{ 0,60,360,20, 1, "save",
					[](){ }},
					
				new menubutton{ 0,90,360,20, 2, "load",
					[](){ }},
					
				new menubutton{0,140,360,20, 1, "options",
					[](){ page=1; }},
					
				new menubutton{ 0,180,360,20, 2, "exit",
					[](){ changescene=true;nextscene=0; }}
			},0},
			&options,&graphics,&sound,&controls
	};

