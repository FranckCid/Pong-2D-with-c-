#include "GameObject.h"

#include "SDL/SDL.h"

GameObject::GameObject(){
    rect = {0,0,0,0};
}

bool GameObject::Intersects(int x, int y, int w, int h){
    if(rect.x >= x && rect.y >= y && rect.x <= h+w && rect.y <= y+h){
        return true;
    }
    return false;
}

bool GameObject::Intersects(SDL_Rect r){
    if((rect.x > r.x && rect.y > r.y && rect.x < r.x+r.w && rect.y < r.y+r.h) ||
       (r.x > rect.x && r.y > rect.y && r.x < rect.x+rect.w && r.y < rect.y+rect.h)){
        return true;
    }
    return false;
}
