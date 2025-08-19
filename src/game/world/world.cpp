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
map::map(){
	// 1024, 2048, 4096, 8192, 16384
	size=1024;
	tiles=new unsigned char[size*size];
	tilemaptx.data=new unsigned char[size*size]{255}; 
	tilemaptx.format=PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
	tilemaptx.width=size;
	tilemaptx.height=size;
	mustupdate=true;
	data[0]=0;
	data[1]=0;
	data[2]=0;
}
Material tilemat;
Texture2D tilesheet;
int locTilemap;

Material basicmat;
	Texture2D tilemapp;
void map::init(){
	camera.position = (Vector3){0.0f, 2.0f, 0.0f};
	camera.target = (Vector3){8.0f, 0.0f, 8.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 80.0f;
	camera.projection = CAMERA_CUSTOM;
	skycolor=(Color){255,255,255,255};
	worldmodel=LoadModel("res/models/world.obj");
	
	tilemapp=LoadTextureFromImage(tilemaptx);
	tilemat=LoadMaterialDefault();
	tilemat.shader=LoadShader("res/shaders/tile.vs","res/shaders/tile.fs");
	rlSetClipPlanes(.01, 10000.0);

	tilesheet = LoadTexture("res/images/tilesheet.png");
	//SetTextureFilter(tilesheet, TEXTURE_FILTER_N);

	locTilemap  = GetShaderLocation(tilemat.shader, "tilemap");
	
	int sheetloc= GetShaderLocation(tilemat.shader, "tilesheet");
	SetShaderValueTexture(tilemat.shader, sheetloc, tilesheet);
	Matrix view = MatrixLookAt(camera.position, camera.target, camera.up);
	float aspect = (float)GetScreenWidth() / (float)GetScreenHeight();
	Matrix projection = MatrixPerspective(80, aspect, .01, 10000.0);
	Matrix model=Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	Matrix mvp = MatrixMultiply(model, MatrixMultiply(view, projection));
	
	
	int MVPloc = GetShaderLocation(tilemat.shader, "mvp");
	SetShaderValueMatrix(tilemat.shader,MVPloc, mvp);
	int modelloc = GetShaderLocation(tilemat.shader, "modelmat");
	SetShaderValueMatrix(tilemat.shader,modelloc, model);
	
	int sizeloc = GetShaderLocation(tilemat.shader, "size");
	float worldSizeV[2] = {(float)size, (float)size};
	SetShaderValue(tilemat.shader, sizeloc, worldSizeV, SHADER_UNIFORM_VEC2);
	
	int colsloc = GetShaderLocation(tilemat.shader, "cols");
	int cols = 16;
	SetShaderValue(tilemat.shader, colsloc, &cols, SHADER_UNIFORM_INT);

	echo("meshcount %i",worldmodel.meshCount);
	echo("materialcount %i",worldmodel.materialCount);
	basicmat=LoadMaterialDefault();
	basicmat.maps[MATERIAL_MAP_DIFFUSE].texture=tilesheet;
}

map::~map(){
}
void map::render(){
	BeginMode3D(camera);
	rlDisableBackfaceCulling();
	DrawMesh(worldmodel.meshes[0],basicmat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	DrawMesh(worldmodel.meshes[1],basicmat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	DrawMesh(worldmodel.meshes[3],basicmat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	DrawMesh(worldmodel.meshes[2],basicmat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	
	DrawCube((Vector3){0,0,0},1,1,1,WHITE);
	EndMode3D();
	std::string pos="X: "+std::to_string((int)player.x)+" Y: "+std::to_string((int)player.y)+" Z: "+std::to_string((int)player.z);
	DrawText(pos.c_str(), 0, 0, 32, WHITE);
	DrawText(std::to_string(GetFPS()).c_str(), 0, 36, 32, WHITE);
	BeginShaderMode(basicmat.shader);

                DrawRectangle(250 - 60, 90, 120, 60, RED);
                DrawRectangleGradientH(250 - 90, 170, 180, 130, MAROON, GOLD);
                DrawRectangleLines(250 - 40, 320, 80, 60, ORANGE);
	EndShaderMode();
	//DrawTexture(tilemapp,0,0,WHITE);
}

void map::update(){
	if (mustupdate==true){
		updatechunks();
		mustupdate=false;
	
	}
	float turnspeed=1.f;
	float movespeed=.6f;
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
	
	savebmp("tilemap.bmp",(unsigned char*)tilemaptx.data,tilemaptx.width,tilemaptx.height);
}
Shader tilegen = LoadShader(0, "res/shaders/tilegen.fs");
inline unsigned char gentile(int x, int z) {
	return SimplexNoise::noise(x * 0.01f, z * 0.01f)>0.5f ? 254 : 1;
	//return (x%2==0)?2:254;
}
void map::updatechunks() {
	int chunks = size / 64;
	long start = __rdtsc();
	long genstart = __rdtsc();
	for (int cz = 0; cz < chunks; cz++) {
		for (int cx = 0; cx < chunks; cx++) {
			for (int z = 0; z < 64; z++) {
				int zidx = (cz << 6) + z;
				unsigned char tile = gentile((cx << 6), zidx);
				for (int x = 0; x < 64; x++) {
					
					int xidx = (cx << 6) + x;
					tile = gentile(xidx, zidx);
					
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
	tilemaptx=LoadImage("res/images/worldtest.png");
	//tilemapTex.format=PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	//tilemapTex.width*=3;
	tilemapp=LoadTextureFromImage(tilemaptx);
	SetTextureFilter(tilemapp, TEXTURE_FILTER_POINT);
	SetShaderValueTexture(tilemat.shader, locTilemap, tilemapp);
	
	error("cycles: %s", FORMAT_NUM(end - start));
	error("terrain gen cycles: %s\nset cycles: %s", FORMAT_NUM(genend - genstart), FORMAT_NUM(setend - setstart));

}

mainmenu::mainmenu(){}
mainmenu::~mainmenu(){}
void mainmenu::init(){}
void mainmenu::update(){}
void mainmenu::render(){}
void mainmenu::close(){}


