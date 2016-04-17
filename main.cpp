#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

const unsigned int SCREEN_W = 800, SCREEN_H = 600;

const unsigned int BALL_X = 390, BALL_Y = 290;

const unsigned int PADDLE_HEIGHT = 75, PADDLE_WIDTH = 20;

//Game Engine
SDL_Surface *screen;
SDL_Event occur;

//Colors
Uint32 white;
SDL_Color c_white, c_black;

//Interface
TTF_Font *font;
SDL_Rect l_score1, l_score2;
SDL_Surface *t_score1=NULL, *t_score2=NULL;
unsigned int score1 = 0, score2 = 0;

//Geometrics
SDL_Rect pPaddle;
SDL_Rect aiPaddle;
SDL_Rect ball;

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

//Util methods

bool intersects(Bound b1, Bound b2){

    if((b1.left < b2.right && b1.right > b2.left && b1.top < b2.bottom && b1.bottom > b2.top)){
        return true;
    }

    return false;

}

void reset_ball(){


    ball.x = BALL_X;
    ball.y = BALL_Y;

    srand(static_cast<unsigned int>(time(0)));

    if(rand()%100>50){
        direction_x*=-1;
    }
    if(rand()%50>25){
        direction_y*=-1;
    }
}

//Logic methods

void CollisionCheck(){
    if(shouldCheck){
        Bound ballBounds(ball.x, ball.y, ball.w, ball.h);
        if((intersects(Bound(pPaddle.x, pPaddle.y, pPaddle.w, pPaddle.h), ballBounds)) ||
            (intersects(Bound(aiPaddle.x, aiPaddle.y, aiPaddle.w, aiPaddle.h), ballBounds))){
            direction_x *= -1;
            shouldCheck = false;
        }
        aiPaddle.h = PADDLE_HEIGHT;
    }else{
        checkColAgain++;
        if(checkColAgain > limitToCheck){
                checkColAgain = 0;
                shouldCheck = true;
        }
    }
}

void AI(){

    unsigned int _top = SCREEN_H - PADDLE_HEIGHT - 5;

    if(aiPaddle.y >= 5){
        aiPaddle.y += direction_y;
    }else if(aiPaddle.y <= _top){
        aiPaddle.y -= direction_y;
    }

    if(aiPaddle.y < 5){
       aiPaddle.y = 5;
    }else if(aiPaddle.y > _top){
        aiPaddle.y = _top;
    }

}

//Main methods
void LoadGame(){

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    c_white = {255, 255, 255};
    c_black = {0, 0, 0};

    font = TTF_OpenFont("arial.ttf", 64);

    t_score1 = TTF_RenderText_Shaded(font, "0", c_white, c_black);
    t_score2 = TTF_RenderText_Shaded(font, "0", c_white, c_black);

    l_score1 = {SCREEN_W / 3, SCREEN_H / 50, 0, 0};
    l_score2 = {SCREEN_W / 3 * 2, SCREEN_H / 50, 0, 0};

    SDL_WM_SetCaption("Pong", NULL);
    screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE);

    pPaddle.x = 20;
    pPaddle.y = 250;
    pPaddle.h = PADDLE_HEIGHT;
    pPaddle.w = PADDLE_WIDTH;

    aiPaddle.x = 760;
    aiPaddle.y = 250;
    aiPaddle.h = PADDLE_HEIGHT;
    aiPaddle.w = PADDLE_WIDTH;

    ball.h = 20;
    ball.w = 20;

    white = SDL_MapRGB(screen->format, 235, 235, 235);

    //Ball direction

    reset_ball();

}

void Logic(){

    Uint8 *keystates = SDL_GetKeyState(NULL);

    if(keystates[SDLK_w] && pPaddle.y > 5){
        pPaddle.y -= 1;
    }else if(keystates[SDLK_s] && pPaddle.y < 600 - PADDLE_HEIGHT - 5){
        pPaddle.y += 1;
    }

    if(ball.y < 10 || ball.y > SCREEN_H - ball.h - 10){
        direction_y *= -1;
    }
    if(ball.x < 10 || ball.x > SCREEN_W - ball.w -10){
        char s_score[10];
        if(ball.x<10){
            score1++;

            sprintf(s_score,"%d", score1);
            t_score2 = TTF_RenderText_Shaded(font, s_score, c_white, c_black);
        }else if(ball.x > SCREEN_W - ball.w -10){
            score2++;
            char s_score2[10];
            sprintf(s_score,"%d", score1);
            t_score1 = TTF_RenderText_Shaded(font, s_score, c_white, c_black);
        }
        reset_ball();
    }



    ball.x += direction_x;
    ball.y += direction_y;

}

void DrawScreen(){

    SDL_FillRect(screen, NULL, 0);


    SDL_FillRect(screen, &pPaddle, white);
    SDL_FillRect(screen, &aiPaddle, white);
    SDL_FillRect(screen, &ball, white);

    SDL_BlitSurface(t_score1, NULL, screen, &l_score1);
    SDL_BlitSurface(t_score2, NULL, screen, &l_score2);


    SDL_Flip(screen);

}

void Quit(){
    SDL_FreeSurface(t_score1);
    SDL_FreeSurface(t_score2);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

int main ( int argc, char** argv ){

    bool running = true;

    LoadGame();

    while(running){

        SDL_PollEvent(&occur);

        if(occur.type == SDL_QUIT){
            running = false;
        }

        Logic();
        CollisionCheck();
        AI();
        DrawScreen();

    }

    Quit();

    return 0;
}

