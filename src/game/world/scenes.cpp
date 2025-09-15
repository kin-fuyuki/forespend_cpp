#include"scenes.h"
short* p;
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
				new menubutton{ 0,40,360,20, 1, "fullscreen",
					[](){ ToggleFullscreen();}},
					
				new menubutton{ 0,70,360,20, 2, "windowed",
					[](){ }},
					
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