#include "world.h"
#include <cmath>
#include <algorithm>
#include <cstring>
Mesh terrain1;//normal
Mesh terrain2;//water/lava
Mesh terrain3;//bridge
Mesh terrain4;//bridge

map::map(){
	size=16; // must be divisible by 4
	tiles.resize(size*size);
	mustupdate=true;
	tilemap.format=PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	tilemap.width=size>>2;
	tilemap.height=size;
	data[0]=0;
	data[1]=0;
	data[2]=0;

}
Material tilemat;
void map::init(){
	camera.position = (Vector3){0.0f, 0.0f, 0.0f};
	camera.target = (Vector3){8.0f, 0.0f, 8.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 80.0f;
	camera.projection = CAMERA_CUSTOM;

	terrain1 = LoadModel("res/models/t1.obj").meshes[0];
	terrain2 = LoadModel("res/models/t2.obj").meshes[0];
	terrain3 = LoadModel("res/models/t3.obj").meshes[0];
	terrain4 = LoadModel("res/models/t4.obj").meshes[0];
	tilemat.shader = LoadShader("res/shaders/tile.vs","res/shaders/tile.fs");

}

map::~map(){
}

void map::updateChunks(){}

Matrix *m = new Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
void map::render(){
	//set shader param 0 as texture
	tilemat.maps[0].texture = LoadTextureFromImage(tilemap);
	
	BeginMode3D(camera);
	DrawMeshInstanced(terrain1,tilemat,m,size*size);
	DrawCube((Vector3){0,0,0},1,1,1,WHITE);
	EndMode3D();
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
}

mainmenu::mainmenu(){}
mainmenu::~mainmenu(){}

void mainmenu::init(){}
void mainmenu::update(){}
void mainmenu::render(){}
void mainmenu::close(){}

void map::updatechunks() {
    delete[] static_cast<unsigned char*>(tilemap.data);
    tilemap.data = new unsigned char[size * size];

    unsigned char* daaa = static_cast<unsigned char*>(tilemap.data);

    for (int x = 0; x < size; x++) {
        for (int z = 0; z < size; z++) {
            if (x == 0) {
				data[2]++;
                tiles[x + z * size] = 255;
                daaa[x + z * size] = 255;
            } else {
				data[0]++;
                tiles[x + z * size] = 0;
                daaa[x + z * size] = 0;
            }
        }
    }
    mustupdate = false;

}
