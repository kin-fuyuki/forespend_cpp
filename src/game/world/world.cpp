#include "world.h"

map::map(){
	size=16;
	tiles.resize(size*size);
	updatechunks();
	updatechunkmesh();
}
map::~map(){}

void map::updateChunks(){}
void map::render(){
	DrawMesh(mesh,LoadMaterialDefault(),Matrix{1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1});
}
void map::init(){}
void map::update(){}
void map::close(){}

mainmenu::mainmenu(){}
mainmenu::~mainmenu(){}

void mainmenu::init(){}
void mainmenu::update(){}
void mainmenu::render(){}
void mainmenu::close(){}

void map::updatechunks(){
	int x=player.x;int z=player.z;
	for(int x=0;x<size;x++){
		for(int z=0;z<size;z++){
			tiles[x+z*size]=0;
		}
	}
}

void map::updatechunkmesh(){
	std::vector<Vector3> vertices;
	std::vector<Vector2> texcoords;
	std::vector<unsigned short> indices;

	const float size=4.0f;
	const float terrain=1.0f;
	const float liquid=0.3f;
	const float bridge=1.1f;

	int verts=0;

	for(int z=0;z<size;z++){
		for(int x=0;x<size;x++){
			char t=tiles[x+z*size];

			float y=terrain;
			if(t==255)y=liquid;
			else if(t==253)y=bridge;

			Vector3 v0={x*size,y,z*size};
			Vector3 v1={(x+1)*size,y,z*size};
			Vector3 v2={(x+1)*size,y,(z+1)*size};
			Vector3 v3={x*size,y,(z+1)*size};

			vertices.push_back(v0);
			vertices.push_back(v1);
			vertices.push_back(v2);
			vertices.push_back(v3);

			texcoords.push_back({0.0f,0.0f});
			texcoords.push_back({1.0f,0.0f});
			texcoords.push_back({1.0f,1.0f});
			texcoords.push_back({0.0f,1.0f});

			indices.push_back(verts+0);
			indices.push_back(verts+1);
			indices.push_back(verts+2);
			indices.push_back(verts+0);
			indices.push_back(verts+2);
			indices.push_back(verts+3);

			verts+=4;
			if(t!=255){
				struct Edge{int dx,dz;Vector3 n0,n1;};
				Edge edges[4]={
					{0,-1,{x*size,0,z*size},{(x+1)*size,0,z*size}},
					{1,0,{(x+1)*size,0,z*size},{(x+1)*size,0,(z+1)*size}},
					{0,1,{(x+1)*size,0,(z+1)*size},{x*size,0,(z+1)*size}},
					{-1,0,{x*size,0,(z+1)*size},{x*size,0,z*size}}
				};

				for(int e=0;e<4;e++){
					char neighbor=tiles[(x+edges[e].dx)+((z+edges[e].dz)*size)];
					if(neighbor==255){
						Vector3 w0=edges[e].n0;w0.y=liquid;
						Vector3 w1=edges[e].n1;w1.y=liquid;
						Vector3 w2=edges[e].n1;w2.y=terrain;
						Vector3 w3=edges[e].n0;w3.y=terrain;

						vertices.push_back(w0);
						vertices.push_back(w1);
						vertices.push_back(w2);
						vertices.push_back(w3);

						texcoords.push_back({0.0f,0.0f});
						texcoords.push_back({1.0f,0.0f});
						texcoords.push_back({1.0f,1.0f});
						texcoords.push_back({0.0f,1.0f});

						indices.push_back(verts+0);
						indices.push_back(verts+1);
						indices.push_back(verts+2);
						indices.push_back(verts+0);
						indices.push_back(verts+2);
						indices.push_back(verts+3);

						verts+=4;
					}
				}
			}
		}
	}
	mesh.vertexCount=vertices.size();
	mesh.triangleCount=indices.size()/3;

	mesh.vertices=(float*)malloc(sizeof(float)*mesh.vertexCount*3);
	mesh.texcoords=(float*)malloc(sizeof(float)*mesh.vertexCount*2);
	mesh.indices=(unsigned short*)malloc(sizeof(unsigned short)*indices.size());

	for(int i=0;i<mesh.vertexCount;i++){
		mesh.vertices[i*3+0]=vertices[i].x;
		mesh.vertices[i*3+1]=vertices[i].y;
		mesh.vertices[i*3+2]=vertices[i].z;

		mesh.texcoords[i*2+0]=texcoords[i].x;
		mesh.texcoords[i*2+1]=texcoords[i].y;
	}
	for(int i=0;i<indices.size();i++){
		mesh.indices[i]=indices[i];
	}

	UploadMesh(&mesh,false);
}
