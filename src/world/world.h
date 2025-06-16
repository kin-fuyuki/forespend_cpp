#pragma once
#include"../gr/gr.h"
#define CHUNK_SIZE 16
#define WORLD_SIZE 16
#ifdef RAYLIB
#define TX2D Texture2D
#define KILLTX2D UnloadTexture
#elif defined(SDL2)
#define TX2D SDL_Texture*
#define KILLTX2D SDL_DestroyTexture

#else
#ifdef _WIN32
struct Win32Bitmap {
    BITMAPINFO bmi;
    HBITMAP hBitmap;
    void* bitmap_data;
    HDC memDC;
};
#define TX2D Win32Bitmap*
void killtexture(Win32Bitmap* t);
#define KILLTX2D killtexture

#else
#define TX2D void*
#define KILLTX2D free
#endif
#endif

#define RAYS 200
#define MAX_RAY_DISTANCE 50.0f
#define RAY_STEP 0.12f  // Larger step for better performance

struct ray
{
    int types[80] = {-1};
    bool hit_vertical_wall;
    float distance;
    int hit_type = -1;
    bool textured;
    char txx;
};


struct point {
    float x, y;
    float height, base;
};
 struct entity {
    float x, y;
    float angle;
};
struct structure
{
    int x;
    int y;
    point *points;
};
struct cell
{
    short type;
    bool textured;
    char height;
};

struct chunk
{
    char x;
    char y;
    entity *entities;
    structure *structures;
    cell cells[CHUNK_SIZE][CHUNK_SIZE];
};
class world:public Object
{
public:
    color fog; // alpha will be used as density
    chunk chunks[WORLD_SIZE][WORLD_SIZE];
    entity player;
    color* pixelbuffer;
    TX2D worldbuffer;
    world(){
        pixelbuffer = (color*)malloc(380 * 240 * sizeof(color));
    };
    void render(Window* window) override;
    ~world() {
        free(pixelbuffer);
        KILLTX2D(worldbuffer);
    };
};

extern world test;

void init_world(Window* window);


ray cast_ray(entity* player, world* world, float angle) ;