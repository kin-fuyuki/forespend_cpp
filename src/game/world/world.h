#include<enginend.h>

class map:public World{
	public:
		buffer pixelbuffer;
		map();
		map(int width, int height);
		~map();
		void render(Window* window) override;
		
};