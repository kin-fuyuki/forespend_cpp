#include<cstdio>
#include<stdint.h>
// -127 to 127
float sin[255];
float cos[255];
float halfpi = 3.14159 / 2;
float pi = 3.14159;
float abs(float x) {
	return x > 0 ? x : -x;
}
float fastsin(float x) {
    const float B = 4/pi;
    const float C = -4/(pi*pi);

    float y = B * x + C * x * abs(x);
	return y;
}
float fastcos(float x) {
	return fastsin(x + halfpi);
}

void initsincos() {
	for (int i = 0; i < 255; i++) {
		float angle = (i / 255.0f) * 2 * pi - pi; 
		sin[i] = fastsin(angle);
		cos[i] = fastcos(angle);
	}
}


int main(int argc, char* argv[]) {
	printf("unsigned int64_t lutx[255] = {\n");
	int64_t x64 = 0;
	int64_t y64 = 0; 
	initsincos();
	float x = 0;
	float y = 0;
	
	for (int i = 0; i < 255; i++) {
		bool neg=false;
		for (int j = 0; j < 63; j++) {
			x += sin[i];
			if (x>=1 || x<=-1)x=0;
			if (x<0){
				neg=true;}
			//pack x abs as bit to x64, since x64 is a bit array
			char bit = x>0?1:0;
			x64=(x64<<1)|bit;
		}
		if (neg){
			x64 =-x64;
		}
		x=0;
		printf("%lld,",(long long) x64);
	}
	printf("};\n");
	printf("unsigned int64_t luty[255] = {\n");
	return 0;	
}