#include "Interface.h"

#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

Interface::Interface(){
}

void Interface::SetFont(const char* f, int s){
    font = TTF_OpenFont(f, s);
}

void Interface::NewText(int rx, int ry, char* t, SDL_Color c){
    texts.push_back(TTF_RenderText_Shaded(font, "0", c, {0,0,0}));
    r_texts.push_back({rx, ry, 0, 0});
}

void Interface::UpdateText(int i, char* nt, SDL_Color c){
    texts[i] = TTF_RenderText_Shaded(font, nt, c, {255,0,0.0});
}

void Interface::UpdateText(int i, int nt, SDL_Color c){
    char newtext[65];
    sprintf(newtext,"%d", nt);
    texts[i] = TTF_RenderText_Shaded(font, newtext, c, {255,0,0,0});
}

