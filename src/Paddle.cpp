#include "Paddle.h"

#include "Game.h"
#include "GameObject.h"

Paddle::Paddle(){
    rect.x = 20;
    rect.y = 250;
    rect.h = Game::PADDLE_HEIGHT;
    rect.w = Game::PADDLE_WIDTH;
}
