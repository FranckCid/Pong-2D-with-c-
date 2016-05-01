#ifndef INTERFACE_H
#define INTERFACE_H

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class Interface
{
    public:
        TTF_Font *font;
        std::vector<SDL_Surface*> texts;
        std::vector<SDL_Rect> r_texts;
        Interface();
        void SetFont(const char* f, int s);
        void NewText(int x, int y, char* t, SDL_Color c);
        void UpdateText(int index, char* nt, SDL_Color c);
        void UpdateText(int index, int nt, SDL_Color c);
};

#endif // INTERFACE_H
