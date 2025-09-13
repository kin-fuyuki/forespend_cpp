#include "game.h"
#include "../term.h"
float relresx,relresy;
float prevresx,prevresy;
float winwidth,winheight;
int scale=4;	
short	renderw=360;
short	renderh=240;
bool changescene=false;
short nextscene=0;
Shader shd;GRAPHICS_API_OPENGL_33
RenderTexture2D rndr;
void init() {
	p=&pagem;
	pagem=0;
	startup(CSTR(NAME),CSTR(VERSION));
	cfg.load();
	SetConfigFlags(FLAG_VSYNC_HINT|FLAG_WINDOW_RESIZABLE);
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	
	InitWindow(STARTINGRESX,STARTINGRESY,NAME.c_str());
	winwidth=GetScreenWidth();
	winheight=GetScreenHeight();
	
	prevresx=winwidth;
	prevresy=winheight;
	relresx=(float)renderw/(float)winwidth;
	relresy=(float)renderh/(float)winheight;
	menum.init();
	world.init();
	error("gl version: %i",rlGetVersion());
	shd=LoadShader("res/shaders/screen.vs","res/shaders/screen.fs");
	SetExitKey(KEY_NULL);
	
	renderh*=cfg.scale;
	renderw*=cfg.scale;
	rndr=LoadRenderTexture(renderw,renderh);
	menufont=LoadFont("res/fonts/dos.ttf");
}
void close() {
	world.close();
	menum.close();
	
	CloseWindow();
	cfg.save();
}
void mouse(int button,int state,int x,int y) {
	
}

unsigned long now=0;
unsigned long prv=0;
unsigned long delta=0;
short updatems=1;
void timer(int value) {
	now=__rdtsc();
	delta=now-prv;
	echo("delta %i",delta);
	prv=now;
}
Color clra={255,0,0,255};
short col=1;
void updateclra();
void render() {
	winwidth=GetScreenWidth();
	winheight=GetScreenHeight();
	if (prevresx!=winwidth||prevresy!=winheight){
		renderw=winwidth;
		renderh=winheight;
		relresx=(float)renderw/(float)winwidth;
		relresy=(float)renderh/(float)winheight;
		rndr=LoadRenderTexture(renderw,renderh);
		prevresx=winwidth;
		prevresy=winheight;
	}
	if (changescene) {
		switch (nextscene) {
				case 0:
					pagem=0;
					p=&page;
					changescene=false;
					current=&menum;
					break;
				case 1:
					pagem=-1;
					p=&page;
					changescene=false;
					current=&world;
					break;
		}
	}
	updateclra();

	if (IsKeyDown(KEY_LEFT_ALT)&& IsKeyDown(KEY_F4)) close();
	current->update();
	SetTraceLogLevel(LOG_ERROR);
	
	if (cfg.internalres)
	BeginTextureMode(rndr);
	else
	BeginDrawing();
	ClearBackground(clra);
	current->render();
	if (cfg.internalres){
	EndTextureMode();
	
	BeginDrawing();
	BeginShaderMode(shd);
	DrawTexturePro(rndr.texture,
		{0,0,(float)renderw,(float)-renderh},{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()}
	,
	{0,0},0,WHITE
	);
	EndShaderMode();}
	EndDrawing();
	
	clearchanged();
	
}

void updateclra() {
	switch (col)
	{
	case 0:
		if (clra.r == 255) {
			col = 1;
			clra.b = 0;
		} else {
			clra.r++;
			if (clra.b != 0) clra.b--;
		}
		break;

	case 1:
		if (clra.g == 255) {
			col = 2;
			clra.r = 0;
		} else {
			clra.g++;
			if (clra.r != 0) clra.r--;
		}
		break;

	case 2:
		if (clra.b == 255) {
			col = 0;
			clra.g = 0;
		} else {
			clra.b++;
			if (clra.g != 0) clra.g--;
		}
		break;

	default:
		break;
	}
}