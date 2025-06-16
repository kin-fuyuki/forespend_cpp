#include <vector>
struct action
{
    std::vector<int> keys;
    action(std::initializer_list<int> keys) : keys(keys) {}
};
//#define RAYLIB

#ifdef RAYLIB
#include <raylib.h>
#define GETKEY IsKeyDown(a.keys[i])
action forward = {KEY_W}; action backward = {KEY_S};
action strafeleft = {KEY_A}; action straferight = {KEY_D};
action left = {KEY_LEFT}; action right = {KEY_RIGHT};
action map = {KEY_TAB}; action close = {KEY_F12};
action run = {KEY_LEFT_SHIFT};
#elif defined(SDL2)
    #include <SDL2/SDL.h>
    #define GETKEY SDL_GetKeyboardState(NULL)[a.keys[i]]
    action forward = {SDL_SCANCODE_W}; action backward = {SDL_SCANCODE_S};
    action strafeleft = {SDL_SCANCODE_A}; action straferight = {SDL_SCANCODE_D};
    action left = {SDL_SCANCODE_LEFT}; action right = {SDL_SCANCODE_RIGHT};
    action map = {SDL_SCANCODE_TAB}; action close = {SDL_SCANCODE_F12};
    action run = {SDL_SCANCODE_LSHIFT};

#else
    #ifdef _WIN32
    #include <windows.h>
    #define GETKEY GetAsyncKeyState(a.keys[i]) & 0x8000
    action forward = {'w'}; action backward = {'s'};
    action strafeleft = {'a'}; action straferight = {'d'};
    action left = {VK_LEFT}; action right = {VK_RIGHT};
    action map = {VK_TAB}; action close = {VK_F12};
    action run = {VK_SHIFT};
    #endif
#endif

bool ispressed(action a)
{
    for (int i = 0; i < a.keys.size(); i++)
    {
        if (GETKEY)
            return true;
    }
    return false;
}