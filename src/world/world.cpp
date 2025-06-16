#include "world.h"
//#include <math.h>
#define M_PI 3.141592
#define DOUBLE_PI 6.283185
#define HALF_PI 1.570796
#define rad_to_deg 57.29
float sini(float x) {
  if (x < -M_PI)
        x += DOUBLE_PI;\
    else if (x > M_PI)
            x -= DOUBLE_PI;
    if (x < 0)
        return 1.2732f * x + 0.4052f * x * x;
    else
        return 1.2732f * x - 0.4052f * x * x;}
float cosi(float x) {
    return sini(x + HALF_PI);
}

float floatabs(float x) {
    return x < 0 ? -x : x;
}
float fminf(float a, float b) {
    return a < b ? a : b;
}
float fmaxf(float a, float b) {
    return a > b ? a : b;
}
float floatsqrt(float x) {
    long i;
    float x2, y;
    x2 = x * 0.5f;
    y = x;
    i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (1.5f - (x2 * y * y));
    return y;
}
#include <string>
#include <memory.h>
world test;
#ifdef RAYLIB
#include <raylib.h>
#define DRAWWORLD    UpdateTexture(worldbuffer, pixelbuffer);\
    DrawTexturePro(worldbuffer, Rectangle{0, 0, 380, 240}, Rectangle{0, 0,\ 
    (float)GetScreenWidth(), (float)GetScreenHeight()\
    }, Vector2{0, 0}, 0.0f, WHITE);\
    DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 20, WHITE);
#define INITWORLD Image i = GenImageColor(380, 240, WHITE);\
    test.worldbuffer = LoadTextureFromImage(i);\
    UnloadImage(i);
#elif defined(SDL2)
#define DRAWWORLD \
    SDL_UpdateTexture(test.worldbuffer, nullptr, pixelbuffer, 380 *4);\
    SDL_RenderCopy(window->renderer, worldbuffer, nullptr, nullptr);
#define INITWORLD \
    test.worldbuffer = SDL_CreateTexture(window->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 380, 240);

#else
    #ifdef _WIN32
#define DRAWWORLD 
#define INITWORLD 
    #else
    #define DRAWWORLD
    #define INITWORLD
    #endif

#define DRAWWORLD
#define INITWORLD
#endif


void init_world(Window* window)
{
    INITWORLD
    test.UUID = -1;
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            test.chunks[i][j].x = i;
            test.chunks[i][j].y = j;
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                for (int l = 0; l < CHUNK_SIZE; l++)
                {
                    // Calculate global coordinates
                    int global_x = i * CHUNK_SIZE + k;
                    int global_y = j * CHUNK_SIZE + l;
                    
                    // Create a sparser wall pattern
                    bool is_wall = false;
                    
                    // Border walls only at world edges
                    if (global_x == 0 || global_x == WORLD_SIZE * CHUNK_SIZE - 1 ||
                        global_y == 0 || global_y == WORLD_SIZE * CHUNK_SIZE - 1) {
                        is_wall = true;
                    }
                    // Sparse internal walls - much less dense pattern
                    else if ((global_x % 10 == 0 && global_y % 15 == 0) ||  (// Occasional intersections
                             (global_x % 25 == 0) &&                        // Vertical lines every 25 units
                             (global_y % 20 == 0))) {                        // Horizontal lines every 20 units
                        is_wall = true;
                    }
                    #define pathtocel test.chunks[i][j].cells[k][l]
                    pathtocel.height = is_wall ? 1 : 0;
                    pathtocel.type = (i + j + k + l) % 4;
                    pathtocel.textured=pathtocel.type==2||pathtocel.type==3||pathtocel.type==1;
                }
            }
        }
    }
}


void initlut() {
    
}

// Much more aggressive ray casting optimization
ray cast_ray(entity* player, world* world, float angle) {
    ray r = {};
    
    char x = player->x;
    char y = player->y;
    float cosin=angle*rad_to_deg;
    // Precompute direction
    float cos_angle = cosi(angle);
    float sin_angle = sini(angle);
    float dx = cos_angle * RAY_STEP;
    float dy = sin_angle * RAY_STEP;
    
    float total_distance = 0.0f;
    int type_index = 0;
    
    // Calculate world bounds once
    float world_size_limit = WORLD_SIZE * CHUNK_SIZE - 1.0f;
    
    // Use larger steps initially, then refine when close to walls
    float current_step = RAY_STEP * 2.0f; // Start with larger steps
    char prev_grid_x = (char)x;
    char prev_grid_y = (char)y;
    while (total_distance < MAX_RAY_DISTANCE) {
        x += dx;
        y += dy;
        total_distance += current_step;
        
        // Fast bounds check
        
        char grid_x = (char)x;
        char grid_y = (char)y;
        if (grid_x != prev_grid_x || grid_y != prev_grid_y){
        
        int chunk_x = grid_x >> 4;
        int chunk_y = grid_y >>4;
        int cell_x = grid_x &15;
        int cell_y = grid_y &15;
        if (chunk_x >= WORLD_SIZE || chunk_y >= WORLD_SIZE) {
            break;
        }
        
        auto& cell = world->chunks[chunk_x][chunk_y].cells[cell_x][cell_y];
        if (type_index < 79) {
            r.types[type_index++] = cell.type;
        }
        
        // Check for wall collision
        if (cell.height >= 1) {
            r.hit_type = cell.type;
            r.textured=cell.textured;
            r.distance = total_distance;
            if (r.textured){
            r.txx=(x-(float)grid_x)*8;}
            // Simple wall orientation based on ray direction
            r.hit_vertical_wall = (floatabs(cos_angle) > floatabs(sin_angle));
            
            return r;
        }
    }
        prev_grid_x = grid_x;
        prev_grid_y = grid_y;
    }
    
    r.distance = total_distance;
    return r;
}

