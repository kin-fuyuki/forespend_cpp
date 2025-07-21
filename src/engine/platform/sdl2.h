#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

//GR.H
//!TYPES
#define texturetype SDL_Texture*
#define windowtype SDL_Window*
#define renderertype SDL_Renderer*
#define killtexture SDL_DestroyTexture

//!FUNCTIONSZ
#define DRAWBUFFER \
	SDL_UpdateTexture(window->framebuffer, nullptr, pixelbuffer.data, pixelbuffer.width*4);\
	SDL_RenderCopy(window->renderer, window->framebuffer, nullptr, nullptr);
#define INITWINDOW SDL_Init(SDL_INIT_VIDEO); \
	window=SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);\
	renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);\
	framebuffer=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);

#define CLOSEWINDOW SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); SDL_Quit();
#define INITDRAW SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); \
	SDL_RenderClear(renderer);
#define ENDDRAW SDL_RenderPresent(renderer);
//SFX.H
//!TYPES
//!FUNCTIONS
