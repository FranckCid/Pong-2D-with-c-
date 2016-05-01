#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include "Game.h"
#include "Interface.h"
#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"

#include <iostream>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

//Game Engine
SDL_Surface *screen;
SDL_Event occur;

//Colors
Uint32 white;
SDL_Color c_white, c_black;

//Interface
Interface interface;

//Geometrics
Paddle pPaddle;
Paddle aiPaddle;
Ball ball;

unsigned int direction_x = 1, direction_y = 1;

//Counters
bool shouldCheck = true;
unsigned int checkColAgain = 0;
const unsigned int limitToCheck = 100;

//Game objects
class Bound{
    public:
        int posx, posy, width, height, left, right, top, bottom;
        Bound(int x, int y, int w, int h){
            posx = x;
            posy = y;
            width = w;
            height = y;

            left = x;
            right = x + w;
            top = y;
            bottom = y + h;
        }
};

//Logic methods

void CollisionCheck(){
    if(shouldCheck){
        if(pPaddle.Intersects(ball.rect) ||
           aiPaddle.Intersects(ball.rect)){
            direction_x *= -1;
            shouldCheck = false;
        }
        aiPaddle.rect.h = Game::PADDLE_HEIGHT;
    }else{
        checkColAgain++;
        if(checkColAgain > limitToCheck){
                checkColAgain = 0;
                shouldCheck = true;
        }
    }
}

void AI(){

    unsigned int _top = Game::SCREEN_H - Game::PADDLE_HEIGHT - 5;

    if(aiPaddle.rect.y >= 5){
        aiPaddle.rect.y += direction_y;
    }else if(aiPaddle.rect.y <= _top){
        aiPaddle.rect.y -= direction_y;
    }

    if(aiPaddle.rect.y < 5){
       aiPaddle.rect.y = 5;
    }else if(aiPaddle.rect.y > _top){
        aiPaddle.rect.y = _top;
    }

}

//Main methods
void LoadGame(){

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    c_white = {255, 255, 255};
    c_black = {0, 0, 0};

    interface.SetFont("arial.ttf", 64);

    interface.NewText(Game::SCREEN_W / 3, Game::SCREEN_H / 50, "0", c_white);
    interface.NewText(Game::SCREEN_W / 3 * 2, Game::SCREEN_H / 50, "0", c_white);

    SDL_WM_SetCaption("Pong", NULL);
    screen = SDL_SetVideoMode(Game::SCREEN_W, Game::SCREEN_H, 32, SDL_HWSURFACE);

    aiPaddle.rect.x = Game::SCREEN_W - 20 - Game::PADDLE_WIDTH;

    white = SDL_MapRGB(screen->format, 235, 235, 235);

    //Ball direction

    ball.ResetBall();

}

void Logic(){

    Uint8 *keystates = SDL_GetKeyState(NULL);

    if(keystates[SDLK_w] && pPaddle.rect.y > 5){
        pPaddle.rect.y -= 10;
    }else if(keystates[SDLK_s] && pPaddle.rect.y < 600 - Game::PADDLE_HEIGHT - 5){
        pPaddle.rect.y += 10;
    }

    if(ball.rect.y < 10 || ball.rect.y > Game::SCREEN_H - ball.rect.h - 10){
        direction_y *= -1;
    }
    if(ball.rect.x < 10 || ball.rect.x > Game::SCREEN_W - ball.rect.w -10){
        char s_score[10];
        if(ball.rect.x<10){
            Game::score1++;
            interface.UpdateText(0, Game::score1, c_white);
        }else if(ball.rect.x > Game::SCREEN_W - ball.rect.w -10){
            Game::score2++;
            interface.UpdateText(1, Game::score2, c_white);
        }
        ball.ResetBall();
    }



    ball.rect.x += direction_x*5;
    ball.rect.y += direction_y*5;

}

void DrawScreen(){

    SDL_FillRect(screen, NULL, 0);


    SDL_FillRect(screen, &pPaddle.rect, white);
    SDL_FillRect(screen, &aiPaddle.rect, white);
    SDL_FillRect(screen, &ball.rect, white);

    for(int i=0; i<interface.texts.size(); i++){
        SDL_BlitSurface(interface.texts[i], NULL, screen, &interface.r_texts[i]);
    }

    SDL_Flip(screen);

}

void Quit(){
    for(int i=0; i<interface.texts.size(); i++){
        SDL_FreeSurface(interface.texts[i]);
    }
    SDL_FreeSurface(screen);
    TTF_CloseFont(interface.font);
    TTF_Quit();
    SDL_Quit();
}

int main ( int argc, char** argv ){

    const int FPS = 30;
    Uint32 start;
    float frame=0;

    LoadGame();

    while(1){

        start = SDL_GetTicks();

        SDL_PollEvent(&occur);

        if(occur.type == SDL_QUIT){
            break;
        }

        Logic();
        CollisionCheck();
        AI();
        DrawScreen();

        frame += 0.2;
        if(frame > 10){
            frame = 0;
        }
        if(1000/FPS > SDL_GetTicks()-start){
            SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
        }

    }

    Quit();

    return 0;
}

