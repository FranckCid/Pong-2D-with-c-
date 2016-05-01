#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL/SDL.h>

class GameObject
{
    public:
        int dirx, diry;
        SDL_Rect rect;
        GameObject();
        bool Intersects(SDL_Rect r);
        bool Intersects(int x, int y, int w, int h);
};

#endif // GAMEOBJECT_H
