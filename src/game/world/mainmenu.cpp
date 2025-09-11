#include "mainmenu.h"

Image skybg;
Texture2D skytx;
unsigned int nextFrameDataOffset = 0;  // Current byte offset to next frame in image.data

int frame = 0;       // Current animation frame to load and draw
int delay = 6;             // Frame delay to switch between animation frames
int counter = 0;           // General frames counter
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
void mainmenu::update(){
	counter++;
        if (counter >= delay)
        {
            frame++;
            if (frame >= frames) frame = 0;

            nextFrameDataOffset = skybg.width*skybg.height*4*frame;
			
            UpdateTexture(skytx, ((unsigned char *)skybg.data) + nextFrameDataOffset);

            counter = 0;
        }

		int pagebefore=pagem;
		float scalex=(float)renderw/(float)GetScreenWidth();
		float scaley=(float)renderh/(float)GetScreenHeight();
		int mx=GetMouseX()*scalex;
		int my=GetMouseY()*scaley;
		
			for (int i = 0; i < memum[pagem].items.size(); i++) {
				if (pagebefore!=pagem)break;
				auto* item=memum[pagem].items[i];
				int x=item->x*scale;
				int y=item->y*scale;
				int w=item->w*scale;
				int h=item->h*scale;
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
	
            DrawTexturePro(skytx,
				{0,0,(float)skybg.width,(float)skybg.height},{0,0,(float)renderw,(float)renderh}
			,
				{0,0},0,WHITE
			);
	
	rendermenum();
}
void mainmenu::close(){}

std::vector<menupage> memum={
	menupage{{
				// (360-110)/2=150
				new menubutton{ 120,100,110,40, 0, "play",
					[](){ changescene=true;nextscene=1; }},
				new menubutton{ 20,180,190,40, 1, "options",
					[](){ pagem=1; }},
				new menubutton{ 230,180,110,40, 2, "exit",
					[](){exit(0);}}
	},0}
};

#define getitem memum[pagem].items
void rendermenum(){
		//DrawRectangle(0, 0, renderw, renderh, (Color){0, 60, 180, 100});
		for (int i=0;i<getitem.size();i++){
			auto* item=getitem[i];
			int x=item->x*scale;
			int y=item->y*scale;
			int w=item->w*scale;
			int h=item->h*scale;
			//x 0 y 400 w 5760 h 80
			if (auto* buttonitem = dynamic_cast<menubutton*>(item)){
				DrawRectangle(x, y, w, h,
					buttonitem->down?Color{180, 150, 150, 100}:
					buttonitem->hover?Color{100, 100, 0, 0}:Color{50, 50, 0, 0}
				);
				//fatal("hover: %i down: %i",buttonitem->hover?1:0,buttonitem->down?1:0);
				Vector2 textPos = {
					w/2-(((float)buttonitem->text.size())*(13*scale*2)/2)+x		,
					(float)y+(h/2-(10*scale/2))
				};
					echo("textPos %f %f",textPos.x,textPos.y);
					echo("x %i y %i w %i h %i",x,y,w,h);
				DrawTextEx(menufont,buttonitem->text.c_str(),textPos, 10*scale*2, 3*scale*2, 
					buttonitem->down?Color{255, 255, 255, 255}:
					buttonitem->hover?Color{255, 150, 100, 255}:Color{255, 150, 0, 255}
				);
				
			}else if (auto* textitem = dynamic_cast<menutext*>(item)){
				DrawRectangle(x, y, w, h, Color{50, 50, 0, 100});
				DrawTextEx(menufont,textitem->text.c_str(),(Vector2){(float)x,(float)y}, 10*scale, 3*scale, WHITE);
			}else{
				
				DrawRectangle(x, y, w, h, Color{50, 50, 0, 100});
			}
		}
}
