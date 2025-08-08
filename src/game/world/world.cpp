#include "world.h"
#include "../../term.h"
#include <cmath>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <stack>
Mesh worldmodel;
void savebmp(const char* filename, unsigned char* data, int width, int height) {
	FILE* f = fopen(filename, "wb");
	if (!f) return;

	int row_padded = (width * 3 + 3) & (~3); // Each row is aligned to 4 bytes
	int data_size = row_padded * height;
	int file_size = 54 + data_size;

	// BMP Header
	uint8_t bmp_header[54] = {
		0x42, 0x4D,             // Signature "BM"
		0, 0, 0, 0,             // File size (we'll fill this in)
		0, 0, 0, 0,             // Reserved
		54, 0, 0, 0,            // Offset to pixel data
		40, 0, 0, 0,            // DIB header size
		0, 0, 0, 0,             // Width (we'll fill this in)
		0, 0, 0, 0,             // Height (we'll fill this in)
		1, 0,                   // Planes
		24, 0,                  // Bits per pixel
		0, 0, 0, 0,             // Compression (none)
		0, 0, 0, 0,             // Image size (can be 0 for uncompressed)
		0x13, 0x0B, 0, 0,       // Horizontal resolution (2835 ppm)
		0x13, 0x0B, 0, 0,       // Vertical resolution (2835 ppm)
		0, 0, 0, 0,             // Colors in color table
		0, 0, 0, 0              // Important colors
	};

	// Fill file size
	bmp_header[2] = (uint8_t)(file_size);
	bmp_header[3] = (uint8_t)(file_size >> 8);
	bmp_header[4] = (uint8_t)(file_size >> 16);
	bmp_header[5] = (uint8_t)(file_size >> 24);

	// Fill width
	bmp_header[18] = (uint8_t)(width);
	bmp_header[19] = (uint8_t)(width >> 8);
	bmp_header[20] = (uint8_t)(width >> 16);
	bmp_header[21] = (uint8_t)(width >> 24);

	// Fill height
	bmp_header[22] = (uint8_t)(height);
	bmp_header[23] = (uint8_t)(height >> 8);
	bmp_header[24] = (uint8_t)(height >> 16);
	bmp_header[25] = (uint8_t)(height >> 24);

	fwrite(bmp_header, 1, 54, f);

	// Write pixel data (BMP stores bottom-to-top)
	unsigned char* row = (unsigned char*)malloc(row_padded);
	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; x++) {
			int i = (y * width + x) * 3;
			row[x * 3 + 0] = data[i + 2]; // B
			row[x * 3 + 1] = data[i + 1]; // G
			row[x * 3 + 2] = data[i + 0]; // R
		}
		// Padding
		for (int p = width * 3; p < row_padded; p++)
			row[p] = 0;

		fwrite(row, 1, row_padded, f);
	}

	free(row);
	fclose(f);
}

Matrix *m;
map::map(){
	size=256; // must be divisible by 4
	tiles.resize(size*size);
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
	
}

map::~map(){
}
void map::render(){
	BeginMode3D(camera);
	DrawMesh(worldmodel,
	LoadMaterialDefault(),Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	DrawCube((Vector3){0,0,0},1,1,1,WHITE);
	EndMode3D();
	std::string pos="X: "+std::to_string((int)player.x)+" Y: "+std::to_string((int)player.y)+" Z: "+std::to_string((int)player.z);
	DrawText(pos.c_str(), 0, 0, 32, WHITE);
	DrawText(std::to_string(GetFPS()).c_str(), 0, 36, 32, WHITE);
}

void map::update(){
	if (mustupdate==true)updatechunks();
	UpdateCameraPro(&camera,
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
	);
	player.x=camera.position.x;
	player.y=camera.position.y;
	player.z=camera.position.z;
}
void map::close(){
	//save tilemap to bmp for test purposes
	error("%i %i",tilemap.width,tilemap.height);
	savebmp("tilemap.bmp",(unsigned char*)tilemap.data,tilemap.width,tilemap.height);
}

mainmenu::mainmenu(){}
mainmenu::~mainmenu(){}

void mainmenu::init(){}
void mainmenu::update(){}
void mainmenu::render(){}
void mainmenu::close(){}

inline unsigned char gentile(int x,int z){
	return x==0?255:1;
}

struct region{
	unsigned short x,z;
};
void bakeworldmodel(std::vector<std::vector<region>> borderGroups[256]);
void map::updatechunks() {
    std::vector<float> vertices;
    std::vector<float> texcoords;
    std::vector<float> normals;
    std::vector<unsigned short> indices;
    unsigned short vertexCount = 1;
    float tileSize = 4.0f;

	success("updating chunks");
	delete[] static_cast<unsigned char*>(tilemap.data);
	tilemap.data = new unsigned char[size * size];
	
	unsigned char prevtile = gentile(0,0);
	bool insidex = true;
	for (unsigned short x = 0; x < size; x++) {
		for (unsigned short z = 0; z < size; z++) {
			unsigned char tile = gentile(x, z);
			if (tile != prevtile) {
				vertexCount+=2;
				float height = tile<250?1.0f:0.0f;
				insidex=!insidex;
vertices.push_back((float)x * tileSize); vertices.push_back(height); vertices.push_back((float)z * tileSize);
vertices.push_back((float)x * tileSize); vertices.push_back(height); vertices.push_back((float)(z + 1) * tileSize);
				if (!insidex) {
	indices.push_back(vertexCount-3);
	indices.push_back(vertexCount-2);
	indices.push_back(vertexCount-1);
	for (int n = 0; n < 4; ++n) {
		normals.push_back(0.0f); normals.push_back(1.0f); normals.push_back(0.0f);
	}
	indices.push_back(vertexCount-2);
	indices.push_back(vertexCount-1);
	indices.push_back(vertexCount);
				}
			}
			
			
			prevtile = tile;
		}
	}

	mustupdate = false;
	if(tilemat.maps[0].texture.id > 0)UnloadTexture(tilemat.maps[0].texture);
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

    UploadMesh(&worldmodel, false);
	
}
