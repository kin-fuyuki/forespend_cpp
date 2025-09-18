#include"scenes.h"
short* p;
bool vsync=true;
res resolutions[]={
	{360,240,"360x240"},
	{720,480,"720x480"},
	{1280,720,"1280x720"},
	{1920,1080,"1920x1080"}
	
};

Color TRUE={0,255,0,100};
Color FALSE={255,0,0,100};
menupage options{{//options
			
				new menubutton{ 0,40,360,20, 1, "graphics",
					[](){ *p=2; }},
					
				new menubutton{ 0,70,360,20, 2, "sound",
					[](){ *p=3; }},
					
				new menubutton{0,100,360,20, 1, "controls",
					[](){ *p=4; }},
					
				new menubutton{ 0,180,360,20, 2, "back",
					[](){ *p=0; }}
			
			},1};
menupage graphics {{//graphics
				new menucolor{ 0,0,360,20,0,&TRUE},
				new menubutton{ 0,0,360,20, 1, "internal res",
				
					[](){ cfg.internalres=!cfg.internalres;
					static_cast<menucolor*>(graphics.items[0])->value=cfg.internalres? &TRUE : &FALSE;
					}},
				new menucolor{ 0,20,360,20,0,&TRUE},
				new menubutton{ 0,20,360,20, 1, "vsync",
					[](){
					vsync=!vsync;
					SetTargetFPS(vsync?GetMonitorRefreshRate(GetCurrentMonitor()) :100000);
					static_cast<menucolor*>(graphics.items[2])->value=vsync? &TRUE : &FALSE;
					}},
				new menucolor{ 0,40,360,20,0,&FALSE},
				new menubutton{ 0,40,360,20, 1, "fullscreen",
					[](){ ToggleFullscreen();
					static_cast<menucolor*>(graphics.items[4])->value=IsWindowFullscreen() ? &TRUE : &FALSE;
					
					}},
				new menutext{ 0,70,140,20, 2, "res"},
				new menubutton{ 100,70,16,20, 3, "<",
				[](){}
				},
				new menutext{ 120,70,180,20, 4, "360x240"},
				new menubutton{ 310,70,16,20, 4, ">",
				[](){}
				},
				
				new menubutton{0,100,360,20, 1, "back",
					[](){ *p=1; }}
			
			},2};
menupage sound {{//sound
				new menubutton{ 0,40,360,20, 1, "music",
					[](){ }},
					
				new menubutton{ 0,70,360,20, 2, "sfx",
					[](){ }},
					
				new menubutton{0,100,360,20, 1, "back",
					[](){ *p=2; }}
			
			},3};

menupage controls {{//controls
				new menubutton{ 0,40,360,20, 1, "back",
					[](){ *p=1; }}
			
			},4};