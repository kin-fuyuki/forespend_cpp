#include "world.h"
std::vector<vec3> bake(std::vector<tile>* tiles) {

}
map::map() {
	size=128;
	world=0;
	paused=false;
	tiles.resize(size*size);
	for (x=0;x<size;x++) {
		for (y=0;y<size;y++) {
			tiles[x*size+y].type=0;
			tiles[x*size+y].height=0;
			tiles[x*size+y].hasbillboard=false;
		}
	}
}
map::~map() {}

void map::updateChunks() {}
void map::render() {}
void map::init() {

}
void map::update() {

}
void map::close() {}

//----------------------------

mainmenu::mainmenu() {}
mainmenu::~mainmenu() {}

void mainmenu::init() {}
void mainmenu::update() {}
void mainmenu::render() {}
void mainmenu::close() {}