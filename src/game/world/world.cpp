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
char daytime=3;
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
	skytexture=LoadTextureFromImage(skybox);
	
}

map::~map(){
}
int flip=0;
float playerrotx=0;
float playerroty=0;
void map::render(){
	Matrix view = MatrixLookAt(camera.position, camera.target, camera.up);
	float aspect = (float)GetScreenWidth() / (float)GetScreenHeight();
	Matrix projection = MatrixPerspective(80, aspect, .5, 4000.0);
	Matrix mvp = MatrixMultiply(model, MatrixMultiply(view, projection));
	int cols = 16;
	flip = 0;
	
    // SKYBOX RENDERING - Before BeginMode3D
    // Calculate skybox offset based on player rotation
    float skyoffx = (playerrotx); // Adjust multiplier as needed
    float skyoffy = (playerroty); // Adjust multiplier as needed
    
    // Wrap the offsets to prevent going out of texture bounds
    skyoffx = fmod(skyoffx, skytexture.width);
    skyoffy = fmod(skyoffy, skytexture.height);
    if (skyoffx < 0) skyoffx += skytexture.width;
    if (skyoffy < 0) skyoffy += skytexture.height;
    
    // Draw skybox texture covering the entire screen
    Rectangle srcrec = {
        skyoffx, 
        skyoffy, 
        (float)skytexture.width/(GetScreenWidth()/200), 
        (float)skytexture.height
    };
    Rectangle dstrec = {
        0, 
        0, 
        (float)GetScreenWidth(), 
        (float)GetScreenHeight()
    };
	DrawTexturePro(skytexture, srcrec, dstrec, Vector2{0, 0}, 0, WHITE);
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
	if (texturesupdt>120){
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
	float camx=
			IsKeyDown(KEY_RIGHT)*turnspeed - IsKeyDown(KEY_LEFT)*turnspeed;
	float camy=
			IsKeyDown(KEY_DOWN)*turnspeed - IsKeyDown(KEY_UP)*turnspeed;
	UpdateCameraPro(&camera,
		(Vector3){
			IsKeyDown(KEY_W)*movespeed - IsKeyDown(KEY_S)*movespeed,
			IsKeyDown(KEY_D)*movespeed - IsKeyDown(KEY_A)*movespeed,
			IsKeyDown(KEY_SPACE)*movespeed - IsKeyDown(KEY_LEFT_CONTROL)*movespeed
		},
		(Vector3){
			camx, 
			camy,
			0.0f
			
		},
		0
	);
	playerrotx+=camx*2;
	playerroty+=camy*2;
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
/*
var daynight=[
	["797979","1c1c1c","1e0015"],#8am
	["c0c0c0","404040","006e5a"],#12pm
	["f5f5f5","5d5d5d","408e8b"],#16pm
	["8c8c8c","474747","5c2e00"],#20pm
	["797979","1c1c1c","180000"],#0am
	["181818","1c1c1c","000000"],#4am
	]
*/
#define SKY1 0.0f
#define SKY2 0.8f
#define SKY3 0.1f
std::map<float, Color> daynight[6] = {
	{{SKY1,Color{121,121,121,255}},{SKY2,Color{28,28,28,255}},{SKY3,Color{30,0,21,255}}},
	{{SKY1,Color{192,192,192,255}},{SKY2,Color{64,64,64,255}},{SKY3,Color{0,110,90,255}}},
	{{SKY1,Color{245,245,245,255}},{SKY2,Color{93,93,93,255}},{SKY3,Color{64,142,139,255}}},
	{{SKY1,Color{140,140,140,255}},{SKY2,Color{71,71,71,255}},{SKY3,Color{92,46,0,255}}},
	{{SKY1,Color{121,121,121,255}},{SKY2,Color{28,28,28,255}},{SKY3,Color{24,0,0,255}}},
	{{SKY1,Color{24,24,24,255}},{SKY2,Color{28,28,28,255}},{SKY3,Color{0,0,0,255}}}
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
	// skybox
	
	texturenoise.SetFrequency(0.04);
	texturenoise.SetFractalOctaves(8);
	for (int x = 0; x < 200; x++) {
		for (int z = 0; z < 100; z++) {
			float u = (float)x / 200.0f;
			float v = (float)z / 100.0f;
			
			float x_coord = cosf(u * 2.0f * 3.14159f) * 32.0f;
			float z_coord = sinf(u * 2.0f * 3.14159f) * 32.0f;
			float y_coord = cosf(v * 2.0f * 3.14159f) * 32.0f;
			float w_coord = sinf(v * 2.0f * 3.14159f) * 32.0f;
			
			float idx = texturenoise.GetNoise(x_coord / 4, y * 2 + y_coord, z_coord * 2, w_coord);
			Color r;
			
			if (idx >= SKY2) {
				r = daynight[daytime][SKY1];
			}
			else if (idx >= SKY3) {
				float denom = (SKY2 - SKY3);
				float t = denom != 0.0f ? (idx - SKY3) / denom : 0.0f;
				if (t < 0.0f) t = 0.0f;
				if (t > 1.0f) t = 1.0f;
				r = ColorLerp(daynight[daytime][SKY3], daynight[daytime][SKY1], t);
			}
			else {
				r = daynight[daytime][SKY3];
			}
			
			int px = (z * skybox.width + x) * 3;
			((unsigned char*)skybox.data)[px + 0] = r.r;
			((unsigned char*)skybox.data)[px + 1] = r.g;
			((unsigned char*)skybox.data)[px + 2] = r.b;
		}
	}

	long end = __rdtsc();
	error ("cycles: %s", FORMAT_NUM(end - start));
	
	UpdateTexture(tilesheet, sheet.data);
	UpdateTexture(skytexture, skybox.data);
}