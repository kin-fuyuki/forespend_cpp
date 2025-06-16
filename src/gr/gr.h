#pragma once
#ifdef RAYLIB
#define WINDOWINIT InitWindow(width, height, title);\
            SetTargetFPS(60);
#define WINDOWBEGINDRAW BeginDrawing();\
            ClearBackground(DARKGRAY);
#define WINDOWENDDRAW EndDrawing();
#define WINDOWCLOSE CloseWindow();
#define WINDOWDEF
#include<raylib.h>
#elif defined(SDL2)
    #include <SDL2/SDL.h>
    #define WINDOWDEF SDL_Window* window; SDL_Renderer* renderer;
    #define WINDOWINIT SDL_Init(SDL_INIT_VIDEO); \
                       window=SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN); \
                       renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    #define WINDOWBEGINDRAW SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); \
                            SDL_RenderClear(renderer);
    #define WINDOWENDDRAW SDL_RenderPresent(renderer);
    #define WINDOWCLOSE SDL_DestroyRenderer(renderer); \
                        SDL_DestroyWindow(window); \
                        SDL_Quit();
#else
    #ifdef _WIN32
    
    #include <windows.h>
    
#define WINDOWDEF
    #define WINDOWINIT 
#define WINDOWBEGINDRAW

#define WINDOWENDDRAW 
#define WINDOWCLOSE 
    #else

    #define WINDOWINIT
    #define WINDOWBEGINDRAW
    #define WINDOWENDDRAW
    #define WINDOWCLOSE
    #endif
#endif
#include <malloc.h>
#include <cstdio>
class Window;
#ifndef RAYLIB
typedef struct color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
};
#else
#define color Color
#endif
class Object{
public:
    int UUID;
    virtual void render(Window* window)=0;
    static void draw(Object* self,Window* window) {
        self->render(window); // sigsegv after this, the "render successful" message is printed
    }
};
class Object2D: public Object{
    public:
        float x, y;
        Object2D(float x, float y) {
            this->x = x;
            this->y = y;
        }
        void render(Window* window) {
        }

};

class Window {
    public:
        void (**drawfuncs)(Object*,Window*)=nullptr;
        Object** objects=nullptr;
        int draws = 0;
        WINDOWDEF
        Window(int width, int height, const char* title) {
            WINDOWINIT
        }
        ~Window() {
            WINDOWCLOSE
        }

        void update() {
            WINDOWBEGINDRAW
            for (int i = 0; i < draws; i++) {
                drawfuncs[i](objects[i], this);
            }
            drawfuncs = nullptr;
            objects = nullptr;
            draws = 0;
            WINDOWENDDRAW

        }

        void bind(void (*func)(Object*,Window*), Object* obj) {
            drawfuncs = (void(**)(Object*,Window*))realloc(drawfuncs, sizeof(void(*)(Object*)) * (draws + 1));
            drawfuncs[draws] = func;
            objects = (Object**)realloc(objects, sizeof(Object*) * (draws + 1));
            objects[draws] = obj;
            draws++;
        }

};