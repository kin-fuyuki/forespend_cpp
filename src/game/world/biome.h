// 64 biomes
/*

~!~⁡		⁣⁢~!white⁢~!brightbluebg																				 	~!!~
																														
vertical=heat																											
horizontal=humidity																										
																														
	╔════════════════════════════════════════════════════════════════════════════════════════════════════|-|H|X|╗		
	║ biomes table																								║		
	╠═══════════════════════════════════════════════════════════════════════════════════════════════════════════╣		
	║	◎=seas		◯=lakes		▣=lava oceans		□=lava lakes	n=vegeration density						║		
	║												  cold														║		
	║		   ╔════════════════════════════════════════╦═══════════════════════════════════════════╗			║		
	║		   ║ colddesert		0	permafrost		   0| snowyhills		0	glaciers		◯◎ 0║			║		
	║		   ║ arctictundra	1	tundra 			◯  1| alpinetundra	◯	2	taiga			◯◎ 1║			║		
	║		   ║ rockdesert □◯	0	openshrubland 	◯  1| borealforest	◯◎	2	tempebroad		◯◎ 2║			║		
	║		   ║ semi-arid	□	1	grassland 		◯  1| temperatecone	◯◎	2	woodland		◯◎ 2║			║		
	║	dry	   ╠----------------------------------------+-------------------------------------------╣  wet		║		
	║		   ║ desert		□	0	savanna			□◯ 2| mediterranean	◯◎	3	rainforest		◯◎ 4║			║		
	║		   ║ saline		□▣	0	tropicalsavanna	□◯ 1| tropical		◯◎	2	tropicalrain	◯◎ 4║			║		
	║		   ║ scorched	□▣	0	lumen			□▣ 0| badlands		□◯	1	mangrove		◯◎ 2║			║		
	║		   ║ boiling	□▣	0	moltenruins		□▣ 0| ruins			□▣◯	0	mist			□◯ 1║			║		
	║		   ╚════════════════════════════════════════╩═══════════════════════════════════════════╝			║		
	║													hot														║		
	║																											║		
	╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════╝		
																														
																														
~!~!!white	⁡!!brightbluebg																	  						 ~!!~
*/

unsigned char colddesert(int x,int z,unsigned char population);
unsigned char permafrost(int x,int z,unsigned char population);
unsigned char snowyhills(int x,int z,unsigned char population);
unsigned char glaciers(int x,int z,unsigned char population);

unsigned char arctictundra(int x,int z,unsigned char population);
unsigned char tundra(int x,int z,unsigned char population);
unsigned char alpinetundra(int x,int z,unsigned char population);
unsigned char taiga(int x,int z,unsigned char population);

unsigned char rockydesert(int x,int z,unsigned char population);
unsigned char openshrubland(int x,int z,unsigned char population);
unsigned char borealforest(int x,int z,unsigned char population);
unsigned char tempebroad(int x,int z,unsigned char population);

unsigned char semi_arid(int x,int z,unsigned char population);
unsigned char grassland(int x,int z,unsigned char population);
unsigned char temperatecone(int x,int z,unsigned char population);
unsigned char woodland(int x,int z,unsigned char population);

//----------------------------------------------------------------

unsigned char desert(int x,int z,unsigned char population);
unsigned char savanna(int x,int z,unsigned char population);
unsigned char mediterranean(int x,int z,unsigned char population);
unsigned char rainforest(int x,int z,unsigned char population);

unsigned char saline(int x,int z,unsigned char population);
unsigned char tropicalsavanna(int x,int z,unsigned char population);
unsigned char tropical(int x,int z,unsigned char population);
unsigned char tropicalrain(int x,int z,unsigned char population);

unsigned char scorched(int x,int z,unsigned char population);
unsigned char lumen(int x,int z,unsigned char population);
unsigned char badlands(int x,int z,unsigned char population);
unsigned char mangrove(int x,int z,unsigned char population);

unsigned char boiling(int x,int z,unsigned char population);
unsigned char moltenruins(int x,int z,unsigned char population);
unsigned char ruins(int x,int z,unsigned char population);
unsigned char mist(int x,int z,unsigned char population);



extern unsigned char (* genbiome [32])(int,int,unsigned char);