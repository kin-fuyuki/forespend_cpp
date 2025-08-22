#include "world.h"
#include "../../term.h"
#include "../../libs/simplex.h"
#include <raymath.h>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <stack>
#include <unordered_map>
#define MAX_MATERIAL_MAPS
#define MAX_MESH_VERTEX_BUFFERS
Model worldmodel;
Material generator;
Image tilemaptx;
FastNoiseLite humiditynoise;
FastNoiseLite heatnoise;
FastNoiseLite populationnoise;
map::map(){
	// 1024, 2048, 4096, 8192, 16384
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
}
Material tilemat;
Texture2D tilesheet;
int tilemaploc,sizeloc,colsloc,modelloc,MVPloc,sheetloc,fliploc;
Matrix model=Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

Material basicmat;
	Texture2D tilemapp;
	float worldSizeV[2];
FastNoiseLite texturenoise;
Image sheet;
Image skybox;
Texture skytexture;
void map::init(){
	worldSizeV[0] = (float)size;worldSizeV[1] = (float)size;
	camera.position = (Vector3){0.0f, 2.0f, 0.0f};
	camera.target = (Vector3){8.0f, 0.0f, 8.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 80.0f;
	camera.projection = CAMERA_CUSTOM;
	skycolor=(Color){255,255,255,255};
	worldmodel=LoadModel("res/models/world.obj");
	skybox=LoadImage("res/images/sky.png");
	
	tilemapp=LoadTextureFromImage(tilemaptx);
	tilemat=LoadMaterialDefault();
	tilemat.shader=LoadShader("res/shaders/tile.vs","res/shaders/tile.fs");
	rlSetClipPlanes(.5, 4000.0);

	tilesheet = LoadTexture("res/images/tilesheet.png");
	//SetTextureFilter(tilesheet, TEXTURE_FILTER_N);

	tilemaploc  = GetShaderLocation(tilemat.shader, "tilemap");
	sheetloc= GetShaderLocation(tilemat.shader, "tilesheet");
	MVPloc = GetShaderLocation(tilemat.shader, "mvp");
	modelloc = GetShaderLocation(tilemat.shader, "modelmat");
	sizeloc = GetShaderLocation(tilemat.shader, "size");
	
	colsloc = GetShaderLocation(tilemat.shader, "cols");
	fliploc = GetShaderLocation(tilemat.shader, "flip");
	echo("meshcount %i",worldmodel.meshCount);
	echo("materialcount %i",worldmodel.materialCount);
	basicmat=LoadMaterialDefault();
	
	humiditynoise=FastNoiseLite((this->world*3)+1);
	heatnoise=FastNoiseLite((this->world*3)+2);
	populationnoise=FastNoiseLite((this->world*3)+3);
	
	humiditynoise.SetFractalType(FastNoiseLite::FractalType::FractalType_None);
	humiditynoise.SetFractalOctaves(1);
	humiditynoise.SetFrequency(0.1);
	humiditynoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Value);
	humiditynoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
	
	
	heatnoise.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
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
}

