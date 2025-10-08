#include "mainmenu.h"

Image skybg;
Texture2D skytx;
unsigned int nextFrameDataOffset = 0;

int frame = 0;
int delay = 6;
int counter = 0;
int frames = 0;
short pagem=0;
void rendermenum();
mainmenu::mainmenu(){}
mainmenu::~mainmenu(){
	UnloadTexture(skytx);
	UnloadImage(skybg);
	
}
void mainmenu::init(){
	frames=0;
	skybg = LoadImageAnim("res/images/sky.gif",&frames);
	skytx = LoadTextureFromImage(skybg);
}
int fontsize=8;
void mainmenu::update(){
	if (IsKeyPressed(KEY_KP_ADD)) fontsize++;
	if (IsKeyPressed(KEY_KP_SUBTRACT)) fontsize--;
	counter++;
        if (counter >= delay)
        {
            frame++;
            if (frame >= frames) frame = 0;

            nextFrameDataOffset = skybg.width*skybg.height*4*frame;
			
            UpdateTexture(skytx, ((unsigned char *)skybg.data) + nextFrameDataOffset);
			fatal("frame %i",frame);

            counter = 0;
        }
		int pagebefore=pagem;
		int mx,my;
		mx=GetMouseX();
		my=GetMouseY();
		if (cfg.internalres){
			float scalex=(float)renderw/(float)GetScreenWidth();
			float scaley=(float)renderh/(float)GetScreenHeight();
		mx*=scalex,	my*=scaley;
		}else{
			float scalex=(float)GetScreenWidth()/(float)360;
			float scaley=(float)GetScreenHeight()/(float)240;
		mx/=scalex,	my/=scaley;
		}
			for (int i = 0; i < memum[pagem]->items.size(); i++) {
				
				if (pagebefore!=pagem)break;
				auto* item=memum[pagem]->items[i];
				int x=item->x*relresx;
				int y=item->y*relresy;
				int w=item->w*relresx;
				int h=item->h*relresy;
				if (auto* buttonitem = dynamic_cast<menubutton*>(item)){
					if (mx>x&&mx<x+w&&my>y&&my<y+h){
						if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
							buttonitem->process();
						}if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
							buttonitem->hover=false;
							buttonitem->down=true;
						}else {
							buttonitem->down=false;
							buttonitem->hover=true;
						}
					}else{
						buttonitem->down=false;
						buttonitem->hover=false;
					}
					
				}
			}

}
void mainmenu::render(){
	
	
	rendermenum();
}
void mainmenu::close(){}
#include "../game.h"
std::vector<menupage*> memum={
	new menupage{{
				// (360-110)/2=150
				new menubutton{ 120,100,110,40, 0, "classic",
					[](){ changescene=true;nextscene=1; }},
					
				new menubutton{ 20,180,190,40, 1, "options",
					[](){ *p=1; }},
					
				new menubutton{ 230,180,110,40, 2, "exit",
					[](){close();}}
	},0},
	&options,&graphics,&sound,&controls,
	
	new menupage{{
		
	},0}
};

#define getitem memum[pagem]->items
void rendermenum(){
		//DrawRectangle(0, 0, renderw, renderh, (Color){0, 60, 180, 100});
		int sizef=pagem==0?24:17;
		float sx,sy;
		if (!cfg.internalres)
			{sx=(float)GetScreenWidth()/(float)360, sy=(float)GetScreenHeight()/(float)240;		
}		else{
		sx=relresx, sy=relresy;}
		DrawTexturePro(skytx,
			{0,0,(float)skybg.width,(float)skybg.height},{0,0,(float)renderw*sx,(float)renderh*sy}
		,
			{0,0},0,WHITE
		);
		//DrawRectangle(0, 0, renderw*sx,renderh*sy, (Color){0, 60, 180, 100});
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
					buttonitem->hover?Color{100, 100, 0, 0}:Color{50, 50, 0, 0}
				);
				//fatal("hover: %i down: %i",buttonitem->hover?1:0,buttonitem->down?1:0);
				Vector2 textPos = {
					w/2-(((float)buttonitem->text.size())*(sizef*sx)/2)+x		,
					(float)y+(h/2-((15+((pagem==0)*9))*sy/2))
				};
					echo("textPos %f %f",textPos.x,textPos.y);
					echo("x %i y %i w %i h %i",x,y,w,h);
				DrawTextEx(menufont,buttonitem->text.c_str(),textPos, sizef*sx, 3*sx, 
					buttonitem->down?Color{255, 255, 255, 255}:
					buttonitem->hover?Color{255, 150, 100, 255}:Color{255, 150, 0, 255}
				);
				
			}else if (auto* textitem = dynamic_cast<menutext*>(item)){
				Vector2 textPos = {
					w/2-(((float)textitem->text.size())*(sizef*sx)/2)+x		,
					(float)y+(h/2-((15+((pagem==0)*9))*sy/2))
				};
					echo("textPos %f %f",textPos.x,textPos.y);
					echo("x %i y %i w %i h %i",x,y,w,h);
				DrawTextEx(menufont,textitem->text.c_str(),textPos, sizef*sx, 3*sx, Color{255, 255, 255, 255});
			}else if(auto* edititem = dynamic_cast<menucolor*>(item)){
				DrawRectangle(x, y, w, h, *edititem->value);
			}
			else{
				
				DrawRectangle(x, y, w, h, Color{50, 50, 0, 100});
			}
		}
}
