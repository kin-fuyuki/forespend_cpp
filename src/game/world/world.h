#include"../../engine/engine.h"

class map:public World{
	public:
		map();
		~map();
		void render(Window* window) override;
};