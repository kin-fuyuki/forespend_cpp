#include "world.h"
#include <raylib.h>
	int texturesupdt=120;
	float y=-0.1;
	bool drawnworld=true;
	bool headdown=false;
	float headbob=0;
	bool onsurface=true;
	#define BOBMAX 0.15f
	float yaccel=0.f;
	short ptx,ptz=0;
	unsigned char currenttile=0;
	short page=0;
	
	
	int flip=0;
	float playerrotx=0;
	float playerroty=150;
	void rendermenu();
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
        (float)renderw*relresx, 
        (float)renderh*relresy
    };
	
	DrawTexturePro(skytexture, srcrec, dstrec, Vector2{0, 0}, 0, WHITE);
	BeginMode3D(*cameras[thirdperson]);
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
	
	DrawBillboardRec(camera,*player.fs[player.frame],
		Rectangle{0,0,(float)1,2},
		Vector3{(float)player.x,player.y+1,player.z},
		Vector2{1.,2.},WHITE);
	
	EndMode3D();
	//DrawTexture(tilesheet,0,180,WHITE);
	//DrawTexture(skytexture,0,0,WHITE);
	//DrawTexture(tilemapp,0,0,WHITE);
	//DrawTexture(playercursor,ptx,ptz,WHITE);
	int toolbarh=((55./240.)*renderh);
	int toolbary=renderh-toolbarh;
	//DrawTexture(toolbar,0,toolbary,WHITE);
	float time=GetTime();
	SetShaderValue(toolbarshader, timeloc, &time, SHADER_UNIFORM_FLOAT);
	SetShaderValue(toolbarshader, healthloc, &stats.health, SHADER_UNIFORM_FLOAT);
	SetShaderValue(toolbarshader, staminaloc, &stats.stamina, SHADER_UNIFORM_FLOAT);
	SetShaderValue(toolbarshader, manaloc, &stats.mana, SHADER_UNIFORM_FLOAT);
	SetShaderValue(toolbarshader, energyloc, &stats.energy, SHADER_UNIFORM_FLOAT);
	SetShaderValue(toolbarshader, fatigueloc, &stats.fatigue, SHADER_UNIFORM_FLOAT);
	SetShaderValue(toolbarshader, radioloc, &stats.radioactivity, SHADER_UNIFORM_FLOAT);
	SetShaderValue(toolbarshader, xploc, &stats.xp, SHADER_UNIFORM_FLOAT);
	
	//fatal("toolbary %i toolbarh %i",toolbary,toolbarh);
	std::string pos=
""+std::to_string(GetFPS())+" version:"+VERSION
+"\nX:"+std::to_string((int)player.x)+" Y:"+std::to_string((int)player.y)+" Z:"+std::to_string((int)player.z)
+"\ntile:"+std::to_string(((int)currenttile))+" worldx:"+std::to_string((int)ptx)+" worldz:"+std::to_string((int)ptz)
+"\nspeed:"+std::to_string((int)(sped*60))+"m/s";
	BeginShaderMode(toolbarshader);
	if (f1){
	DrawTexturePro(toolbar, (Rectangle){0,0,(float)toolbar.width,(float)toolbar.height},
	(Rectangle){0,(float)toolbary*relresy,(float)renderw*relresx,(float)toolbarh*relresy}, Vector2{0,0}, 0, WHITE);
	EndShaderMode();
	if (f3){
		DrawTextEx(menufont,pos.c_str(),(Vector2){0,0}, 17*relresx, 2*relresy, WHITE);
	}
	
	}
	rendermenu();
	//fatal("f1 %i",f1);	
	}
#include <typeinfo>
#define getitem menu[page]->items
	void rendermenu(){
		if (page!=-1){
		DrawRectangle(0, 0, renderw*relresx, renderh*relresy, (Color){0, 60, 180, 100});
		float sx,sy;
		sx=relresx, sy=relresy;
		for (int i=0;i<getitem.size();i++){
			auto* item=getitem[i];
			int x=item->x*sx;
			int y=item->y*sy;
			int w=item->w*sx;
			int h=item->h*sy;
			//x 0 y 400 w 5760 h 80
			if (auto* buttonitem = dynamic_cast<menubutton*>(item)){
				DrawRectangle(x, y, w, h,
					buttonitem->down?Color{180, 150, 150, 100}:
					buttonitem->hover?Color{100, 100, 0, 100}:Color{50, 50, 0, 100}
				);
				//fatal("hover: %i down: %i",buttonitem->hover?1:0,buttonitem->down?1:0);
				Vector2 textPos = {
					w/2-(((float)buttonitem->text.size())*(17*sx)/2)		,
					(float)y+(h/2-(15*sy/2))
				};
					echo("textPos %f %f",textPos.x,textPos.y);
					echo("x %i y %i w %i h %i",x,y,w,h);
				DrawTextEx(menufont,buttonitem->text.c_str(),textPos, 17*sx, 2*sy, WHITE);
				
			}else if (auto* textitem = dynamic_cast<menutext*>(item)){
				Vector2 textPos = {
					w/2-(((float)textitem->text.size())*(sx*relresx)/2)+x		,
					(float)y+(h/2-((15+((page==0)*9))*relresy/2))
				};
					echo("textPos %f %f",textPos.x,textPos.y);
					echo("x %i y %i w %i h %i",x,y,w,h);
				DrawTextEx(menufont,textitem->text.c_str(),textPos, sx*relresx, 3*relresx, Color{255, 255, 255, 255});
			}else if(auto* edititem = dynamic_cast<menucolor*>(item)){
				DrawRectangle(x, y, w, h, *edititem->value);
			}
			
			else{
				
				DrawRectangle(x, y, w, h, Color{50, 50, 0, 100});
			}
		}
	}
}

