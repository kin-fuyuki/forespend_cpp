#include"biome.h"
unsigned char (* genbiome [32])(int,int,unsigned char)={
	colddesert,permafrost,snowyhills,glaciers,
	arctictundra,tundra,alpinetundra,taiga,
	rockydesert,openshrubland,borealforest,tempebroad,
	semi_arid,grassland,temperatecone,woodland,
	desert,savanna,mediterranean,rainforest,
	saline,tropicalsavanna,tropical,tropicalrain,
	scorched,lumen,badlands,mangrove,
	boiling,moltenruins,ruins,mist
};

//~!~⁡⁣⁢~!white⁢~!brightredbg						~!!~
//	 tip: look at tilemap on side view on vscode	
//~!~⁡⁣⁢!!white⁢!!brightredbg						~!!~
// cold dry
	unsigned char colddesert(int x,int z,unsigned char population){
		return 8;
	}
	unsigned char permafrost(int x,int z,unsigned char population){
		return 8;
	}
	unsigned char arctictundra(int x,int z,unsigned char population){
		return x%2==0?8:9;
	}
	unsigned char tundra(int x,int z,unsigned char population){
		return 9;
	}
	unsigned char rockydesert(int x,int z,unsigned char population){
		return 11;
	}
	unsigned char openshrubland(int x,int z,unsigned char population){
		return 15+3;
	}
	unsigned char semi_arid(int x,int z,unsigned char population){
		return 15+3;
	}
	unsigned char grassland(int x,int z,unsigned char population){
		return 1;
	}
// cold wet
	unsigned char snowyhills(int x,int z,unsigned char population){
		return 8;
		
	}
	unsigned char glaciers(int x,int z,unsigned char population){
		return 7;
	}
	unsigned char alpinetundra(int x,int z,unsigned char population){
		return x%2==0?9:8;
		
	}
	unsigned char taiga(int x,int z,unsigned char population){
		return x%2==0?9:8;
		
	}
	unsigned char borealforest(int x,int z,unsigned char population){
		return x%2==0?9:10;
		
	}
	unsigned char tempebroad(int x,int z,unsigned char population){
		return 10;
	
	}
	unsigned char temperatecone(int x,int z,unsigned char population){
		return x%2==0?1:10;
		
	}
	unsigned char woodland(int x,int z,unsigned char population){
		return 1;
		
	}
// warm dry
	unsigned char desert(int x,int z,unsigned char population){
		return 15+3;
		
	}
	unsigned char savanna(int x,int z,unsigned char population){
		return 15+3;
		
	}
	unsigned char saline(int x,int z,unsigned char population){
		return 15+1;
		
	}
	unsigned char tropicalsavanna(int x,int z,unsigned char population){
		return 1;
	}
	unsigned char scorched(int x,int z,unsigned char population){
		return 5;
	}
	unsigned char lumen(int x,int z,unsigned char population){
		return 5;
		
	}
	unsigned char boiling(int x,int z,unsigned char population){
		return 15+32+4;
	}
	unsigned char moltenruins(int x,int z,unsigned char population){
		return x%2==0?15+32+4:254;
		
	}
// warm wet
	unsigned char mediterranean(int x,int z,unsigned char population){
		return 1;
		
	}
	unsigned char rainforest(int x,int z,unsigned char population){
		return 1;
		
	}
	unsigned char tropical(int x,int z,unsigned char population){
		return 1;
		
	}
	unsigned char tropicalrain(int x,int z,unsigned char population){
		return 1;
		
	}
	unsigned char badlands(int x,int z,unsigned char population){
		return 15+1;
		
	}
	unsigned char mangrove(int x,int z,unsigned char population){
		return 1;
		
	} 
	unsigned char ruins(int x,int z,unsigned char population){
		return 15+4;
		
	}
	unsigned char mist(int x,int z,unsigned char population){
		return 6;
		
	}