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
#define MAX_MATERIAL_MAPS              12
Meshw worldmodel;
Image tilees={0,0,0,0};
Material generator;
unsigned char* tilaes;

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
	drawgl33mesh(worldmodel,tilemat,(Matrix){1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1});
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
inline unsigned char gentile(int x, int z) {
    //return SimplexNoise::noise(x * 0.01f, z * 0.01f)>0.5f ? 254 : 1;
	return (x%32==0);
}
#define addvert(x,y,z) worldmodel.vertices[vertexes++]=x; worldmodel.vertices[vertexes++]=y; worldmodel.vertices[vertexes++]=z
#define addidx(i1,i2,i3) worldmodel.indices[indices++]=i1; worldmodel.indices[indices++]=i2; worldmodel.indices[indices++]=i3

void format_number(long long num, char* buffer);
#define FORMAT_NUM(n) ({ \
    static char buf[32]; \
    format_number(n, buf); \
    buf; \
})
void map::updatechunks(){
    int chunks = size / 64;
    int totalchunks = chunks * chunks;
    int totalvers = 0;
    long start = __rdtsc();
	worldmodel = {};
	
	int vertexes = 0;
	int indices = 0;
	worldmodel.vertices = new float[402653184];
	worldmodel.normals = new float[402653184];
	worldmodel.texcoords = new float[402653184];
	worldmodel.indices = new unsigned int[134217728];
	
	long genstart = __rdtsc();
    for (int cz = 0; cz < chunks; cz++) for (int cx = 0; cx < chunks; cx++){
		

        int chunkidx = (cz * chunks) + cx;
		auto addnormal = [&](float nx,float ny,float nz){
            worldmodel.normals[vertexes] = nx;
            worldmodel.normals[vertexes+1] = ny;
            worldmodel.normals[vertexes+2] = nz;
        };
        auto addtex = [&](int tile,float u,float v){
            int tx = tile % 16;
            int ty = tile / 16;
            float du = 1.0f/16.0f;
            float dv = 1.0f/16.0f;
            worldmodel.texcoords[(vertexes/3)*2]   = (tx+u)*du;
            worldmodel.texcoords[(vertexes/3)*2+1] = (ty+v)*dv;
        };
        for (int z = 0; z < 64; z++){
            int zidx = (cz << 6) + z;
            unsigned char prevtile = gentile((cx << 6), zidx);
            bool terrain = prevtile < 250;

            // initial vertices along z edge
            addvert((cx << 6), 0, zidx);
            addnormal(0,1,0);
            addtex(prevtile,0,0);
            addvert((cx << 6), 0, zidx+1);
            addnormal(0,1,0);
            addtex(prevtile,0,1);
            vertexes += 2;

            if (terrain){
                addvert((cx << 6), 1, zidx);
            	addnormal(0,1,0);
                addtex(prevtile,1,0);
                addvert((cx << 6), 1, zidx+1);
            	addnormal(0,1,0);
				addtex(prevtile,1,1);
                vertexes += 2;
            }

            for (int x = 0; x < 64; x++){
                int xidx = (cx << 6) + x;
                unsigned char tile = gentile(xidx, zidx);

                if (tile != prevtile){
                    // add edge vertices
                    addvert((cx << 6), 0, zidx);
            		addnormal(0,1,0);
					addtex(tile,0,0);
                    addvert((cx << 6), 0, zidx+1);
            		addnormal(0,1,0);
					addtex(tile,0,1);
                    vertexes += 2;

                    if (terrain){
                        addvert((cx << 6), 1, zidx);
            			addnormal(0,1,0);
						addtex(tile,1,0);
                        addvert((cx << 6), 1, zidx+1);
            			addnormal(0,1,0);
						addtex(tile,1,1);
                        vertexes += 2;
                    }

                    // add faces
                    if (terrain){
                        addidx(vertexes-5, vertexes-4, vertexes-1); // top
                        addidx(vertexes-4, vertexes-1, vertexes);
                        addidx(vertexes-3, vertexes-2, vertexes-1); // x+
                        addidx(vertexes-2, vertexes-1, vertexes);
                        addidx(vertexes-7, vertexes-6, vertexes-5); // x-
                        addidx(vertexes-6, vertexes-5, vertexes-4);
                        addidx(vertexes-6, vertexes-2, vertexes-4); // y+
                        addidx(vertexes-4, vertexes, vertexes-6);
                        addidx(vertexes-7, vertexes-3, vertexes-1); // y-
                        addidx(vertexes-5, vertexes-1, vertexes-3);
                    } else {
                        addidx(vertexes-3, vertexes-2, vertexes-1); // top only
                        addidx(vertexes-2, vertexes-1, vertexes);
                    }

                    // duplicate vertices for next iteration (old style)
                    addvert((cx << 6), 0, zidx);
            		addnormal(0,1,0);
					addtex(tile,0,0);
                    addvert((cx << 6), 0, zidx);
            		addnormal(0,1,0);
					addtex(tile,0,1);
                    vertexes += 2;

                    if (terrain){
                        addvert((cx << 6), 0, zidx);
            			addnormal(0,1,0);
						addtex(tile,1,0);
                        addvert((cx << 6), 0, zidx);
            			addnormal(0,1,0);
						addtex(tile,1,1);
                        vertexes += 2;
                    }
                }

                prevtile = tile;
            }
        }
    }

	long genend = __rdtsc();
	long setstart = __rdtsc();
	float* newVerts = new float[vertexes];
	std::memcpy(newVerts, worldmodel.vertices, vertexes * sizeof(float));
	delete[] worldmodel.vertices;
	worldmodel.vertices = newVerts;

	float* newNorms = new float[vertexes];
	std::memcpy(newNorms, worldmodel.normals, vertexes * sizeof(float));
	delete[] worldmodel.normals;
	worldmodel.normals = newNorms;

	float* newTex = new float[(vertexes/3)*2];
	std::memcpy(newTex, worldmodel.texcoords, (vertexes/3)*2 * sizeof(float));
	delete[] worldmodel.texcoords;
	worldmodel.texcoords = newTex;

	unsigned int* newIdx = new unsigned int[indices];
	std::memcpy(newIdx, worldmodel.indices, indices * sizeof(unsigned int));
	delete[] worldmodel.indices;
	worldmodel.indices = newIdx;

	// tell raylib how many
	worldmodel.vertexCount = vertexes/3;
	worldmodel.triangleCount = indices/3;
	
	
	worldmodel.vboId = new unsigned int[4];
	memset(worldmodel.vboId, 0, 4 * sizeof(unsigned int));

	long setend = __rdtsc();
	totalvers += vertexes/3;
    long end = __rdtsc();
	error("chunk indices: %s",FORMAT_NUM(indices));
	error("chunk vertices: %s",FORMAT_NUM(vertexes/3));
	error("cycles: %s\nvertices: %s",FORMAT_NUM(end - start),FORMAT_NUM(totalvers));
	error("gen cycles: %s\nset cycles: %s",FORMAT_NUM(genend - genstart),FORMAT_NUM(setend - setstart));
}

