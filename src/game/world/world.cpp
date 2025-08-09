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
		long start=__rdtsc();
		updatechunks();
		long end=__rdtsc();
		error("cycles: %i",end-start);
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
inline unsigned char gentile(int x, int z) {
    return tilaes[x + z * 1024];
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
    unsigned int vertexCount = 0; 

    success("updating chunks");
    delete[] static_cast<unsigned char*>(tilemap.data);
    tilemap.data = new unsigned char[size * size];
	
	RenderTexture target = LoadRenderTexture(tilees.width, tilees.height);


SetShaderValue(tileGenShader, GetShaderLocation(tileGenShader, "size"), &size, SHADER_UNIFORM_INT);


BeginTextureMode(target);
ClearBackground(BLACK);
BeginShaderMode(tileGenShader);
DrawRectangle(0, 0, tilees.width, tilees.height, WHITE);
EndShaderMode();
EndTextureMode();



Image tempImg = LoadImageFromTexture(target.texture);
memcpy(tilaes, tempImg.data, size * size);
UnloadImage(tempImg);
UnloadRenderTexture(target);
UnloadShader(tileGenShader);
    
    std::vector<std::vector<Run>> runs;
    runs.resize(size);
    for (unsigned short z = 0; z < size; ++z) {
        runs[z].reserve(size / 4 + 4);
        unsigned short x = 0;
        while (x < size) {
            unsigned char t = gentile(x, z);
            unsigned short start = x;
            ++x;
            while (x < size && gentile(x, z) == t) ++x;
            runs[z].push_back({ start, (unsigned short)(x - start), t });
        }
    }

    
    std::vector<Rect> prevRects; 
    prevRects.reserve(size / 4 + 4);
    std::vector<Rect> outRects; 
    outRects.reserve(size * size / 16);

    for (unsigned short z = 0; z < size; ++z) {
        
        
        std::unordered_map<unsigned int, unsigned int> prevByX; 
        prevByX.reserve(prevRects.size() * 2 + 4);
        for (unsigned int i = 0; i < prevRects.size(); ++i) {
            unsigned int key = ((unsigned int)prevRects[i].x << 16) | prevRects[i].width;
            prevByX[key] = i;
        }

        std::vector<Rect> currRects;
        currRects.reserve(runs[z].size());

        
        for (auto &r : runs[z]) {
            unsigned int key = ((unsigned int)r.x << 16) | r.width;
            auto it = prevByX.find(key);
            if (it != prevByX.end()) {
                Rect &cand = prevRects[it->second];
                if (cand.tile == r.tile) {
                    
                    cand.height += 1;
                    currRects.push_back(cand); 
                    
                    prevRects[it->second].tile = 0xFF; 
                    continue;
                }
            }
            
            currRects.push_back({ r.x, z, r.width, 1, r.tile });
        }

        
        for (auto &pr : prevRects) {
            if (pr.tile != 0xFF) { 
                outRects.push_back(pr);
            }
        }

        
        
        
        prevRects.swap(currRects);
    }

    
    for (auto &pr : prevRects) {
        if (pr.tile != 0xFF) outRects.push_back(pr);
    }

    
    vertices.reserve(outRects.size() * 12); 
    texcoords.reserve(outRects.size() * 8);
    normals.reserve(outRects.size() * 12);
    indices.reserve(outRects.size() * 6);

    
    for (auto &rec : outRects) {
        float h = (rec.tile < 250) ? 1.0f : 0.0f;
        float x0 = (float)rec.x * tileSize;
        float x1 = (float)(rec.x + rec.width) * tileSize;
        float z0 = (float)rec.z * tileSize;
        float z1 = (float)(rec.z + rec.height) * tileSize;

        
        float tu0, tv0, tu1, tv1;
        tileuv(rec.tile, tu0, tv0, tu1, tv1);
        vertices.insert(vertices.end(), { x0, h, z0,  x1, h, z0,  x1, h, z1,  x0, h, z1 });
        
        texcoords.insert(texcoords.end(), { tu0, tv0,  tu1, tv0,  tu1, tv1,  tu0, tv1 });
        for (int i = 0; i < 4; ++i) normals.insert(normals.end(), { 0.0f, 1.0f, 0.0f });

        
        indices.push_back((unsigned short)(vertexCount + 0));
        indices.push_back((unsigned short)(vertexCount + 2));
        indices.push_back((unsigned short)(vertexCount + 1));
        indices.push_back((unsigned short)(vertexCount + 0));
        indices.push_back((unsigned short)(vertexCount + 3));
        indices.push_back((unsigned short)(vertexCount + 2));
        vertexCount += 4;

        
        
        if (rec.z > 0) {
            for (unsigned short xx = 0; xx < rec.width; ++xx) {
                if (gentile(rec.x + xx, rec.z - 1) >= 250) {
                    float wx0 = (rec.x + xx) * tileSize;
                    float wx1 = (rec.x + xx + 1) * tileSize;
                    float wz = rec.z * tileSize;
                    tileuv(rec.tile, tu0, tv0, tu1, tv1);
                    vertices.insert(vertices.end(), { wx0, 0.0f, wz,  wx1, 0.0f, wz,  wx1, h, wz,  wx0, h, wz });
                    texcoords.insert(texcoords.end(), { tu0, tv1,  tu1, tv1,  tu1, tv0,  tu0, tv0 });
                    for (int i = 0; i < 4; ++i) normals.insert(normals.end(), { 0.0f, 0.0f, -1.0f });
                    indices.push_back((unsigned short)(vertexCount + 0));
                    indices.push_back((unsigned short)(vertexCount + 1));
                    indices.push_back((unsigned short)(vertexCount + 2));
                    indices.push_back((unsigned short)(vertexCount + 0));
                    indices.push_back((unsigned short)(vertexCount + 2));
                    indices.push_back((unsigned short)(vertexCount + 3));
                    vertexCount += 4;
                }
            }
        } else { 
            for (unsigned short xx = 0; xx < rec.width; ++xx) {
                float wx0 = (rec.x + xx) * tileSize;
                float wx1 = (rec.x + xx + 1) * tileSize;
                float wz = rec.z * tileSize;
                tileuv(rec.tile, tu0, tv0, tu1, tv1);
                vertices.insert(vertices.end(), { wx0, 0.0f, wz,  wx1, 0.0f, wz,  wx1, h, wz,  wx0, h, wz });
                texcoords.insert(texcoords.end(), { tu0, tv1,  tu1, tv1,  tu1, tv0,  tu0, tv0 });
                for (int i = 0; i < 4; ++i) normals.insert(normals.end(), { 0.0f, 0.0f, -1.0f });
                indices.push_back((unsigned short)(vertexCount + 0));
                indices.push_back((unsigned short)(vertexCount + 1));
                indices.push_back((unsigned short)(vertexCount + 2));
                indices.push_back((unsigned short)(vertexCount + 0));
                indices.push_back((unsigned short)(vertexCount + 2));
                indices.push_back((unsigned short)(vertexCount + 3));
                vertexCount += 4;
            }
        }

        
        if (rec.z + rec.height < size) {
            for (unsigned short xx = 0; xx < rec.width; ++xx) {
                if (gentile(rec.x + xx, rec.z + rec.height) >= 250) {
                    float wx0 = (rec.x + xx) * tileSize;
                    float wx1 = (rec.x + xx + 1) * tileSize;
                    float wz = (rec.z + rec.height) * tileSize;
                    tileuv(rec.tile, tu0, tv0, tu1, tv1);
                    vertices.insert(vertices.end(), { wx0, 0.0f, wz,  wx0, h, wz,  wx1, h, wz,  wx1, 0.0f, wz });
                    texcoords.insert(texcoords.end(), { tu0, tv1,  tu0, tv0,  tu1, tv0,  tu1, tv1 });
                    for (int i = 0; i < 4; ++i) normals.insert(normals.end(), { 0.0f, 0.0f, 1.0f });
                    indices.push_back((unsigned short)(vertexCount + 0));
                    indices.push_back((unsigned short)(vertexCount + 1));
                    indices.push_back((unsigned short)(vertexCount + 2));
                    indices.push_back((unsigned short)(vertexCount + 0));
                    indices.push_back((unsigned short)(vertexCount + 2));
                    indices.push_back((unsigned short)(vertexCount + 3));
                    vertexCount += 4;
                }
            }
        }

        
        if (rec.x > 0) {
            for (unsigned short zz = 0; zz < rec.height; ++zz) {
                if (gentile(rec.x - 1, rec.z + zz) >= 250) {
                    float wz0 = (rec.z + zz) * tileSize;
                    float wz1 = (rec.z + zz + 1) * tileSize;
                    float wx = rec.x * tileSize;
                    tileuv(rec.tile, tu0, tv0, tu1, tv1);
                    vertices.insert(vertices.end(), { wx, 0.0f, wz0,  wx, 0.0f, wz1,  wx, h, wz1,  wx, h, wz0 });
                    texcoords.insert(texcoords.end(), { tu0, tv1,  tu1, tv1,  tu1, tv0,  tu0, tv0 });
                    for (int i = 0; i < 4; ++i) normals.insert(normals.end(), { -1.0f, 0.0f, 0.0f });
                    indices.push_back((unsigned short)(vertexCount + 0));
                    indices.push_back((unsigned short)(vertexCount + 1));
                    indices.push_back((unsigned short)(vertexCount + 2));
                    indices.push_back((unsigned short)(vertexCount + 0));
                    indices.push_back((unsigned short)(vertexCount + 2));
                    indices.push_back((unsigned short)(vertexCount + 3));
                    vertexCount += 4;
                }
            }
        } else { 
            for (unsigned short zz = 0; zz < rec.height; ++zz) {
                float wz0 = (rec.z + zz) * tileSize;
                float wz1 = (rec.z + zz + 1) * tileSize;
                float wx = rec.x * tileSize;
                tileuv(rec.tile, tu0, tv0, tu1, tv1);
                vertices.insert(vertices.end(), { wx, 0.0f, wz0,  wx, 0.0f, wz1,  wx, h, wz1,  wx, h, wz0 });
                texcoords.insert(texcoords.end(), { tu0, tv1,  tu1, tv1,  tu1, tv0,  tu0, tv0 });
                for (int i = 0; i < 4; ++i) normals.insert(normals.end(), { -1.0f, 0.0f, 0.0f });
                indices.push_back((unsigned short)(vertexCount + 0));
                indices.push_back((unsigned short)(vertexCount + 1));
                indices.push_back((unsigned short)(vertexCount + 2));
                indices.push_back((unsigned short)(vertexCount + 0));
                indices.push_back((unsigned short)(vertexCount + 2));
                indices.push_back((unsigned short)(vertexCount + 3));
                vertexCount += 4;
            }
        }

        
        if (rec.x + rec.width < size) {
            for (unsigned short zz = 0; zz < rec.height; ++zz) {
                if (gentile(rec.x + rec.width, rec.z + zz) >= 250) {
                    float wz0 = (rec.z + zz) * tileSize;
                    float wz1 = (rec.z + zz + 1) * tileSize;
                    float wx = (rec.x + rec.width) * tileSize;
                    tileuv(rec.tile, tu0, tv0, tu1, tv1);
                    vertices.insert(vertices.end(), { wx, 0.0f, wz0,  wx, h, wz0,  wx, h, wz1,  wx, 0.0f, wz1 });
                    texcoords.insert(texcoords.end(), { tu0, tv1,  tu0, tv0,  tu1, tv0,  tu1, tv1 });
                    for (int i = 0; i < 4; ++i) normals.insert(normals.end(), { 1.0f, 0.0f, 0.0f });
                    indices.push_back((unsigned short)(vertexCount + 0));
                    indices.push_back((unsigned short)(vertexCount + 1));
                    indices.push_back((unsigned short)(vertexCount + 2));
                    indices.push_back((unsigned short)(vertexCount + 0));
                    indices.push_back((unsigned short)(vertexCount + 2));
                    indices.push_back((unsigned short)(vertexCount + 3));
                    vertexCount += 4;
                }
            }
        }
    }

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

    UploadMesh(&worldmodel, false);
    error("optimized vertices: %u", vertexCount);
}