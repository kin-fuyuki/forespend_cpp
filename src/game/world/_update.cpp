#include"world.h"

#ifndef WORLDUPDATE
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
			//updatetextures();
			hourcycle+=0.1;
			if (hourcycle>=1.){
				hourcycle=0;
				daytime=daytime==5?daytime=0:daytime+1;
			}
		}
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


#endif
