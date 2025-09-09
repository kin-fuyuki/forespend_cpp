#include"world.h"

FastNoiseLite humiditynoise;
FastNoiseLite heatnoise;
FastNoiseLite populationnoise;
FastNoiseLite texturenoise;
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


first=false;
	row=++row==9?0:row;
	long end = __rdtsc();
	success("cycles: %s", FORMAT_NUM(end - start));
	
	UpdateTexture(tilesheet, sheet.data);
	UpdateTexture(skytexture, skybox.data);
}