// here be shenegans
#ifdef __MMX__
#include <mmintrin.h>
extern __m64 	keys[4],changed[4];
#elif defined(__SSE__)
#include <xmmintrin.h>
extern __m128i 	keys[2],changed[2];
#elif defined(__SSE2__)
#include <emmintrin.h>
extern __m128i 	keys[2],changed[2];
#elif defined(__SSE3__)
#include <pmmintrin.h>
extern __m128i 	keys[2],changed[2];
#elif defined(__SSSE3__)
#include <tmmintrin.h>
extern __m128i 	keys[2],changed[2];
#elif defined(__SSE4_1__)
#include <smmintrin.h>
extern __m128i 	keys[2],changed[2];
#elif defined(__SSE4_2__)
#include <nmmintrin.h>
extern __m128i 	keys[2],changed[2];
#elif defined(__AVX__)
#include <immintrin.h>
extern __m256i 	keys,changed;
#elif defined(__AVX2__)
#include <immintrin.h>
extern __m256i 	keys,changed;
#else
uint32_t 	keys[8],changed[8];
#endif
void setkey(unsigned char k,bool state);
bool pressed(unsigned char k);
bool justchanged(unsigned char k);
void clearchanged();
void savebmp(const char* filename, unsigned char* data, int width, int height);

void format_number(long long num, char* buffer);
#define FORMAT_NUM(n) ({ \
	static char buf[32]; \
	format_number(n, buf); \
	buf; \
})