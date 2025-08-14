#include "world.h"
#include "../../term.h"
#include <cmath>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <stack>
#include <unordered_map>
Mesh* worldmodel;
Image tilees={0,0,0,0};
Material generator;
unsigned char* tilaes;

Matrix *m;
map::map(){
	size=1024; 
	tiles.resize(size*size);
	tilees.data=new unsigned char[size*size]; 
	tilaes=new unsigned char[size*size];
	tilees.format=PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	tilees.width=size;
	tilees.height=size>>2;
	mustupdate=true;
	tilemap.format=PIXELFORMAT_UNCOMPRESSED_R8G8B8;
	tilemap.width=tilemap.height=size;
	tilemap.data=new unsigned char[tilemap.width*tilemap.height*3];
	data[0]=0;
	data[1]=0;
	data[2]=0;
	m=new Matrix[size*size];
	for (int x=0;x<size;x++) for (int z=0;z<size;z++){
		m[x+z*size]=Matrix{
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			(float)x*4.0f,(float)z*4.0f,0.0f,1.0f
		};
	};
}
Material tilemat;
void map::init(){
	camera.position = (Vector3){0.0f, 0.0f, 0.0f};
	camera.target = (Vector3){8.0f, 0.0f, 8.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 80.0f;
	camera.projection = CAMERA_CUSTOM;

	tilemat=LoadMaterialDefault();
	tilemat.shader=LoadShader("res/shaders/tile.vs","res/shaders/tile.fs");
	
}

map::~map(){
}
void map::render(){
	BeginMode3D(camera);
	DrawModel(worldmodel,(Vector3){0,0,0},1.0f,WHITE);
	DrawCube((Vector3){0,0,0},1,1,1,WHITE);
	EndMode3D();
	std::string pos="X: "+std::to_string((int)player.x)+" Y: "+std::to_string((int)player.y)+" Z: "+std::to_string((int)player.z);
	DrawText(pos.c_str(), 0, 0, 32, WHITE);
	DrawText(std::to_string(GetFPS()).c_str(), 0, 36, 32, WHITE);
}

void map::update(){
	if (mustupdate==true){
		updatechunks();
		mustupdate=false;
	
	}
	/*UpdateCameraPro(&camera,
		(Vector3){
			IsKeyDown(KEY_W)*0.5f - IsKeyDown(KEY_S)*0.5f,
			IsKeyDown(KEY_D)*0.5f - IsKeyDown(KEY_A)*0.5f,
			IsKeyDown(KEY_SPACE)*0.5f - IsKeyDown(KEY_LEFT_CONTROL)*0.5f
		},
		(Vector3){
			IsKeyDown(KEY_RIGHT)*1.5f - IsKeyDown(KEY_LEFT)*1.5f,
			IsKeyDown(KEY_DOWN)*1.5f - IsKeyDown(KEY_UP)*1.5f,
			0.0f
		},
		0
	);*/
	float turnspeed=1.f;
	float movespeed=1.f;
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
	
	error("%i %i",tilemap.width,tilemap.height);
	savebmp("tilemap.bmp",(unsigned char*)tilemap.data,tilemap.width,tilemap.height);
}

mainmenu::mainmenu(){}
mainmenu::~mainmenu(){}

void mainmenu::init(){}
void mainmenu::update(){}
void mainmenu::render(){}
void mainmenu::close(){}

Shader tileGenShader = LoadShader(0, "res/shaders/tilegen.fs");
#include "../../libs/simplex.h"
inline unsigned char gentile(int x, int z) {
    //return SimplexNoise::noise(x * 0.01f, z * 0.01f)>0.5f ? 254 : 1;
	return (x%32==0);
}
/*
void map::updatechunks() {

    float tileSize = 1.0f;
    unsigned int vertexCount = 0;

    success("updating chunks");

    long start=__rdtsc();
	int chunks=size/64;
	int chunkssizeof=(sizeof(Mesh)*chunks);
	worldmodel.meshes=(Mesh*)RL_MALLOC(chunkssizeof*chunkssizeof);
	for (int cx = 0; cx < chunks; cx++) {
		for (int cz = 0; cz < chunks; cz++) {
		int chunkindex=cx*chunks+cz;
		std::vector<float> vertices;
		std::vector<float> texcoords;
		std::vector<float> normals;
		std::vector<unsigned short> indices;
		
		for (int z = 0; z < 64; z++) {
			unsigned char prev = gentile(0, z);
			bool liquid = prev >= 250;
			
			// Start of strip - add first vertices
			vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(z*tileSize);
			vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back((z+1)*tileSize);
			texcoords.push_back(0.0f);texcoords.push_back(0.0f);
			texcoords.push_back(0.0f);texcoords.push_back(1.0f);
			normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
			normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
			
			if (!liquid) {
				vertices.push_back(0.0f);vertices.push_back(1.0f);vertices.push_back(z*tileSize);
				vertices.push_back(0.0f);vertices.push_back(1.0f);vertices.push_back((z+1)*tileSize);
				texcoords.push_back(0.0f);texcoords.push_back(0.0f);
				texcoords.push_back(0.0f);texcoords.push_back(1.0f);
				normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
				normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
				vertexCount += 4;
			} else {
				vertexCount += 2;
			}
			
			for (int x = 1; x < 64; x++) {
				unsigned char tile = gentile(x, z);
				bool currentLiquid = tile >= 250;
				
				if (tile != prev || currentLiquid != liquid) {
					// End current strip
					vertices.push_back(x*tileSize);vertices.push_back(0.0f);vertices.push_back(z*tileSize);
					vertices.push_back(x*tileSize);vertices.push_back(0.0f);vertices.push_back((z+1)*tileSize);
					texcoords.push_back(1.0f);texcoords.push_back(0.0f);
					texcoords.push_back(1.0f);texcoords.push_back(1.0f);
					normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
					normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
					
					if (!liquid) {
						vertices.push_back(x*tileSize);vertices.push_back(1.0f);vertices.push_back(z*tileSize);
						vertices.push_back(x*tileSize);vertices.push_back(1.0f);vertices.push_back((z+1)*tileSize);
						texcoords.push_back(1.0f);texcoords.push_back(0.0f);
						texcoords.push_back(1.0f);texcoords.push_back(1.0f);
						normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
						normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
					}
					
					// Generate indices for previous strip
					if (liquid) {
						indices.push_back((unsigned short)(vertexCount - 2));
						indices.push_back((unsigned short)(vertexCount));
						indices.push_back((unsigned short)(vertexCount - 1));
						
						indices.push_back((unsigned short)(vertexCount - 1));
						indices.push_back((unsigned short)(vertexCount));
						indices.push_back((unsigned short)(vertexCount + 1));
						vertexCount += 2;
					} else {
						indices.push_back((unsigned short)(vertexCount - 4));
						indices.push_back((unsigned short)(vertexCount));
						indices.push_back((unsigned short)(vertexCount - 2));
						
						indices.push_back((unsigned short)(vertexCount - 2));
						indices.push_back((unsigned short)(vertexCount));
						indices.push_back((unsigned short)(vertexCount + 2));
						
						indices.push_back((unsigned short)(vertexCount - 3));
						indices.push_back((unsigned short)(vertexCount - 1));
						indices.push_back((unsigned short)(vertexCount + 1));
						
						indices.push_back((unsigned short)(vertexCount - 1));
						indices.push_back((unsigned short)(vertexCount + 1));
						indices.push_back((unsigned short)(vertexCount + 3));
						vertexCount += 4;
					}
					
					// Start new strip
					vertices.push_back(x*tileSize);vertices.push_back(0.0f);vertices.push_back(z*tileSize);
					vertices.push_back(x*tileSize);vertices.push_back(0.0f);vertices.push_back((z+1)*tileSize);
					texcoords.push_back(0.0f);texcoords.push_back(0.0f);
					texcoords.push_back(0.0f);texcoords.push_back(1.0f);
					normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
					normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
					
					if (!currentLiquid) {
						vertices.push_back(x*tileSize);vertices.push_back(1.0f);vertices.push_back(z*tileSize);
						vertices.push_back(x*tileSize);vertices.push_back(1.0f);vertices.push_back((z+1)*tileSize);
						texcoords.push_back(0.0f);texcoords.push_back(0.0f);
						texcoords.push_back(0.0f);texcoords.push_back(1.0f);
						normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
						normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
						vertexCount += 4;
					} else {
						vertexCount += 2;
					}
					
					liquid = currentLiquid;
				}
				prev = tile;
			}
			
			// End final strip for this row
			vertices.push_back(size*tileSize);vertices.push_back(0.0f);vertices.push_back(z*tileSize);
			vertices.push_back(size*tileSize);vertices.push_back(0.0f);vertices.push_back((z+1)*tileSize);
			texcoords.push_back(1.0f);texcoords.push_back(0.0f);
			texcoords.push_back(1.0f);texcoords.push_back(1.0f);
			normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
			normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
			
			if (!liquid) {
				vertices.push_back(size*tileSize);vertices.push_back(1.0f);vertices.push_back(z*tileSize);
				vertices.push_back(size*tileSize);vertices.push_back(1.0f);vertices.push_back((z+1)*tileSize);
				texcoords.push_back(1.0f);texcoords.push_back(0.0f);
				texcoords.push_back(1.0f);texcoords.push_back(1.0f);
				normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
				normals.push_back(0.0f);normals.push_back(1.0f);normals.push_back(0.0f);
			}
			
			// Final indices for this row
			if (liquid) {
				indices.push_back((unsigned short)(vertexCount - 2));
				indices.push_back((unsigned short)vertexCount);
				indices.push_back((unsigned short)(vertexCount - 1));
				
				indices.push_back((unsigned short)(vertexCount - 1));
				indices.push_back((unsigned short)vertexCount);
				indices.push_back((unsigned short)(vertexCount + 1));
				vertexCount += 2;
			} else {
				indices.push_back((unsigned short)(vertexCount - 4));
				indices.push_back((unsigned short)vertexCount);
				indices.push_back((unsigned short)(vertexCount - 2));
				
				indices.push_back((unsigned short)(vertexCount - 2));
				indices.push_back((unsigned short)vertexCount);
				indices.push_back((unsigned short)(vertexCount + 2));
				
				indices.push_back((unsigned short)(vertexCount - 3));
				indices.push_back((unsigned short)(vertexCount - 1));
				indices.push_back((unsigned short)(vertexCount + 1));
				
				indices.push_back((unsigned short)(vertexCount - 1));
				indices.push_back((unsigned short)(vertexCount + 1));
				indices.push_back((unsigned short)(vertexCount + 3));
				vertexCount += 4;
			}
		}
		
		long end=__rdtsc();
		error("generation cycles: %i",end-start);
		
		mustupdate = false;
		if (tilemat.maps[0].texture.id > 0) UnloadTexture(tilemat.maps[0].texture);
		tilemat.maps[0].texture = LoadTextureFromImage(tilemap);

		if (worldmodel.meshes[chunkindex].vertices) RL_FREE(worldmodel.meshes[chunkindex].vertices);
		if (worldmodel.meshes[chunkindex].texcoords) RL_FREE(worldmodel.meshes[chunkindex].texcoords);
		if (worldmodel.meshes[chunkindex].normals) RL_FREE(worldmodel.meshes[chunkindex].normals);
		if (worldmodel.meshes[chunkindex].indices) RL_FREE(worldmodel.meshes[chunkindex].indices);

		worldmodel.meshes[chunkindex].vertexCount = (int)(vertices.size() / 3);
		worldmodel.meshes[chunkindex].triangleCount = (int)(indices.size() / 3);
		worldmodel.meshes[chunkindex].vertices = (float*)RL_MALLOC(vertices.size() * sizeof(float));
		worldmodel.meshes[chunkindex].texcoords = (float*)RL_MALLOC(texcoords.size() * sizeof(float));
		worldmodel.meshes[chunkindex].normals = (float*)RL_MALLOC(normals.size() * sizeof(float));
		worldmodel.meshes[chunkindex].indices = (unsigned short*)RL_MALLOC(indices.size() * sizeof(unsigned short));

		memcpy(worldmodel.meshes[chunkindex].vertices, vertices.data(), vertices.size() * sizeof(float));
		memcpy(worldmodel.meshes[chunkindex].texcoords, texcoords.data(), texcoords.size() * sizeof(float));
		memcpy(worldmodel.meshes[chunkindex].normals, normals.data(), normals.size() * sizeof(float));
		memcpy(worldmodel.meshes[chunkindex].indices, indices.data(), indices.size() * sizeof(unsigned short));
		
	    UploadMesh(&worldmodel.meshes[chunkindex], false);
	}}
    long end=__rdtsc();
    error("mesh assignment cycles: %lli",end-start);
}*/

// new code i made when on market
void map::updatechunks(){
	int chunks=size/64;
	int totalchunks=(size/64)*(size/64);

	for (int cz=0;cz<chunks;cz++) for (int cx=0;cx<chunks;cx++){
		std::vector<float> normals;
		std::vector<float> vertices;
		std::vector<short> indices;
		unsigned short vertexes=0;
		for (int z=0;z<64;z++){
			int zidx=(cz<<6)+z;
			unsigned char prevtile=gentile((cx<<6),zidx);
			bool terrain=prevtile<250;
// add vertices
vertices.push_back((cx<<6)); vertices.push_back(0); vertices.push_back(zidx); // terrain: idx-7		!terrain: idx-3
vertices.push_back((cx<<6)); vertices.push_back(0); vertices.push_back(zidx+1); // terrain: idx-6	!terrain: idx-2
			vertexes+=2;
if (terrain){
vertices.push_back((cx<<6)); vertices.push_back(1); vertices.push_back(zidx); // terrain: idx-5
vertices.push_back((cx<<6)); vertices.push_back(1); vertices.push_back(zidx+1); // terrain: idx-4
			vertexes+=2;
}
			for (int x=0;x>64;x++){
				int xidx=(cx<<6)+x;
				unsigned char tile=gentile(xidx,zidx);
				if (tile!=prevtile){
// add vertices
vertices.push_back((cx<<6)); vertices.push_back(0); vertices.push_back(zidx); // terrain: idx-3		!terrain: idx-1
vertices.push_back((cx<<6)); vertices.push_back(0); vertices.push_back(zidx+1); // terrain: idx-2	!terrain: idx
			vertexes+=2;
if (terrain){
vertices.push_back((cx<<6)); vertices.push_back(1); vertices.push_back(zidx); // terrain: idx-1		
vertices.push_back((cx<<6)); vertices.push_back(1); vertices.push_back(zidx+1); // terrain: idx		
			vertexes+=2;
}

// add indices
if (terrain){
	// top
indices.push_back( vertexes-5 ); indices.push_back( vertexes-4 ); indices.push_back( vertexes-1	); 
indices.push_back( vertexes-4 ); indices.push_back( vertexes-1 ); indices.push_back( vertexes	); 


	// x+
indices.push_back( vertexes-3 ); indices.push_back( vertexes-2 ); indices.push_back( vertexes-1	);
indices.push_back( vertexes-2 ); indices.push_back( vertexes-1 ); indices.push_back( vertexes	);

	// x-
indices.push_back( vertexes-7 ); indices.push_back( vertexes-6 ); indices.push_back( vertexes-5	);
indices.push_back( vertexes-6 ); indices.push_back( vertexes-5 ); indices.push_back( vertexes-4	);

	// y+
indices.push_back( vertexes-6 ); indices.push_back( vertexes-2 ); indices.push_back( vertexes-4	);
indices.push_back( vertexes-4 ); indices.push_back( vertexes   ); indices.push_back( vertexes-6	);

	// y-
indices.push_back( vertexes-7 ); indices.push_back( vertexes-3 ); indices.push_back( vertexes-1	);
indices.push_back( vertexes-5 ); indices.push_back( vertexes-1 ); indices.push_back( vertexes-3	);

}else{
	// top
indices.push_back( vertexes-3 ); indices.push_back( vertexes-2 ); indices.push_back( vertexes-1	); 
indices.push_back( vertexes-2 ); indices.push_back( vertexes-1 ); indices.push_back( vertexes	);
}

// add vertices
vertices.push_back((cx<<6)); vertices.push_back(0); vertices.push_back(zidx);
vertices.push_back((cx<<6)); vertices.push_back(0); vertices.push_back(zidx);
			vertexes+=2;
if (terrain){
vertices.push_back((cx<<6)); vertices.push_back(0); vertices.push_back(zidx);
vertices.push_back((cx<<6)); vertices.push_back(0); vertices.push_back(zidx);
			vertexes+=2;
}				}
				prevtile=tile;
			}
		}
	}
}
