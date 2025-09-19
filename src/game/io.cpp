#include"io.h"
#include"../term.h"
#ifdef __MMX__
__m64 		keys[4],changed[4];
#elif defined(__SSE__) || defined(__SSE2__) || defined(__SSE3__) || defined(__SSSE3__) || defined(__SSE4_1__) || defined(__SSE4_2__) || defined(__AVX__) || defined(__AVX2__)
__m128i 	keys[2],changed[2];
#elif defined(__AVX__) || defined(__AVX2__)
__m256i 	keys,changed;
#else
uint32_t 	keys[8],changed[8];
#endif

void setkey(unsigned char index,bool state) {
	#ifdef __MMX__
	char posinarray=index>>6;
    char bitpos=index&63;
	bool same=state==((_mm_cvtsi64_si32(keys[posinarray]) >> bitpos)&1);
	state?keys[posinarray]|=1<<bitpos:keys[posinarray]&=~(1<<bitpos);
	same?changed[posinarray]|=1<<bitpos:changed[posinarray]&=~(1<<bitpos);
	#elif defined(__SSE__) || defined(__SSE2__) || defined(__SSE3__) || defined(__SSSE3__) || defined(__SSE4_1__) || defined(__SSE4_2__) || defined(__AVX__) || defined(__AVX2__)
	char posinarray=index>>7;
	char bitpos=index&127;
	bool same=state==((_mm_cvtsi128_si32(keys[posinarray]) >> bitpos)&1);
	state?keys[posinarray]|=1<<bitpos:keys[posinarray]&=~(1<<bitpos);
	same?changed[posinarray]|=1<<bitpos:changed[posinarray]&=~(1<<bitpos);
	#elif defined(__AVX__) || defined(__AVX2__)
	bool same=state==((keys >> index)&1);
	state?keys|=1<<index:keys&=~(1<<index);
	same?changed|=1<<index:changed&=~(1<<index);
	#else
	char posinarray=index>>5;
	char bitpos=index&31;
	bool same=state==((keys[posinarray] >> bitpos)&1);
	state?keys[posinarray]|=1<<bitpos:keys[posinarray]&=~(1<<bitpos);
	same?changed[posinarray]|=1<<bitpos:changed[posinarray]&=~(1<<bitpos);
	#endif
}
bool pressed(unsigned char index) {
	#ifdef __MMX__
	char posinarray=index>>6;
	char bitpos=index&63;
	return (_mm_cvtsi64_si32(keys[posinarray]) >> bitpos)&1;
	#elif defined(__SSE__) || defined(__SSE2__) || defined(__SSE3__) || defined(__SSSE3__) || defined(__SSE4_1__) || defined(__SSE4_2__) || defined(__AVX__) || defined(__AVX2__)
	char posinarray=index>>7;
	char bitpos=index&127;
	return (_mm_cvtsi128_si32(keys[posinarray]) >> bitpos)&1;
	#elif defined(__AVX__) || defined(__AVX2__)
	return (keys >> index)&1;
	#else
	char posinarray=index>>5;
	char bitpos=index&31;
	return (keys[posinarray] >> bitpos)&1;
	#endif
}
bool justchanged(unsigned char index) {
	#ifdef __MMX__
	char posinarray=index>>6;
	char bitpos=index&63;
	return (_mm_cvtsi64_si32(changed[posinarray]) >> bitpos)&1;
	#elif defined(__SSE__) || defined(__SSE2__) || defined(__SSE3__) || defined(__SSSE3__) || defined(__SSE4_1__) || defined(__SSE4_2__) || defined(__AVX__) || defined(__AVX2__)
	char posinarray=index>>7;
	char bitpos=index&127;
	return (_mm_cvtsi128_si32(changed[posinarray]) >> bitpos)&1;
	#elif defined(__AVX__) || defined(__AVX2__)
	return (changed >> index)&1;
	#else
	char posinarray=index>>5;
	char bitpos=index&31;
	return (changed[posinarray] >> bitpos)&1;
	#endif
}
void clearchanged() {
	#ifdef __MMX__
	changed[0] = _mm_setzero_si64();
	changed[1] = _mm_setzero_si64();
	changed[2] = _mm_setzero_si64();
	changed[3] = _mm_setzero_si64();
	#elif defined(__SSE__) || defined(__SSE2__) || defined(__SSE3__) || defined(__SSSE3__) || defined(__SSE4_1__) || defined(__SSE4_2__) || defined(__AVX__) || defined(__AVX2__)
	changed[0] = _mm_setzero_si128();
	changed[1] = _mm_setzero_si128();
	#elif defined(__AVX__) || defined(__AVX2__)
	changed=0;
	#else
	changed[0] = changed[1] = changed[2] = changed[3] = 0;
	changed[4] = changed[5] = changed[6] = changed[7] = 0;
	#endif
}
#include"scenes/world/world.h"
#include <cstdint>
#include <cstring>
void savebmp(const char* filename, unsigned char* data, int width, int height) {
	FILE* f = fopen(filename, "wb");
	if (!f) return;

	int rowpad = (width * 3 + 3) & (~3); 
	int datas = rowpad * height;
	int size = 54 + datas;

	
	uint8_t header[54] = {
		0x42, 0x4D,             
		0, 0, 0, 0,             
		0, 0, 0, 0,             
		54, 0, 0, 0,            
		40, 0, 0, 0,            
		0, 0, 0, 0,             
		0, 0, 0, 0,             
		1, 0,                   
		24, 0,                  
		0, 0, 0, 0,             
		0, 0, 0, 0,             
		0x13, 0x0B, 0, 0,       
		0x13, 0x0B, 0, 0,       
		0, 0, 0, 0,             
		0, 0, 0, 0              
	};

	
	header[2] = (uint8_t)(size);
	header[3] = (uint8_t)(size >> 8);
	header[4] = (uint8_t)(size >> 16);
	header[5] = (uint8_t)(size >> 24);

	
	header[18] = (uint8_t)(width);
	header[19] = (uint8_t)(width >> 8);
	header[20] = (uint8_t)(width >> 16);
	header[21] = (uint8_t)(width >> 24);

	
	header[22] = (uint8_t)(height);
	header[23] = (uint8_t)(height >> 8);
	header[24] = (uint8_t)(height >> 16);
	header[25] = (uint8_t)(height >> 24);

	fwrite(header, 1, 54, f);

	
	unsigned char* row = (unsigned char*)malloc(rowpad);
	for (int y = height - 1; y >= 0; y--) {
		for (int x = 0; x < width; x++) {
			int i = (y * width + x) * 3;
			row[x * 3 + 0] = data[i + 2]; 
			row[x * 3 + 1] = data[i + 1]; 
			row[x * 3 + 2] = data[i + 0]; 
		}
		
		for (int p = width * 3; p < rowpad; p++)
			row[p] = 0;

		fwrite(row, 1, rowpad, f);
	}

	free(row);
	fclose(f);
}

void format_number(long long num, char* buffer) {
	char temp[32];
	sprintf(temp, "%lld", num);
	int len = strlen(temp);
	int pos = 0;
	
	for (int i = 0; i < len; i++) {
		if (i > 0 && (len - i) % 3 == 0) {
			buffer[pos++] = '\'';
		}
		buffer[pos++] = temp[i];
	}
	buffer[pos] = '\0';
}