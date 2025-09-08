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
float daycol[4]={.8,.8,.8,.8};
char daytime=2;
float hourcycle=0.;
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
int tilemaploc,sizeloc,colsloc,modelloc,MVPloc,sheetloc,fliploc,fragcolorloc,camloc;
Matrix model=Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

Material basicmat;
	Texture2D tilemapp;
	float worldSizeV[2];
FastNoiseLite texturenoise;
Image sheet;
Image skybox;
Texture skytexture;
Texture playercursor;
void map::init(){
	worldSizeV[0] = (float)size;worldSizeV[1] = (float)size;
	camera.position = (Vector3){0.0f, 1.3f, 0.0f};
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
	playercursor=LoadTexture("res/images/playercursor.png");
	rlSetClipPlanes(.2, 4000.0);

	tilesheet = LoadTexture("res/images/tilesheet.png");
	//SetTextureFilter(tilesheet, TEXTURE_FILTER_N);

	tilemaploc  = GetShaderLocation(tilemat.shader, "tilemap");
	sheetloc= GetShaderLocation(tilemat.shader, "tilesheet");
	MVPloc = GetShaderLocation(tilemat.shader, "mvp");
	modelloc = GetShaderLocation(tilemat.shader, "modelmat");
	sizeloc = GetShaderLocation(tilemat.shader, "size");
	fragcolorloc = GetShaderLocation(tilemat.shader, "fragColor");
	camloc = GetShaderLocation(tilemat.shader, "campos");
	colsloc = GetShaderLocation(tilemat.shader, "cols");
	fliploc = GetShaderLocation(tilemat.shader, "flip");
	echo("meshcount %i",worldmodel.meshCount);
	echo("materialcount %i",worldmodel.materialCount);
	basicmat=LoadMaterialDefault();
	
	humiditynoise=FastNoiseLite((this->world*3)+1);
	heatnoise=FastNoiseLite((this->world*3)+2);
	populationnoise=FastNoiseLite((this->world*3));
	
	humiditynoise.SetFractalType(FastNoiseLite::FractalType::FractalType_None);
	humiditynoise.SetFractalOctaves(3);
	humiditynoise.SetFrequency(0.4);
	humiditynoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Value);
	humiditynoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
	
	
	heatnoise.SetFractalType(FastNoiseLite::FractalType::FractalType_None);
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
float playerroty=150;
unsigned char currenttile;
short ptx,ptz;
void map::render(){
	float campos[3]={camera.position.x,camera.position.y,camera.position.z};
	Matrix view = MatrixLookAt(camera.position, camera.target, camera.up);
	float aspect = (float)renderw / (float)renderh;
	Matrix projection = MatrixPerspective(80, aspect, .2, 4000.0);
	Matrix mvp = MatrixMultiply(model, MatrixMultiply(view, projection));
	int cols = 16;
	flip = 0;
	
	float yaw = fmodf(playerrotx, 360.0f);
	if (yaw < 0) yaw += 360.0f;

	float skyoffx = (yaw / 360.0f) * skytexture.width;
    float skyoffy = (playerroty);
    
    skyoffx = fmod(skyoffx, skytexture.width);
    skyoffy = fmod(skyoffy, skytexture.height);
    if (skyoffx < 0) skyoffx += skytexture.width;
    if (skyoffy < 0) skyoffy += skytexture.height;
    
    Rectangle srcrec = {
        skyoffx, 
        skyoffy, 
        (float)skytexture.width/(renderw/100), 
        (float)skytexture.height/2
    };
    Rectangle dstrec = {
        0, 
        0, 
        (float)renderw, 
        (float)renderh
    };
	
	DrawTexturePro(skytexture, srcrec, dstrec, Vector2{0, 0}, 0, WHITE);
	BeginMode3D(camera);
	rlEnableColorBlend();
	SetShaderValue(tilemat.shader, colsloc, &flip, SHADER_UNIFORM_INT);
	SetShaderValueTexture(tilemat.shader, tilemaploc, tilemapp);
	SetShaderValueTexture(tilemat.shader, sheetloc, tilesheet);
	SetShaderValue(tilemat.shader, colsloc, &cols, SHADER_UNIFORM_INT);
	SetShaderValue(tilemat.shader, sizeloc, worldSizeV, SHADER_UNIFORM_VEC2);
	SetShaderValue(tilemat.shader, camloc, campos, SHADER_UNIFORM_VEC3);
	SetShaderValueMatrix(tilemat.shader,modelloc, model);
	SetShaderValueMatrix(tilemat.shader,MVPloc, mvp);
	rlDisableBackfaceCulling();
	//basicmat tilemat
	SetShaderValue(tilemat.shader, fragcolorloc, &daycol, SHADER_UNIFORM_VEC4);
	DrawMesh(worldmodel.meshes[0],tilemat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	DrawMesh(worldmodel.meshes[1],tilemat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	DrawMesh(worldmodel.meshes[2],tilemat,Matrix{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
	
	DrawCube((Vector3){0,0,0},1,1,1,WHITE);
	EndMode3D();
	//DrawTexture(tilesheet,0,180,WHITE);
	//DrawTexture(skytexture,0,0,WHITE);
	//DrawTexture(tilemapp,0,0,WHITE);
	//DrawTexture(playercursor,ptx,ptz,WHITE);
	std::string pos="X: "+std::to_string((int)player.x)+" Y: "+std::to_string((int)player.y)+" Z: "+std::to_string((int)player.z)
+"\n\n tile: "+std::to_string(((int)currenttile))+" worldx: "+std::to_string((int)ptx)+" worldz: "+std::to_string((int)ptz);
	DrawText(pos.c_str(), 0, 0, 32, WHITE);
	DrawText(std::to_string(GetFPS()).c_str(), 0, 36, 32, WHITE);
	
}
int texturesupdt=120;
float y=-0.1;
void updatetextures();
bool drawnworld=true;
bool headdown=false;
float headbob=0;
bool onsurface=true;
#define BOBMAX 0.15f
float yaccel=0.f;
void map::update(){
	if (mustupdate==true){
		updatechunks();
		mustupdate=false;
	}
	ptx=((short)(player.x/4)+512);
	ptz=1024-((short)(player.z/4.)+512);
	if (player.z>0)ptz--;
	if (player.x<0)ptx--;
	currenttile=tiles[(int)ptx+size*(int)ptz];
	bool onwater=currenttile>=250||(currenttile>=245&&player.y>=1.0);
	bool onground=(onwater
? player.y<=-0.8
: player.y<=1.
);
	float turnspeed=1.f;
	if (IsKeyPressed(KEY_F12)){
		mustupdate=true;
	}
	bool sprinting=(IsKeyDown(KEY_LEFT_SHIFT)||IsKeyDown(KEY_RIGHT_SHIFT));
	float movespeed=.01f*(1.+10* sprinting);
	if (onwater&&onground){
		movespeed/=4;
	}
	bool crouching=(IsKeyDown(KEY_LEFT_CONTROL)||IsKeyDown(KEY_RIGHT_CONTROL));
	float camx=
			IsKeyDown(KEY_RIGHT)*turnspeed - IsKeyDown(KEY_LEFT)*turnspeed;
	float camy=
			IsKeyDown(KEY_DOWN)*turnspeed - IsKeyDown(KEY_UP)*turnspeed;
			
	
	yaccel=onground?0.f:
		yaccel<=-1.?-1.:
		yaccel-0.005f;
	yaccel=(IsKeyDown(KEY_SPACE)
	&&onground&&!onwater
)?.08:yaccel;		
	float jumpboost=(yaccel!=0.f)+1.;
	float mx=(IsKeyDown(KEY_W) - IsKeyDown(KEY_S))*movespeed*jumpboost;
	float my=(IsKeyDown(KEY_D) - IsKeyDown(KEY_A))*movespeed*jumpboost;
	
	if (headbob<-0.05f){
		headdown=false;
	}else if (headbob>0.05f){
		headdown=true;
	}
	float totalspeed=sqrt(mx*mx+my*my)*(sprinting?0.5:1.);
	float bob=headdown?-totalspeed/4:totalspeed/4;
	headbob+=bob;
	float tcos=cos(player.yaw);
	float tsin=sin(player.yaw);
	
	float targetx=player.x+((mx+0.3)*tcos-(my+0.3)*tsin);
	float targetz=player.z+((mx+0.3)*tsin+(my+0.3)*tcos);
	short ttx=((short)(targetx/4)+512);
	short ttz=1024-((short)(targetz/4.)+512);
	if (targetz>0)ttz--; // wont work cuz ur not taking rotation into consideration
	if (targetx<0)ttx--;
	unsigned char targettile=tiles[(int)ttx+size*(int)ttz];
	if (currenttile!=targettile){
		echo("currenttile %i targettile %i",currenttile,targettile);
	}
	if (currenttile>=250&&targettile<250&&player.y<1.){
		mx=0;my=0;
		yaccel=0.01f;
	}
	UpdateCameraPro(&camera,
		(Vector3){
			mx,	my,
			bob+
			yaccel
//			+	(IsKeyDown(KEY_SPACE)*movespeed - IsKeyDown(KEY_LEFT_CONTROL)*movespeed)*1
		},
		(Vector3){
			camx, 
			camy,
			0.0f
			
		},
		0
	);
	playerrotx+=camx;
	
	if ((playerroty>179&&camy>0)||(playerroty<101&&camy<0)){
		camy=0;
	}
	playerroty+=camy/2;
	//echo ("playerrotx %f playerroty %f",playerrotx,playerroty);
	int prevy=(int)player.y;
	player.x=camera.position.x;
	player.y=camera.position.y-(1.6+headbob);
	player.z=camera.position.z;
	Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
	player.yaw=atan2f(forward.z, forward.x);
	if (player.y>=150&&prevy<150){
	tilesheet = LoadTexture("res/images/tilesheetl1.png");
	sheet=LoadImageFromTexture(tilesheet);
	updatetextures();
	}else if (player.y<150&&prevy>=150){
	tilesheet = LoadTexture("res/images/tilesheet.png");
	sheet=LoadImageFromTexture(tilesheet);
	updatetextures();
	}
	else if(player.y>=200&&prevy<200){
	tilesheet = LoadTexture("res/images/tilesheetl2.png");
	sheet=LoadImageFromTexture(tilesheet);
	updatetextures();		
	}else if(player.y<200&&prevy>=200){
	tilesheet = LoadTexture("res/images/tilesheetl1.png");
	sheet=LoadImageFromTexture(tilesheet);
	updatetextures();
	}
	else if (player.y>=280&&prevy<280){
	tilesheet = LoadTexture("res/images/tilesheetl3.png");
	sheet=LoadImageFromTexture(tilesheet);
	updatetextures();
	}else if (player.y<280&&prevy>=280){
	tilesheet = LoadTexture("res/images/tilesheetl2.png");
	sheet=LoadImageFromTexture(tilesheet);
	updatetextures();
	}
	else if (player.y>=360&&prevy<360){
	tilesheet = LoadTexture("res/images/tilesheetl4.png");
	sheet=LoadImageFromTexture(tilesheet);
	updatetextures();
	}else if (player.y<360&&prevy>=360){
	tilesheet = LoadTexture("res/images/tilesheetl3.png");
	sheet=LoadImageFromTexture(tilesheet);
	updatetextures();
	}
	texturesupdt++;
	if (texturesupdt>60){
		texturesupdt=0;
		y+=0.1;
		updatetextures();
		hourcycle+=0.1;
		if (hourcycle>=1.){
			hourcycle=0;
			daytime=daytime==5?daytime=0:daytime+1;
		}
	}
}
void map::close(){
	unsigned char *data=new unsigned char[size*size*3];
	tilesheet = LoadTexture("res/images/tilesheetl4.png");
	sheet=LoadImageFromTexture(tilesheet);
	for (int i = 0; i < size*size; i++) {
		unsigned char tile = ((unsigned char*)tilemaptx.data)[i];
		int xtilecoord=tile%16;
		int ytilecoord=tile/16;
		data[i*3+0]=((unsigned char*)sheet.data)[(ytilecoord*16+xtilecoord)*4+0];
		data[i*3+1]=((unsigned char*)sheet.data)[(ytilecoord*16+xtilecoord)*4+1];
		data[i*3+2]=((unsigned char*)sheet.data)[(ytilecoord*16+xtilecoord)*4+2];
		
	}
	savebmp("tilemap.bmp",data,
	tilemaptx.format==PIXELFORMAT_UNCOMPRESSED_GRAYSCALE?tilemaptx.width:tilemaptx.width
	,tilemaptx.height);
}
//#define TESTGEN

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
	long start = __rdtsc();
	long genstart = __rdtsc();
	if (!drawnworld){
	int chunksize=64;
	int chunks = size / chunksize;
	// 1=0 2=1 4=2 8=3 16=4 32=5 64=6 128=7 256=8 512=9 1024=10 2048=11 4096=12 8192=13 16384=14 32768=15
	unsigned char shift=6;
	for (int cz = 0; cz < chunks; cz++) {
		for (int cx = 0; cx < chunks; cx++) {
			
			float fx=cx; float fz=cz;
			float heat=(heatnoise.GetNoise(fx,fz));
			float humidity=(humiditynoise.GetNoise(fx,fz));
			float population=(populationnoise.GetNoise(fx,fz));
			char heatpos=(heat+1.)*4;
			char humiditypos=(humidity+1.)*2;
			unsigned char mask=(heatpos<<2)+humiditypos;
			
			for (int z = 0; z < chunksize; z++) {
				int zidx = (cz << shift) + z;
				unsigned char tile = gentile((cx << shift), zidx,mask,(unsigned char)population);
				for (int x = 0; x < chunksize; x++) {
					
					int xidx = (cx << shift) + x;
					tile = gentile(xidx, zidx,mask,(unsigned char)population);
					
					((unsigned char*)tilemaptx.data)[zidx * size + xidx] = tile;
					tiles[zidx * size + xidx] = tile;
				}
			}
		}
	}}
	long genend = __rdtsc();
	
	long setstart = __rdtsc();
	long setend = __rdtsc();
	long end = __rdtsc();
	if (drawnworld){
	tilemaptx=LoadImage("res/images/coolworld.png");
	for (int i = 0; i < size*size; i++) {
		tiles[i]=((unsigned char*)tilemaptx.data)[i];
	}
}
	bool naturallygray=tilemaptx.format==PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
	echo ("size: %s, format: %s", FORMAT_NUM(tilemaptx.width), FORMAT_NUM(tilemaptx.format));
	
	tilemaptx.format=PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
	//tilemaptx.width>>=2;
	if (tilemaptx.format==PIXELFORMAT_UNCOMPRESSED_GRAYSCALE&&!naturallygray){
		tilemaptx.width<<=2;
	}
	tilemapp=LoadTextureFromImage(tilemaptx);
	//SetTextureFilter(tilemapp, TEXTURE_FILTER_POINT);
	UpdateTexture(tilemapp, tilemaptx.data);
	success("cycles: %s", FORMAT_NUM(end - start));
	success("terrain gen cycles: %s\nset cycles: %s", FORMAT_NUM(genend - genstart), FORMAT_NUM(setend - setstart));

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
	{G1,Color{20,110,18,255}},
	{G2,Color{55,156,60,255}},
	{G3,Color{111,255,122,255}},
};
#define W1 0.1f
#define W2 0.38f
#define W3 0.58f
#define W4 0.98f
std::map<float, Color> water={
	{W1,Color{100,150,200,255}},
	{W2,Color{162,205,255,255}},
	{W3,Color{255,255,255,255}},
	{W4,Color{162,205,255,255}},
	
};
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
short rows[8][2]={
	{0,10},
	{10,20},
	{20,30},
	{30,40},
	{40,50},
	{50,60},
	{60,70},
	{70,80}
};
char row=0;
bool first=true;
void updatetextures(){
	dragx+=0.5f;
	dragz+=1.5f;
	long start = __rdtsc();
	texturenoise.SetFractalType(FastNoiseLite::FractalType::FractalType_None);
	texturenoise.SetFractalOctaves(0);
	texturenoise.SetFrequency(1.);
	texturenoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Value);
	texturenoise.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
	int size=sheet.width>>4;
	int tileX = 1;
	int tileY = 0;
	int px = tileX * size;
	int pz = tileY * size;
	float previdx = texturenoise.GetNoise((float)0, y, (float)0);
	int roadsx[]={
		12*size,13*size,14*size,15*size,12*size,13*size
	};
	int roadsz[]={
		1*size,1*size,1*size,1*size,2*size,2*size
	};
	bool genpixel=true;
	for (int x=0;x<size;x++){
		for (int z=0;z<size;z++){
			int xidx = px + x;
			int zidx = pz + z;

			int px = (zidx * sheet.width + xidx) * 4;
			int pixels[]{
				((roadsz[0]+z)*sheet.width+(roadsx[0]+x))*4,
				((roadsz[1]+z)*sheet.width+(roadsx[1]+x))*4,
				((roadsz[2]+z)*sheet.width+(roadsx[2]+x))*4,
				((roadsz[3]+z)*sheet.width+(roadsx[3]+x))*4,
				((roadsz[4]+z)*sheet.width+(roadsx[4]+x))*4,
				((roadsz[5]+z)*sheet.width+(roadsx[5]+x))*4
			};
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
			if (
				((unsigned char*)sheet.data)[pixels[0]+0]==0 &&
				((unsigned char*)sheet.data)[pixels[0]+1]==255 &&
				((unsigned char*)sheet.data)[pixels[0]+2]==0||
				((unsigned char*)sheet.data)[pixels[0]+3]==254
			){
			((unsigned char*)sheet.data)[pixels[0]+0] = r.r;
			((unsigned char*)sheet.data)[pixels[0]+1] = r.g;
			((unsigned char*)sheet.data)[pixels[0]+2] = r.b;
			((unsigned char*)sheet.data)[pixels[0]+3] = 254;
			
			}
			if (
				((unsigned char*)sheet.data)[pixels[1]+0]==0 &&
				((unsigned char*)sheet.data)[pixels[1]+1]==255 &&
				((unsigned char*)sheet.data)[pixels[1]+2]==0||
				((unsigned char*)sheet.data)[pixels[1]+3]==254
			){
				((unsigned char*)sheet.data)[pixels[1]+0] = r.r;
				((unsigned char*)sheet.data)[pixels[1]+1] = r.g;
				((unsigned char*)sheet.data)[pixels[1]+2] = r.b;
				((unsigned char*)sheet.data)[pixels[1]+3] = 254;
			}
			if (
				((unsigned char*)sheet.data)[pixels[2]+0]==0 &&
				((unsigned char*)sheet.data)[pixels[2]+1]==255 &&
				((unsigned char*)sheet.data)[pixels[2]+2]==0||
				((unsigned char*)sheet.data)[pixels[2]+3]==254
			){
			((unsigned char*)sheet.data)[pixels[2]+0] = r.r;
			((unsigned char*)sheet.data)[pixels[2]+1] = r.g;
			((unsigned char*)sheet.data)[pixels[2]+2] = r.b;
			((unsigned char*)sheet.data)[pixels[2]+3] = 254;
			}
			if (
				((unsigned char*)sheet.data)[pixels[3]+0]==0 &&
				((unsigned char*)sheet.data)[pixels[3]+1]==255 &&
				((unsigned char*)sheet.data)[pixels[3]+2]==0||
				((unsigned char*)sheet.data)[pixels[3]+3]==254
			){
			((unsigned char*)sheet.data)[pixels[3]+0] = r.r;
			((unsigned char*)sheet.data)[pixels[3]+1] = r.g;
			((unsigned char*)sheet.data)[pixels[3]+2] = r.b;
			((unsigned char*)sheet.data)[pixels[3]+3] = 254;
			}
			if (
				((unsigned char*)sheet.data)[pixels[4]+0]==0 &&
				((unsigned char*)sheet.data)[pixels[4]+1]==255 &&
				((unsigned char*)sheet.data)[pixels[4]+2]==0||
				((unsigned char*)sheet.data)[pixels[4]+3]==254
			){
			((unsigned char*)sheet.data)[pixels[4]+0] = r.r;
			((unsigned char*)sheet.data)[pixels[4]+1] = r.g;
			((unsigned char*)sheet.data)[pixels[4]+2] = r.b;
			((unsigned char*)sheet.data)[pixels[4]+3] = 254;
			}
			if (
				((unsigned char*)sheet.data)[pixels[5]+0]==0 &&
				((unsigned char*)sheet.data)[pixels[5]+1]==255 &&
				((unsigned char*)sheet.data)[pixels[5]+2]==0||
				((unsigned char*)sheet.data)[pixels[5]+3]==254
			){
			((unsigned char*)sheet.data)[pixels[5]+0] = r.r;
			((unsigned char*)sheet.data)[pixels[5]+1] = r.g;
			((unsigned char*)sheet.data)[pixels[5]+2] = r.b;
			((unsigned char*)sheet.data)[pixels[5]+3] = 254;
			}
			
			
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
	px = tileX * size;
	pz = tileY * size;
	for (int x=0;x<size;x++){
		for (int z=0;z<size;z++){
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
	px = tileX * size;
	pz = tileY * size;
	for (int x=0;x<size;x++){
		for (int z=0;z<size;z++){
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
	
	texturenoise.SetFrequency(0.5);
	texturenoise.SetFractalOctaves(4);


int width = skybox.width;
int height = first?skybox.height:rows[row][1];
int startx=0;
int startz=first?100:rows[row][0];
float sca = 3.0f;
float yd = 0.02f;
char nextdaytime=daytime==5?0:daytime+1;
Color sky1=ColorLerp(daynight[daytime][SKY1], daynight[nextdaytime][SKY1], hourcycle);
Color sky2=ColorLerp(daynight[daytime][SKY2], daynight[nextdaytime][SKY2], hourcycle);
Color sky3=ColorLerp(daynight[daytime][SKY3], daynight[nextdaytime][SKY3], hourcycle);
first=false;
daycol[0] = (
	((float)sky3.r) + ((float)sky1.r))/ 512.0f;
daycol[1] = (
	((float)sky3.g) + ((float)sky1.g))/ 512.0f;
daycol[2] = (
	((float)sky3.b) + ((float)sky1.b))/ 512.f;
for (int z = startz; z < height; ++z) {
    float v = (float)z / (float)(100 - 1);
    float phi = (v - 0.5f) * PI;

    for (int x = startx; x < width; ++x) {
        float u = (float)x / (float)skybox.width;
        float theta = u * 2.0f * PI;

        float cph = cosf(phi);
        float vx = cph * cosf(theta);
        float vy = sinf(phi);
        float vz = cph * sinf(theta);
        float idx = texturenoise.GetNoise(
vx * sca, vy * sca * 2,vz * sca, y * yd);

        Color r;
        if (idx >= SKY2) {
            r = sky1;
        } else if (idx >= SKY3) {
            float denom = (SKY2 - SKY3);
            float t = denom != 0.0f ? (idx - SKY3) / denom : 0.0f;
            t = fmaxf(0.0f, fminf(1.0f, t));
            r = ColorLerp(sky3, sky1, t);
        } else {
            r = sky3;
        }

        int px = (z * skybox.width + x) * 3;
        ((unsigned char*)skybox.data)[px + 0] = r.r;
        ((unsigned char*)skybox.data)[px + 1] = r.g;
        ((unsigned char*)skybox.data)[px + 2] = r.b;
    }
}


	row=++row==9?0:row;
	long end = __rdtsc();
	success ("cycles: %s", FORMAT_NUM(end - start));
	
	UpdateTexture(tilesheet, sheet.data);
	UpdateTexture(skytexture, skybox.data);
}