void drawgl33mesh(Meshw mesh, Material material, Matrix transform) {
	    rlEnableShader(material.shader.id);

    // Send required data to shader (matrices, values)
    //-----------------------------------------------------
    // Upload to shader material.colDiffuse
    if (material.shader.locs[SHADER_LOC_COLOR_DIFFUSE] != -1)
    {
        float values[4] = {
            (float)material.maps[MATERIAL_MAP_DIFFUSE].color.r/255.0f,
            (float)material.maps[MATERIAL_MAP_DIFFUSE].color.g/255.0f,
            (float)material.maps[MATERIAL_MAP_DIFFUSE].color.b/255.0f,
            (float)material.maps[MATERIAL_MAP_DIFFUSE].color.a/255.0f
        };

        rlSetUniform(material.shader.locs[SHADER_LOC_COLOR_DIFFUSE], values, SHADER_UNIFORM_VEC4, 1);
    }

    // Upload to shader material.colSpecular (if location available)
    if (material.shader.locs[SHADER_LOC_COLOR_SPECULAR] != -1)
    {
        float values[4] = {
            (float)material.maps[MATERIAL_MAP_SPECULAR].color.r/255.0f,
            (float)material.maps[MATERIAL_MAP_SPECULAR].color.g/255.0f,
            (float)material.maps[MATERIAL_MAP_SPECULAR].color.b/255.0f,
            (float)material.maps[MATERIAL_MAP_SPECULAR].color.a/255.0f
        };

        rlSetUniform(material.shader.locs[SHADER_LOC_COLOR_SPECULAR], values, SHADER_UNIFORM_VEC4, 1);
    }

    // Get a copy of current matrices to work with,
    // just in case stereo render is required, and we need to modify them
    // NOTE: At this point the modelview matrix just contains the view matrix (camera)
    // That's because BeginMode3D() sets it and there is no model-drawing function
    // that modifies it, all use rlPushMatrix() and rlPopMatrix()
    Matrix matModel = MatrixIdentity();
    Matrix matView = rlGetMatrixModelview();
    Matrix matModelView = MatrixIdentity();
    Matrix matProjection = rlGetMatrixProjection();

    // Upload view and projection matrices (if locations available)
    if (material.shader.locs[SHADER_LOC_MATRIX_VIEW] != -1) rlSetUniformMatrix(material.shader.locs[SHADER_LOC_MATRIX_VIEW], matView);
    if (material.shader.locs[SHADER_LOC_MATRIX_PROJECTION] != -1) rlSetUniformMatrix(material.shader.locs[SHADER_LOC_MATRIX_PROJECTION], matProjection);

    // Accumulate several model transformations:
    //    transform: model transformation provided (includes DrawModel() params combined with model.transform)
    //    rlGetMatrixTransform(): rlgl internal transform matrix due to push/pop matrix stack
    matModel = MatrixMultiply(transform, rlGetMatrixTransform());

    // Model transformation matrix is sent to shader uniform location: SHADER_LOC_MATRIX_MODEL
    if (material.shader.locs[SHADER_LOC_MATRIX_MODEL] != -1) rlSetUniformMatrix(material.shader.locs[SHADER_LOC_MATRIX_MODEL], matModel);

    // Get model-view matrix
    matModelView = MatrixMultiply(matModel, matView);

    // Upload model normal matrix (if locations available)
    if (material.shader.locs[SHADER_LOC_MATRIX_NORMAL] != -1) rlSetUniformMatrix(material.shader.locs[SHADER_LOC_MATRIX_NORMAL], MatrixTranspose(MatrixInvert(matModel)));


    //-----------------------------------------------------

    // Bind active texture maps (if available)
    for (int i = 0; i < MAX_MATERIAL_MAPS; i++)
    {
        if (material.maps[i].texture.id > 0)
        {
            // Select current shader texture slot
            rlActiveTextureSlot(i);

            // Enable texture for active slot
            if ((i == MATERIAL_MAP_IRRADIANCE) ||
                (i == MATERIAL_MAP_PREFILTER) ||
                (i == MATERIAL_MAP_CUBEMAP)) rlEnableTextureCubemap(material.maps[i].texture.id);
            else rlEnableTexture(material.maps[i].texture.id);

            rlSetUniform(material.shader.locs[SHADER_LOC_MAP_DIFFUSE + i], &i, SHADER_UNIFORM_INT, 1);
        }
    }

    // Try binding vertex array objects (VAO) or use VBOs if not possible
    // WARNING: UploadMesh() enables all vertex attributes available in mesh and sets default attribute values
    // for shader expected vertex attributes that are not provided by the mesh (i.e. colors)
    // This could be a dangerous approach because different meshes with different shaders can enable/disable some attributes
    if (!rlEnableVertexArray(mesh.vaoId))
    {
        // Bind mesh VBO data: vertex position (shader-location = 0)
        rlEnableVertexBuffer(mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION]);
        rlSetVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_POSITION], 3, RL_FLOAT, 0, 0, 0);
        rlEnableVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_POSITION]);

        // Bind mesh VBO data: vertex texcoords (shader-location = 1)
        rlEnableVertexBuffer(mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD]);
        rlSetVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_TEXCOORD01], 2, RL_FLOAT, 0, 0, 0);
        rlEnableVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_TEXCOORD01]);

        if (material.shader.locs[SHADER_LOC_VERTEX_NORMAL] != -1)
        {
            // Bind mesh VBO data: vertex normals (shader-location = 2)
            rlEnableVertexBuffer(mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL]);
            rlSetVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_NORMAL], 3, RL_FLOAT, 0, 0, 0);
            rlEnableVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_NORMAL]);
        }

        // Bind mesh VBO data: vertex colors (shader-location = 3, if available)
        if (material.shader.locs[SHADER_LOC_VERTEX_COLOR] != -1)
        {
            if (mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR] != 0)
            {
                rlEnableVertexBuffer(mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR]);
                rlSetVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_COLOR], 4, RL_UNSIGNED_BYTE, 1, 0, 0);
                rlEnableVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_COLOR]);
            }
            else
            {
                // Set default value for defined vertex attribute in shader but not provided by mesh
                // WARNING: It could result in GPU undefined behaviour
                float value[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                rlSetVertexAttributeDefault(material.shader.locs[SHADER_LOC_VERTEX_COLOR], value, SHADER_ATTRIB_VEC4, 4);
                rlDisableVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_COLOR]);
            }
        }

        // Bind mesh VBO data: vertex tangents (shader-location = 4, if available)
        if (material.shader.locs[SHADER_LOC_VERTEX_TANGENT] != -1)
        {
            rlEnableVertexBuffer(mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT]);
            rlSetVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_TANGENT], 4, RL_FLOAT, 0, 0, 0);
            rlEnableVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_TANGENT]);
        }

        // Bind mesh VBO data: vertex texcoords2 (shader-location = 5, if available)
        if (material.shader.locs[SHADER_LOC_VERTEX_TEXCOORD02] != -1)
        {
            rlEnableVertexBuffer(mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2]);
            rlSetVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_TEXCOORD02], 2, RL_FLOAT, 0, 0, 0);
            rlEnableVertexAttribute(material.shader.locs[SHADER_LOC_VERTEX_TEXCOORD02]);
        }

        if (mesh.indices != NULL) rlEnableVertexBufferElement(mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_INDICES]);
    }

    int eyeCount = 1;
    if (rlIsStereoRenderEnabled()) eyeCount = 2;

    for (int eye = 0; eye < eyeCount; eye++)
    {
        // Calculate model-view-projection matrix (MVP)
        Matrix matModelViewProjection = MatrixIdentity();
        if (eyeCount == 1) matModelViewProjection = MatrixMultiply(matModelView, matProjection);
        else
        {
            // Setup current eye viewport (half screen width)
            rlViewport(eye*rlGetFramebufferWidth()/2, 0, rlGetFramebufferWidth()/2, rlGetFramebufferHeight());
            matModelViewProjection = MatrixMultiply(MatrixMultiply(matModelView, rlGetMatrixViewOffsetStereo(eye)), rlGetMatrixProjectionStereo(eye));
        }

        // Send combined model-view-projection matrix to shader
        rlSetUniformMatrix(material.shader.locs[SHADER_LOC_MATRIX_MVP], matModelViewProjection);

        // Draw mesh
        if (mesh.indices != NULL) rlDrawVertexArrayElements(0, mesh.triangleCount*3, 0);
        else rlDrawVertexArray(0, mesh.vertexCount);
    }

    // Unbind all bound texture maps
    for (int i = 0; i < MAX_MATERIAL_MAPS; i++)
    {
        if (material.maps[i].texture.id > 0)
        {
            // Select current shader texture slot
            rlActiveTextureSlot(i);

            // Disable texture for active slot
            if ((i == MATERIAL_MAP_IRRADIANCE) ||
                (i == MATERIAL_MAP_PREFILTER) ||
                (i == MATERIAL_MAP_CUBEMAP)) rlDisableTextureCubemap();
            else rlDisableTexture();
        }
    }

    // Disable all possible vertex array objects (or VBOs)
    rlDisableVertexArray();
    rlDisableVertexBuffer();
    rlDisableVertexBufferElement();

    // Disable shader program
    rlDisableShader();

    // Restore rlgl internal modelview and projection matrices
    rlSetMatrixModelview(matView);
    rlSetMatrixProjection(matProjection);
}


void format_number(long long num, char* buffer) {
    char temp[32];
    sprintf(temp, "%lld", num);
    int len = strlen(temp);
    int pos = 0;
    
    for (int i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) {
            buffer[pos++] = '\'';
        }
        buffer[pos++] = temp[i];
    }
    buffer[pos] = '\0';
}
