#define SDL2
#ifdef RAYLIB
#define SETFLAGS SetConfigFlags(FLAG_WINDOW_RESIZABLE);
#define SHOULDCLOSE !WindowShouldClose()
#define HANDLE_EVENTS

#elif defined(SDL2)
#define SDL_MAIN_HANDLED
#define SETFLAGS bool running = true;
#define SHOULDCLOSE running
#define HANDLE_EVENTS \
    SDL_Event e; \
    while (SDL_PollEvent(&e)) { \
        if (e.type == SDL_QUIT) { \
            running = false; \
        } \
    }if (mustupdate){
#elif defined(SDL1)
#define SETFLAGS int running = 1;
#define SHOULDCLOSE running
#define HANDLE_EVENTS \
    SDL_Event e; \
    while (SDL_PollEvent(&e)) { \
        if (e.type == SDL_QUIT) { \
            running = 0; \
        } \
    }

#else
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
static int running = 1;
#define SETFLAGS
#define SHOULDCLOSE running
#define HANDLE_EVENTS \
    MSG msg; \
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { \
        if (msg.message == WM_QUIT) { \
            running = 0; \
        } else { \
            TranslateMessage(&msg); \
            DispatchMessage(&msg); \
        } \
    }
#else
#define SETFLAGS
#define SHOULDCLOSE true
#define HANDLE_EVENTS
#endif
#endif
#include "incmgr.h"
#define T true
#define F false
#define SIZE 64
#define FOV (60 * (PI / 180))
#define player test.player
ErrorLevel level(2);
#include <x86intrin.h>

int main(int argc, char* argv[]) {
    player.x=1.0f;
    player.y=1.0f;
    if (argc > 1)if (argv[1][0] == 'd')level.setvalue(4);
    success("forespend %s",VERSION);
    SETFLAGS
    Window window(1500, 800, "raycaster");
    //set resizeable
    init_world(&window);
    
    
#ifdef SDL2 
int frame_count = 0;
float fps = 0.0f;
Uint32 last_time = SDL_GetTicks();
const int TARGET_FPS = 30;
const int FRAME_DELAY = 1000 / TARGET_FPS;
/*
//benchmark
long scorecast=0;
int amount=0;
int results[100];
while (amount<100){
    amount++;    
unsigned long long start = __rdtsc();
    while(scorecast<380){
    scorecast++;
    
    cast_ray(&player, &test, 1.0f);
}
unsigned long long end = __rdtsc();
results[amount-1]=end-start;
scorecast=0;
}
//get smallest
int smallest=results[0];
for (int i = 1; i < 100; i++) {
    if (results[i] < smallest) {
        smallest = results[i];
    }
}
printf("smallest: %d\n",smallest/380);
return 0;
//*/
#endif
while (SHOULDCLOSE) {
    bool mustupdate = handleMovement(&test);
    mustupdate=true;
    HANDLE_EVENTS
        window.bind(world::draw, &test);
        window.update();
#ifdef SDL2
}
    Uint32 frame_start = SDL_GetTicks();

    frame_count++;

    Uint32 current_time = SDL_GetTicks();
    if (current_time - last_time >= 1000) {
        fps = frame_count * 1000.0f / (current_time - last_time);
        printf("fps: %f\n",frame_count);
        frame_count = 0;
        last_time = current_time;
    }

    // frame limiting
    Uint32 frame_time = SDL_GetTicks() - frame_start;
    if (frame_time < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frame_time);
    }
#endif
}
    


    return 0;
}


