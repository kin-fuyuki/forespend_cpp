#include "world.h"
#include "../../term.h"
#include <cmath>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <stack>
#include <unordered_map>
Mesh worldmodel;
Image tilees={0,0,0,0};
Material generator;
unsigned char* tilaes;
void savebmp(const char* filename, unsigned char* data, int width, int height) {
	FILE* f = fopen(filename, "wb");
	if (!f) return;

	int row_padded = (width * 3 + 3) & (~3); 
	int data_size = row_padded * height;
	int file_size = 54 + data_size;

	
	uint8_t bmp_header[54] = {
		0x42, 0x4D,             
		0, 0, 0, 0,             
		0, 0, 0, 0,             
		54, 0, 0, 0,            
		40, 0, 0, 0,            
		0, 0, 0, 0,             
		0, 0, 0, 0,             
		1, 0,                   
		24, 0,                  
		0, 0, 0, 0,             
		0, 0, 0, 0,             
		0x13, 0x0B, 0, 0,       
		0x13, 0x0B, 0, 0,       
		0, 0, 0, 0,             
		0, 0, 0, 0              
	};

	
	bmp_header[2] = (uint8_t)(file_size);
	bmp_header[3] = (uint8_t)(file_size >> 8);
	bmp_header[4] = (uint8_t)(file_size >> 16);
	bmp_header[5] = (uint8_t)(file_size >> 24);

	
	bmp_header[18] = (uint8_t)(width);
	bmp_header[19] = (uint8_t)(width >> 8);
	bmp_header[20] = (uint8_t)(width >> 16);
	bmp_header[21] = (uint8_t)(width >> 24);

	
	bmp_header[22] = (uint8_t)(height);
	bmp_header[23] = (uint8_t)(height >> 8);
	bmp_header[24] = (uint8_t)(height >> 16);
	bmp_header[25] = (uint8_t)(height >> 24);

	fwrite(bmp_header, 1, 54, f);

	
	unsigned char* row = (unsigned char*)malloc(row_padded);
	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; x++) {
			int i = (y * width + x) * 3;
			row[x * 3 + 0] = data[i + 2]; 
			row[x * 3 + 1] = data[i + 1]; 
			row[x * 3 + 2] = data[i + 0]; 
		}
		
		for (int p = width * 3; p < row_padded; p++)
			row[p] = 0;

		fwrite(row, 1, row_padded, f);
	}

	free(row);
	fclose(f);
}

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
	DrawMesh(worldmodel,
	tilemat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
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
#include "../../libs/simplex.h"
inline unsigned char gentile(int x, int z) {
    return SimplexNoise::noise(x * 0.01f, z * 0.01f)>0.5f ? 254 : 1;
}

inline void tileuv(unsigned char tile, float &u0, float &v0, float &u1, float &v1) {
    int tx = tile % 16;
    int ty = tile / 16;
    u0 = tx * (1.0f / 16.0f);
    v0 = ty * (1.0f / 16.0f);
    u1 = u0 + (1.0f / 16.0f);
    v1 = v0 + (1.0f / 16.0f);
}

struct Run {
    unsigned short x;
    unsigned short width;
    unsigned char tile;
};
struct Rect {
    unsigned short x;
    unsigned short z;
    unsigned short width;
    unsigned short height;
    unsigned char tile;
};

Shader tileGenShader = LoadShader(0, "res/shaders/tilegen.fs");
void map::updatechunks() {
    std::vector<float> vertices;
    std::vector<float> texcoords;
    std::vector<float> normals;
    std::vector<unsigned short> indices;

    float tileSize = 4.0f;
    unsigned int vertexCount = -1; 

    success("updating chunks");
//    delete[] static_cast<unsigned char*>(tilemap.data);
//    tilemap.data = new unsigned char[size * size];
	
//	RenderTexture target = LoadRenderTexture(tilees.width, tilees.height);



	long start=__rdtsc();
	unsigned char prev=gentile(0,0);
	unsigned char tile = gentile(0, 0);
	bool liquid=tile>=250;
// !liquid: vertexCount-7 , liquid: vertexCount-3
vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);
// !liquid: vertexCount-6 , liquid: vertexCount-2
vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(1.0f);
vertexCount += 2;
if (!liquid) {
// !liquid: vertexCount-5
vertices.push_back(0.0f);vertices.push_back(1.0f);vertices.push_back(0.0f);
// !liquid: vertexCount-4
vertices.push_back(0.0f);vertices.push_back(1.0f);vertices.push_back(1.0f);
vertexCount += 2;
}
	for (int z = 0; z < size; z++) {
		for (int x = 0; x < size; x++) {
			tile = gentile(0, 0);
			liquid=tile>=250;
			//end of strip
			if (tile != prev) {
// !liquid: vertexCount-3 , liquid: vertexCount-1
vertices.push_back((x+1)*tileSize);vertices.push_back(0.0f);vertices.push_back(z*tileSize);
// !liquid: vertexCount-2 , liquid: vertexCount
vertices.push_back((x+1)*tileSize);vertices.push_back(0.0f);vertices.push_back((z+1)*tileSize);
vertexCount += 2;
if (!liquid) {
// !liquid: vertexCount-1
vertices.push_back((x+1)*tileSize);vertices.push_back(1.0f);vertices.push_back(z*tileSize);
// !liquid: vertexCount
vertices.push_back((x+1)*tileSize);vertices.push_back(1.0f);vertices.push_back((z+1)*tileSize);
vertexCount += 2;
}
			//indices
			if (liquid) {
				// up triangle 1
				indices.push_back((unsigned short)(vertexCount - 3));
				indices.push_back((unsigned short)(vertexCount - 1));
				indices.push_back((unsigned short)(vertexCount - 2));
				// up triangle 2
				indices.push_back((unsigned short)(vertexCount - 2));
				indices.push_back((unsigned short)(vertexCount));
				indices.push_back((unsigned short)(vertexCount - 1));
			}else{
				// up triangle 1
				indices.push_back((unsigned short)(vertexCount - 5));
				indices.push_back((unsigned short)(vertexCount - 1));
				indices.push_back((unsigned short)(vertexCount - 4));
				// up triangle 2
				indices.push_back((unsigned short)(vertexCount - 4));
				indices.push_back((unsigned short)(vertexCount));
				indices.push_back((unsigned short)(vertexCount - 1));
				// sides
			}
			}
			
			prev = tile;
		}
	}
	long end=__rdtsc();
	error("generation cycles: %i",end-start);
	
	
	
    mustupdate = false;
    if (tilemat.maps[0].texture.id > 0) UnloadTexture(tilemat.maps[0].texture);
    tilemat.maps[0].texture = LoadTextureFromImage(tilemap);

    
    if (worldmodel.vertices) RL_FREE(worldmodel.vertices);
    if (worldmodel.texcoords) RL_FREE(worldmodel.texcoords);
    if (worldmodel.normals) RL_FREE(worldmodel.normals);
    if (worldmodel.indices) RL_FREE(worldmodel.indices);

    worldmodel.vertexCount = (int)(vertices.size() / 3);
    worldmodel.triangleCount = (int)(indices.size() / 3);
    worldmodel.vertices = (float*)RL_MALLOC(vertices.size() * sizeof(float));
    worldmodel.texcoords = (float*)RL_MALLOC(texcoords.size() * sizeof(float));
    worldmodel.normals = (float*)RL_MALLOC(normals.size() * sizeof(float));
    worldmodel.indices = (unsigned short*)RL_MALLOC(indices.size() * sizeof(unsigned short));

    memcpy(worldmodel.vertices, vertices.data(), vertices.size() * sizeof(float));
    memcpy(worldmodel.texcoords, texcoords.data(), texcoords.size() * sizeof(float));
    memcpy(worldmodel.normals, normals.data(), normals.size() * sizeof(float));
    memcpy(worldmodel.indices, indices.data(), indices.size() * sizeof(unsigned short));

		end=__rdtsc();
		error("mesh assignment cycles: %i",end-start);
    UploadMesh(&worldmodel, false);
    error("optimized vertices: %u", vertexCount);
}