map::~map(){
}
int flip=0;
void map::render(){
	Matrix view = MatrixLookAt(camera.position, camera.target, camera.up);
	float aspect = (float)GetScreenWidth() / (float)GetScreenHeight();
	Matrix projection = MatrixPerspective(80, aspect, .5, 4000.0);
	Matrix mvp = MatrixMultiply(model, MatrixMultiply(view, projection));
	int cols = 16;
	flip = 0;
	BeginMode3D(camera);
	rlEnableColorBlend();
	SetShaderValue(tilemat.shader, colsloc, &flip, SHADER_UNIFORM_INT);
	SetShaderValueTexture(tilemat.shader, tilemaploc, tilemapp);
	SetShaderValueTexture(tilemat.shader, sheetloc, tilesheet);
	SetShaderValue(tilemat.shader, colsloc, &cols, SHADER_UNIFORM_INT);
	SetShaderValue(tilemat.shader, sizeloc, worldSizeV, SHADER_UNIFORM_VEC2);
	SetShaderValueMatrix(tilemat.shader,modelloc, model);
	SetShaderValueMatrix(tilemat.shader,MVPloc, mvp);
	rlDisableBackfaceCulling();
	//basicmat tilemat
	DrawMesh(worldmodel.meshes[0],tilemat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	DrawMesh(worldmodel.meshes[1],tilemat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	DrawMesh(worldmodel.meshes[2],tilemat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	
	DrawCube((Vector3){0,0,0},1,1,1,WHITE);
	EndMode3D();
	DrawTexture(tilesheet,0,180,WHITE);
	std::string pos="X: "+std::to_string((int)player.x)+" Y: "+std::to_string((int)player.y)+" Z: "+std::to_string((int)player.z);
	DrawText(pos.c_str(), 0, 0, 32, WHITE);
	DrawText(std::to_string(GetFPS()).c_str(), 0, 36, 32, WHITE);
}
int texturesupdt=120;
float y=-0.1;
void updatetextures();

void map::update(){
	texturesupdt++;
	if (texturesupdt>1){
		texturesupdt=0;
		y+=0.1;
		updatetextures();
	}
	if (mustupdate==true){
		updatechunks();
		mustupdate=false;
	
	}
	float turnspeed=1.f;
	float movespeed=.2f;
	UpdateCameraPro(&camera,
		(Vector3){
			IsKeyDown(KEY_W)*movespeed - IsKeyDown(KEY_S)*movespeed,
			IsKeyDown(KEY_D)*movespeed - IsKeyDown(KEY_A)*movespeed,
			IsKeyDown(KEY_SPACE)*movespeed - IsKeyDown(KEY_LEFT_CONTROL)*movespeed
		},
		(Vector3){
			IsKeyDown(KEY_RIGHT)*turnspeed - IsKeyDown(KEY_LEFT)*turnspeed,
			IsKeyDown(KEY_DOWN)*turnspeed - IsKeyDown(KEY_UP)*turnspeed,
			0.0f
		},
		0
	);
	player.x=camera.position.x;
	player.y=camera.position.y;
	player.z=camera.position.z;
}
void map::close(){
	
	savebmp("tilemap.bmp",(unsigned char*)tilemaptx.data,
	tilemaptx.format==PIXELFORMAT_UNCOMPRESSED_GRAYSCALE?tilemaptx.width>>2:tilemaptx.width
	,tilemaptx.height);
}
// simplex og: 219'433'052 cycles on 1024x1024
// fastnoise lite:
Shader tilegen = LoadShader(0, "res/shaders/tilegen.fs");
#define TESTGEN

#include"biome.h"
inline unsigned char gentile(int x, int z,unsigned char mask,unsigned char population){
	
#ifdef TESTGEN
	float fx=x; float fz=z;
	float heat=(heatnoise.GetNoise(fx,fz));
	float humidity=(humiditynoise.GetNoise(fx,fz));
	float pop=(populationnoise.GetNoise(fx,fz));
	if (humidity>0.49f){
		if (humidity>0.61f){
			if (heat<-0.49f){
				return 7;
			}
			return 255;
		}
		return 2;
	}
	if (heat<-0.49f){
		return 8;
	}
	if (heat>0.49f){
		if (heat>0.58f){
			if (heat>0.76f){
				return 254;
			}
			return 5;
		}
		return 4;
	}
	return 1;
#else
	return  genbiome[mask](x,z,population);
#endif
}
void map::updatechunks() {
	int chunks = size / 64;
	long start = __rdtsc();
	long genstart = __rdtsc();
	for (int cz = 0; cz < chunks; cz++) {
		for (int cx = 0; cx < chunks; cx++) {
			
			float fx=cx; float fz=cz;
			float heat=(heatnoise.GetNoise(fx,fz));
			float humidity=(humiditynoise.GetNoise(fx,fz));
			float population=(populationnoise.GetNoise(fx,fz));
			unsigned char mask =
				((unsigned char)(((heat + 1.0f) * 2.0f) > 3 ? 3 : (unsigned char)((heat + 1.0f) * 2.0f)) << 4) |
				((unsigned char)(((humidity + 1.0f) * 2.0f) > 3 ? 3 : (unsigned char)((humidity + 1.0f) * 2.0f)) << 2);
	
			for (int z = 0; z < 64; z++) {
				int zidx = (cz << 6) + z;
				unsigned char tile = gentile((cx << 6), zidx,mask,population);
				for (int x = 0; x < 64; x++) {
					
					int xidx = (cx << 6) + x;
					tile = gentile(xidx, zidx,mask,population);
					
					((unsigned char*)tilemaptx.data)[zidx * size + xidx] = tile;
					tiles[zidx * size + xidx] = tile;
				}
			}
		}
	}
	long genend = __rdtsc();
	
	long setstart = __rdtsc();
	long setend = __rdtsc();
	long end = __rdtsc();
	
	//tilemaptx=LoadImage("res/images/worldtest.png");
	tilemaptx.format=PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
	//tilemaptx.width>>=2;
	if (tilemaptx.format==PIXELFORMAT_UNCOMPRESSED_GRAYSCALE){
		tilemaptx.width<<=2;
	}
	tilemapp=LoadTextureFromImage(tilemaptx);
	//SetTextureFilter(tilemapp, TEXTURE_FILTER_POINT);
	UpdateTexture(tilemapp, tilemaptx.data);
	error("cycles: %s", FORMAT_NUM(end - start));
	error("terrain gen cycles: %s\nset cycles: %s", FORMAT_NUM(genend - genstart), FORMAT_NUM(setend - setstart));

}

mainmenu::mainmenu(){}
mainmenu::~mainmenu(){}
void mainmenu::init(){}
void mainmenu::update(){}
void mainmenu::render(){}
void mainmenu::close(){}

#define G1 0.f
#define G2 0.1f
#define G3 0.98f
std::map<float, Color> grass={
	{G1,Color{30,50,18,255}},
	{G2,Color{55,126,60,255}},
	{G3,Color{111,255,122,255}},
};
#define W1 0.1f
#define W2 0.48f
#define W3 0.88f
#define W4 0.98f
std::map<float, Color> water={
	{W1,Color{82,130,130,255}},
	{W2,Color{162,255,255,255}},
	{W3,Color{255,255,255,255}},
	{W4,Color{162,255,255,255}},
	
};
// lava is basically same as water but flipped red
std::map<float, Color> lava={
	{W1,Color{130,26,0,255}},
	{W2,Color{223,66,0,255}},
	{W3,Color{255,180,180,255}},
	{W4,Color{223,66,0,255}},
	
};

float dragx=0.f;
float dragz=0.f;
void updatetextures(){
	dragx+=0.5f;
	dragz+=1.5f;
	// 32 x 32 per tile, 16 x 16 tiles
	long start = __rdtsc();
	texturenoise.SetFractalType(FastNoiseLite::FractalType::FractalType_None);
	texturenoise.SetFractalOctaves(0);
	texturenoise.SetFrequency(1.);
	texturenoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Value);
	texturenoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
	
	//grass position: 1,0
	int tileX = 1;
	int tileY = 0;
	int px = tileX * 32;
	int pz = tileY * 32;
	float previdx = texturenoise.GetNoise((float)0, y, (float)0);
	echo("previdx %f",previdx);
	bool genpixel=true;
	for (int x=0;x<32;x++){
		for (int z=0;z<32;z++){
			int xidx = px + x;
			int zidx = pz + z;

			int px = (zidx * sheet.width + xidx) * 4;
			
			float tidx =(genpixel?
				(texturenoise.GetNoise((float)x, y, (float)z+dragz))
				:previdx
			);
			float idx; 
			if (genpixel){
				idx = Lerp(previdx, tidx, 0.5f);
			}else{
				idx = tidx;
			}
			Color r;
			if (idx>=G3){
				r=grass[G3];
			}else{
				if (idx>=G2){
					r=ColorLerp(grass[G2], grass[G3], (idx-G2)/(G3-G2));
				}else{
					if (idx>=G1){
						r=ColorLerp(grass[G1], grass[G2], (idx-G1)/(G2-G1));
					}else{
						r=grass[G1];
					}
				}
			}

			((unsigned char*)sheet.data)[px+0] = r.r;
			((unsigned char*)sheet.data)[px+1] = r.g;
			((unsigned char*)sheet.data)[px+2] = r.b;
			((unsigned char*)sheet.data)[px+3] = 255;
			genpixel=!genpixel;
			
			previdx=idx;
		}
	}
	texturenoise.SetFractalType(FastNoiseLite::FractalType::FractalType_PingPong);
	texturenoise.SetFractalOctaves(2);
	texturenoise.SetFrequency(.22);
	texturenoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2S);
	texturenoise.SetFractalLacunarity(2.);
	texturenoise.SetFractalGain(0.5);
	texturenoise.SetFractalPingPongStrength(1.339);
	texturenoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
	tileX=15;
	tileY=15;
	px = tileX * 32;
	pz = tileY * 32;
	for (int x=0;x<32;x++){
		for (int z=0;z<32;z++){
			int xidx = px + x;
			int zidx = pz + z;

			int px = (zidx * sheet.width + xidx) * 4;
			float idx = texturenoise.GetNoise((float)x+dragx, y*4, (float)z+dragz);
			Color r;
			if (idx>=W4){
				r=water[W4];
			}else{
				if (idx>=W3){
					r=ColorLerp(water[W3], water[W4], (idx-W3)/(W4-W3));
				}else{
					if (idx>=W2){
						r=ColorLerp(water[W2], water[W3], (idx-W2)/(W3-W2));
					}else{
						if (idx>=W1){
							r=ColorLerp(water[W1], water[W2], (idx-W1)/(W2-W1));
						}else{
							r=water[W1];
						}
					}
				}
			}
			((unsigned char*)sheet.data)[px+0] = r.r;
			((unsigned char*)sheet.data)[px+1] = r.g;
			((unsigned char*)sheet.data)[px+2] = r.b;
			((unsigned char*)sheet.data)[px+3] = 255;
		}
	}
	texturenoise.SetFrequency(.1);
	texturenoise.SetFractalType(FastNoiseLite::FractalType::FractalType_PingPong);
	texturenoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
	tileX=14;
	tileY=15;
	px = tileX * 32;
	pz = tileY * 32;
	for (int x=0;x<32;x++){
		for (int z=0;z<32;z++){
			int xidx = px + x;
			int zidx = pz + z;

			int px = (zidx * sheet.width + xidx) * 4;
			float idx = texturenoise.GetNoise((float)x+dragx, y*4, (float)z+dragz);
			Color r;
			if (idx>=W4){
				r=lava[W4];
			}else{
				if (idx>=W3){
					r=ColorLerp(lava[W3], lava[W4], (idx-W3)/(W4-W3));
				}else{
					if (idx>=W2){
						r=ColorLerp(lava[W2], lava[W3], (idx-W2)/(W3-W2));
					}else{
						if (idx>=W1){
							r=ColorLerp(lava[W1], lava[W2], (idx-W1)/(W2-W1));
						}else{
							r=lava[W1];
						}
					}
				}
			}
			((unsigned char*)sheet.data)[px+0] = r.r;
			((unsigned char*)sheet.data)[px+1] = r.g;
			((unsigned char*)sheet.data)[px+2] = r.b;
			((unsigned char*)sheet.data)[px+3] = 255;
		}
	}
	long end = __rdtsc();
	error ("cycles: %s", FORMAT_NUM(end - start));
	
	UpdateTexture(tilesheet, sheet.data);
}