#include "Game.h"
#include "Ball.h"
#include "GameObject.h"

#include <ctime>

Ball::Ball(){
    rect.h = 20;
    rect.w = 20;
    dirx = 1;
    diry = 1;
}

void Ball::ResetBall(){

    rect.x = Game::BALL_X;
    rect.y = Game::BALL_Y;

    srand(static_cast<unsigned int>(time(0)));

    if(rand()%100>50){
        dirx*=-1;
    }
    if(rand()%50>25){
        diry*=-1;
    }

}