static color tile_colors[4] = {
    {100, 100, 100, 255},  // Stone
    {139, 69, 19, 255},    // Wood
    {255, 0, 0, 255},      // Red brick
    {128, 128, 128, 255}   // Default gray
};
void minimap(world* world) {
    const int posx = 310;
    const int posy = 180;
    const int pixelsize = 1;
    int px=world->player.x;
    int py=world->player.y;
    const int MAP_SIZE = 50;
    for (int i = 0; i < MAP_SIZE+2; i++) {
        for (int j = 0; j < MAP_SIZE+2; j++) {
           
           
            if (i==0 || i==MAP_SIZE+1 || j==0 || j==MAP_SIZE+1)
            {
                world->pixelbuffer[(posy + j) * 380 + (posx + i)] = {127, 0, 127, 255};
            }
            else
            { int rx=px+i-25;int ry=py+j-25;
            if (rx<0 || rx>=WORLD_SIZE*CHUNK_SIZE ||
                ry<0 || ry>=WORLD_SIZE*CHUNK_SIZE) {
                continue;
            }
            int chunk_x = rx / CHUNK_SIZE;
            int chunk_y = ry / CHUNK_SIZE;
            int cell_x = rx % CHUNK_SIZE;
            int cell_y = ry % CHUNK_SIZE;
            auto& cell = world->chunks[chunk_x][chunk_y].cells[cell_x][cell_y];
            if (cell.height >= 1) {
                world->pixelbuffer[(posy + j) * 380 + (posx + i)] = tile_colors[cell.type];
            }else {
                world->pixelbuffer[(posy + j) * 380 + (posx + i)] = {63, 31, 0, 255};
            }}
        }
    }
    
    // Player position and direction
    int poscenter = (posx +25);
    
    //DrawRectangle(player_screen_x, player_screen_y, pixelsize * 2, pixelsize * 2, RED);
    // x+posx+mapsize/2 y+posy+mapsize/2
    // x,x+1,y,y+1
    world->pixelbuffer[poscenter * 380 + poscenter] = {255, 0, 0, 255};
    world->pixelbuffer[(poscenter + 1) * 380 + poscenter] = {255, 0, 0, 255};
    world->pixelbuffer[poscenter * 380 + poscenter + 1] = {255, 0, 0, 255};
    world->pixelbuffer[(poscenter + 1) * 380 + poscenter + 1] = {255, 0, 0, 255};
    
    // player dir
    int raydist=10;
    for (int i = 0; i < raydist; i++) {
        int rx = (int)(world->player.x + i * cosi(world->player.angle));
        int ry = (int)(world->player.y + i * sini(world->player.angle));
        int dx = rx - px + MAP_SIZE / 2;
        int dy = ry - py + MAP_SIZE / 2;

        if (dx < 0 || dx >= MAP_SIZE || dy < 0 || dy >= MAP_SIZE)
            continue;

        int x = posx + dx;
        int y = posy + dy;
        world->pixelbuffer[y * 380 + x] = {255, 0, 0, 255};
    }
}

#define FOV .9f


#include"..\gr\textures.h"
const color** types[4]={nullptr,lavatx,bricktx,grasstx};
inline color getTileColor(int tileType,bool textured, float brightness,char posx, char posy) {
    // Handle case where no wall was hit
    if (tileType == -1) {
        return color{0, 0, 0, 0}; // Black for debugging
    }
    if (textured){
        return *types[tileType][posy*8+posx];
    }
    // Clamp tile type to valid range
    int type = (tileType >= 0 && tileType < 4) ? tileType : 0;
    color& base = tile_colors[type];
    
    // Clamp brightness
    brightness = fmaxf(0.0f, fminf(1.0f, brightness));
    
    return color{
        (unsigned char)(base.r * brightness),
        (unsigned char)(base.g * brightness),
        (unsigned char)(base.b * brightness),
        (unsigned char)(255*brightness)
    };
}
inline color getTileColor(int tileType,float brightness) {
    return getTileColor(tileType,false,brightness,-1,-1);
}
void world::render(Window* window) {
        const int screenWidth = 380;
    const int screenHeight = 240;
    
    minimap(this);
    updatetextures();
    DRAWWORLD
}