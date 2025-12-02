#include "game.h"
#include "../libs/term.h"
float relresx,relresy;
float prevresx,prevresy;
float winwidth,winheight;
int scale=3;	
short	renderw=360;
short	renderh=240;
bool changescene=false;
short nextscene=0;
Shader shd;
RenderTexture2D rndr;
void init() {
	cfg.load();
	p=&page;
	pagem=0;
	page=-1;
	tiny::startup(CSTR(NAME),CSTR(VERSION));
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	
	InitWindow(STARTINGRESX,STARTINGRESY,NAME.c_str());
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
	winwidth=GetScreenWidth();
	winheight=GetScreenHeight();
	
	scale=cfg.scale;
	renderw*=scale;
	renderh*=scale;
	prevresx=winwidth;
	prevresy=winheight;
	relresx=(float)winwidth/(float)renderw;
	relresy=(float)winheight/(float)renderh;
	current->init();
	tiny::error("gl version: %i",rlGetVersion());
	shd=LoadShader(AT("res/shaders/screen.vs"),AT("res/shaders/screen.fs"));
	SetExitKey(KEY_NULL);
	
	rndr=LoadRenderTexture(renderw,renderh);
	menufont=LoadFont(AT("res/fonts/dos.fnt"));
	SetTextureFilter(menufont.texture, TEXTURE_FILTER_POINT);
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
	int hihi;
	
	now=__rdtsc();
	delta=now-prv;
	tiny::echo("delta %i",delta);
	prv=now;
}
Color clra={255,0,0,255};
short col=1;
void updateclra();
void render() {
	winwidth=GetScreenWidth();
	winheight=GetScreenHeight();
	bool previnternal=cfg.internalres;
	if (IsKeyDown(KEY_LEFT_ALT)&& IsKeyDown(KEY_F4)) close();
	current->update();
	SetTraceLogLevel(LOG_ERROR);
	if (prevresx!=winwidth||prevresy!=winheight && !cfg.internalres
	|| previnternal!= cfg.internalres
	){
			UnloadRenderTexture(rndr);
			if (cfg.internalres)
			rndr=LoadRenderTexture(renderw,renderh);
			else
			rndr=LoadRenderTexture(winwidth,winheight);
		}
			
	
	if (changescene) {
		switch (nextscene) {
				case 0:
					pagem=0;
					page=-1;
					p=&pagem;
					changescene=false;
					current->close();
					
					current=&menum;
					world=map();
					current->init();
					break;
				case 1:
					pagem=0;
					page=-1;
					p=&page;
					changescene=false;
					current->close();
					current=&world;
					menum=mainmenu();
					current->init();
					break;
		}
	}
	updateclra();

	
	BeginTextureMode(rndr);
	ClearBackground(clra);
	current->render();
	EndTextureMode();
	
	BeginDrawing();
	BeginShaderMode(shd);
	DrawTexturePro(rndr.texture,
		{0,0,(float)rndr.texture.width,(float)-rndr.texture.height},{0,0,(float)GetScreenWidth(),(float)GetScreenHeight()}
	,
	{0,0},0,WHITE
	);
	EndShaderMode();
	EndDrawing();

	//clearchanged();

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