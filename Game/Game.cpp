#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>;
#include <SDL_ttf.h>;
#include "defs.h"
#include "graphics.h"
#include <vector>
#include "game.h"
#include "monster.h";
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include "chaanimation.h"
#define FIRELIMIT 30;
using namespace std;

void logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    // SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
     //full screen
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());
    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
        logErrorAndExit("SDL_image error:", IMG_GetError());


    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy trong máy ảo (ví dụ phòng máy ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    return renderer;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    IMG_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0 &&
            (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(50);
    }
}


int main(int argc, char* argv[])
{
    srand(time(NULL));
    Graphics graphics;
    graphics.init();
    SDL_Texture* background = loadTexture("img//map1.png", graphics.renderer);
    SDL_Texture* heart = loadTexture("img/heart.png", graphics.renderer);
    SDL_Texture* heartless = loadTexture("img/heartless.png", graphics.renderer);
    SDL_Texture* burning = loadTexture("img//Fire.png",graphics.renderer);
    SDL_Texture* keyimg = loadTexture(KEYIMAGE,graphics.renderer);
    SDL_Texture* warning = loadTexture("img\\Warning.png", graphics.renderer);
    Key key;
    key.x = 370;
    key.y = 280;
    key.locx= 408;
    key.locy = 308;
    int firestate = 0;
    //375//277
    Sprite idle;
    SDL_Texture* chatexture = graphics.loadTexture(CHARACTERFILE);
    idle.init(chatexture, IDLE_FRAME, IDLE_CLIPS);
    Sprite moveright;
    moveright.init(chatexture, MOVERIGHT_FRAME, MOVERIGHT_CLIPS);
    Sprite moveup;
    moveup.init(chatexture, MOVEUP_FRAME, MOVEUP_CLIPS);
    Sprite moveleft;
    moveleft.init(chatexture, MOVELEFT_FRAME, MOVELEFT_CLIPS);
    Sprite movedown;
    movedown.init(chatexture, MOVEDOWN_FRAME, MOVEDOWN_CLIPS);
    Sprite slashleft;
    slashleft.init(chatexture, SLASHLEFT_FRAME, SLASHLEFT_CLIPS);
    Sprite slashdown;
    slashdown.init(chatexture, SLASHDOWN_FRAME, SLASHDOWN_CLIPS);
    Sprite slashright;
    slashright.init(chatexture, SLASHRIGHT_FRAME, SLASHRIGHT_CLIPS);
    Sprite slashup;
    slashup.init(chatexture, SLASHUP_FRAME, SLASHUP_CLIPS);
    Sprite chadeath;
    chadeath.init(chatexture, CHADEATH_FRAME, CHADEATH_CLIPS);
    Sprite slimeidle1;
    SDL_Texture* slime = graphics.loadTexture(SLIMEFILE);
    slimeidle1.init(slime, SLIME_IDLE_RIGHT_FRAME, SLIME_IDLE_RIGHT_CLIPS);
    Sprite slimeidle2;
    slimeidle2.init(slime, SLIME_IDLE_LEFT_FRAME, SLIME_IDLE_LEFT_CLIPS);
    Sprite slimeright1;
    slimeright1.init(slime, SLIME_MOVE_RIGHT_FRAME, SLIME_MOVE_RIGHT_CLIPS);
    Sprite slimeleft1;
    slimeleft1.init(slime, SLIME_MOVE_LEFT_FRAME, SLIME_MOVE_LEFT_CLIPS);
    Sprite slimeright2;
    slimeright2.init(slime, SLIME_MOVE_RIGHT_FRAME, SLIME_MOVE_RIGHT_CLIPS);
    Sprite slimeleft2;
    slimeleft2.init(slime, SLIME_MOVE_LEFT_FRAME, SLIME_MOVE_LEFT_CLIPS);
    Sprite slimeright3;
    slimeright3.init(slime, SLIME_MOVE_RIGHT_FRAME, SLIME_MOVE_RIGHT_CLIPS);
    Sprite slimeleft3;
    slimeleft3.init(slime, SLIME_MOVE_LEFT_FRAME, SLIME_MOVE_LEFT_CLIPS);
    Sprite slimedeath1;
    slimedeath1.init(slime, SLIME_DEATH_FRAME, SLIME_DEATH_CLIPS);
    Sprite slimedeath2;
    slimedeath2.init(slime, SLIME_DEATH_FRAME, SLIME_DEATH_CLIPS);
    Sprite slimedeath3;
    slimedeath3.init(slime, SLIME_DEATH_FRAME, SLIME_DEATH_CLIPS);
    bool isfacingup = 0;
    bool isfacingdown = 1;
    bool isfacingright = 0;
    bool isfacingleft = 0;
    Cha cha(65, 50);
    vector<fire> vecfire(30);

    //cha.x = SCREEN_WIDTH / 2;
   // cha.y = SCREEN_HEIGHT / 2;
    int enx = 65 + (rand() % 10 + 1) * INITIAL_SPEEDX * 7;
    int eny = 70 + (rand() % 5 + 1) * INITIAL_SPEEDY * 7;
    bool changedmap = 0;
    En slime1(65+ 2*INITIAL_SPEEDX * 7,  70 + 2*INITIAL_SPEEDY * 7,6,210,238);
    slime1.isfacingright = 1;
    En slime2(65 + 10 * INITIAL_SPEEDX * 7, 70 + 4 * INITIAL_SPEEDY * 7,10,718,373);
    slime2.isfacingright = 0;
    En slime3(65 + 0* INITIAL_SPEEDX * 7, 70 + 6 * INITIAL_SPEEDY * 7, 10,85,510);
    slime3.isfacingright = 1;
    int x;
    int y;
    bool quit = false;
    bool idling = 1;
    cha.heartamount = 3;
    int m = 1;
    int currentmap = 0;
    SDL_Event event;
    int flimit = rand() % FIRELIMIT + 1;
   /* Mix_Music* nc = graphics.loadMusic("audio\\nightdancer.mp3");
    Mix_Music* rn = graphics.loadMusic("audio\\renai.mp3");
    graphics.play(rn);*/

    while (!quit && !gameOver(cha)) {
        SDL_GetMouseState(&x, &y);
        //  cerr << x << "," << y;

        while (SDL_PollEvent(&event)) {
            if (changedmap)
            {
                switch (currentmap)
                {
                default:
                    changedmap = 0;
                    SDL_DestroyTexture(background);
                    background = loadTexture("img//map1.png", graphics.renderer);
                    break;
                case 1:
                    changedmap = 0;
                    SDL_DestroyTexture(background);
                    background = loadTexture("img//map1opened.png", graphics.renderer);
                    key.showing = 0;
                    break;
                case 2:
                    changedmap = 0;
                    key.obtained = 0;
                    SDL_DestroyTexture(background);
                    cha.newmap();
                    background = loadTexture("img//map2.png", graphics.renderer);
                    break;
                }
            }
            switch(currentmap)
            {
            case 0:
            case 1:
        {
        //start of map1 and zero
            switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_T)
                {
                    slime1.heartamount-=3;
                    slime2.heartamount-=3;
                    slime3.heartamount-=3;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    for (int i = 1;i <= 7;i++)
                    {

                        graphics.prepareScene(background);
                        graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                        if (key.showing)
                        {
                            graphics.renderTexture(keyimg, key.x, key.y);

                        }
                        if (isfacingup)
                        {
                            graphics.render(cha.x, cha.y, moveup);
                        }
                        else if (isfacingleft)
                        {
                            graphics.render(cha.x, cha.y, moveleft);

                        }
                        else if (isfacingdown)
                        {
                            graphics.render(cha.x, cha.y, movedown);
                        }
                        else
                        {
                            graphics.render(cha.x, cha.y, moveright);
                        }
                        if (slime1.isalive)
                        {
                            if (slime1.isfacingright)
                            {
                                slime1.turnEast();
                                slime1.move();
                                if (i >= 2 && i <= 4)
                                {
                                    graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                }
                                else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                if (i <= 6)slimeright1.tick();
                            }
                            else
                            {
                                slime1.turnWest();
                                slime1.move();
                                if (i >= 2 && i <= 4)
                                {
                                    graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                }
                                else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                if (i <= 6) slimeleft1.tick();
                            }
                        }
                        if (slime2.isalive)
                        {
                            if (slime2.isfacingright)
                            {
                                slime2.turnEast();
                                slime2.move();
                                if (i >= 2 && i <= 4)
                                {
                                    graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                }
                                else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                if (i <= 6) slimeright2.tick();
                            }
                            else
                            {
                                slime2.turnWest();
                                slime2.move();
                                if (i >= 2 && i <= 4)
                                {
                                    graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                }
                                else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                if (i <= 6) { slimeleft2.tick(); }
                            }
                        }
                        if (slime3.isalive)
                        {
                            if (slime3.isfacingright)
                            {
                                slime3.turnEast();
                                slime3.move();
                                if (i >= 2 && i <= 4)
                                {
                                    graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                }
                                else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                if (i <= 6)slimeright3.tick();
                            }
                            else
                            {
                                slime3.turnWest();
                                slime3.move();
                                if (i >= 2 && i <= 4)
                                {
                                    graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                }
                                else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                if (i <= 6) slimeleft3.tick();
                            }
                        }
                        if (key.showing)
                        {
                            graphics.renderTexture(keyimg, key.x, key.y);
                        }
                        graphics.presentScene();
                        
                        SDL_Delay(50);





                    }
                    if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive)
                    {
                        cha.heartamount--;
                    }
                    slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                    if (slime1.limitcount == 1)
                    {
                        slime1.isfacingright = !slime1.isfacingright;
                    }
                    slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                    if (slime2.limitcount == 1)
                    {
                        slime2.isfacingright = !slime2.isfacingright;
                    }
                    slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                    if (slime3.limitcount == 1)
                    {
                        slime3.isfacingright = !slime3.isfacingright;
                    }
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_Z)
                {
                    if (isfacingright)
                    {

                        if (slime1.isalive || slime2.isalive || slime3.isalive)
                        {
                            if (cha.slashthrough(slime1.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime1.heartamount--;
                            }
                            else if (cha.slashthrough(slime2.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime2.heartamount--;
                            }
                            else if (cha.slashthrough(slime3.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime3.heartamount--;
                            }
                        }
                        if (slime1.heartamount <= 0 && slime1.isalive)
                        {
                            slime1.canhurt = 0;
                        }
                        if (slime2.heartamount <= 0 && slime2.isalive)
                        {
                            slime2.canhurt = 0;
                        }
                        if (slime3.heartamount <= 0 && slime3.isalive)
                        {
                            slime3.canhurt = 0;
                        }
                        for (int i = 1;i <= 7;i++)
                        {
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            if (key.showing)
                            {
                                graphics.renderTexture(keyimg, key.x, key.y);

                            }
                            if (slime1.isalive)
                            {
                                if (slime1.isfacingright)
                                {
                                    slime1.turnEast();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                    if (i <= 6)slimeright1.tick();
                                }
                                else
                                {
                                    slime1.turnWest();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                    if (i <= 6) slimeleft1.tick();
                                }
                            }
                            if (slime2.isalive)
                            {
                                if (slime2.isfacingright)
                                {
                                    slime2.turnEast();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                    if (i <= 6) slimeright2.tick();
                                }
                                else
                                {
                                    slime2.turnWest();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                    if (i <= 6) { slimeleft2.tick(); }
                                }
                            }
                            if (slime3.isalive)
                            {
                                if (slime3.isfacingright)
                                {
                                    slime3.turnEast();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                    if (i <= 6)slimeright3.tick();
                                }
                                else
                                {
                                    slime3.turnWest();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                    if (i <= 6) slimeleft3.tick();
                                }
                            }
                            graphics.render(cha.x, cha.y, slashright);
                            if (i <= 4)
                            {
                                slashright.tick();
                            }
                            graphics.presentScene();
                            
                            SDL_Delay(50);

                        }
                        if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.canhurt || cha.ishurt(slime2.rect) && slime2.canhurt || cha.ishurt(slime3.rect) && slime3.canhurt)
                        {
                            cha.heartamount--;
                        }
                    }
                    else if (isfacingleft)
                    {
                        if (slime1.isalive || slime2.isalive || slime3.isalive)
                        {
                            if (cha.slashthrough(slime1.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime1.heartamount--;
                            }
                            else if (cha.slashthrough(slime2.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime2.heartamount--;
                            }
                            else if (cha.slashthrough(slime3.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime3.heartamount--;
                            }
                        }
                        if (slime1.heartamount <= 0 && slime1.isalive)
                        {
                            slime1.canhurt = 0;
                        }
                        if (slime2.heartamount <= 0 && slime2.isalive)
                        {
                            slime2.canhurt = 0;
                        }
                        if (slime3.heartamount <= 0 && slime3.isalive)
                        {
                            slime3.canhurt = 0;
                        }
                        for (int i = 1;i <= 7;i++)
                        {
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            if (key.showing)
                            {
                                graphics.renderTexture(keyimg, key.x, key.y);

                            }
                            if (slime1.isalive)
                            {
                                if (slime1.isfacingright)
                                {
                                    slime1.turnEast();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                    if (i <= 6)slimeright1.tick();
                                }
                                else
                                {
                                    slime1.turnWest();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                    if (i <= 6) slimeleft1.tick();
                                }
                            }
                            if (slime2.isalive)
                            {
                                if (slime2.isfacingright)
                                {
                                    slime2.turnEast();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                    if (i <= 6) slimeright2.tick();
                                }
                                else
                                {
                                    slime2.turnWest();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                    if (i <= 6) { slimeleft2.tick(); }
                                }
                            }
                            if (slime3.isalive)
                            {
                                if (slime3.isfacingright)
                                {
                                    slime3.turnEast();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                    if (i <= 6)slimeright3.tick();
                                }
                                else
                                {
                                    slime3.turnWest();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                    if (i <= 6) slimeleft3.tick();
                                }
                            }
                            if (slashleft.currentFrame != 1)
                            {
                                graphics.render(cha.x, cha.y, slashleft);
                            }
                            else
                            {
                                graphics.render(cha.x - 30, cha.y, slashleft);
                            }

                             if(i<=4) slashleft.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);
                        }
                        if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.canhurt || cha.ishurt(slime2.rect) && slime2.canhurt || cha.ishurt(slime3.rect) && slime3.canhurt)
                        {
                            cha.heartamount--;
                        }
                    }
                    else if (isfacingdown)
                    {
                        if (slime1.isalive || slime2.isalive || slime3.isalive)
                        {
                            if (cha.slashthrough(slime1.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime1.heartamount--;
                            }
                            else if (cha.slashthrough(slime2.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime2.heartamount--;
                            }
                            else if (cha.slashthrough(slime3.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime3.heartamount--;
                            }
                        }
                        for (int i = 1;i <= 7;i++)
                        {
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            if (key.showing)
                            {
                                graphics.renderTexture(keyimg, key.x, key.y);

                            }
                            if (slime1.isalive)
                            {
                                if (slime1.isfacingright)
                                {
                                    slime1.turnEast();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                    if (i <= 6)slimeright1.tick();
                                }
                                else
                                {
                                    slime1.turnWest();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                    if (i <= 6) slimeleft1.tick();
                                }
                            }
                            if (slime2.isalive)
                            {
                                if (slime2.isfacingright)
                                {
                                    slime2.turnEast();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                    if (i <= 6) slimeright2.tick();
                                }
                                else
                                {
                                    slime2.turnWest();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                    if (i <= 6) { slimeleft2.tick(); }
                                }
                            }
                            if (slime3.isalive)
                            {
                                if (slime3.isfacingright)
                                {
                                    slime3.turnEast();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                    if (i <= 6)slimeright3.tick();
                                }
                                else
                                {
                                    slime3.turnWest();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                    if (i <= 6) slimeleft3.tick();
                                }
                            }
                            graphics.render(cha.x, cha.y, slashdown); 
                           if(i<=4) slashdown.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);
                        }
                        if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive)
                        {
                            cha.heartamount--;
                        }
                    }
                    else if (isfacingup)
                    {
                        if (slime1.isalive || slime2.isalive || slime3.isalive)
                        {
                            if (cha.slashthrough(slime1.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime1.heartamount--;
                            }
                            else if (cha.slashthrough(slime2.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime2.heartamount--;
                            }
                            else if (cha.slashthrough(slime3.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime3.heartamount--;
                            }
                        }
                        for (int i = 1;i <= 7;i++)
                        {
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            if (key.showing)
                            {
                                graphics.renderTexture(keyimg, key.x, key.y);

                            }
                            if (slime1.isalive)
                            {
                                if (slime1.isfacingright)
                                {
                                    slime1.turnEast();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                    if (i <= 6)slimeright1.tick();
                                }
                                else
                                {
                                    slime1.turnWest();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                    if (i <= 6) slimeleft1.tick();
                                }
                            }
                            if (slime2.isalive)
                            {
                                if (slime2.isfacingright)
                                {
                                    slime2.turnEast();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                    if (i <= 6) slimeright2.tick();
                                }
                                else
                                {
                                    slime2.turnWest();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                    if (i <= 6) { slimeleft2.tick(); }
                                }
                            }
                            if (slime3.isalive)
                            {
                                if (slime3.isfacingright)
                                {
                                    slime3.turnEast();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                    if (i <= 6)slimeright3.tick();
                                }
                                else
                                {
                                    slime3.turnWest();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                    if (i <= 6) slimeleft3.tick();
                                }
                            }
                            graphics.render(cha.x, cha.y, slashup);

                          if(i<=4)  slashup.tick();

                            graphics.presentScene();
                            
                            SDL_Delay(50);
                        }
                        if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive)
                        {
                            cha.heartamount--;
                        }
                    }
                    else if (isfacingleft)
                    {
                        if (slime1.isalive || slime2.isalive || slime3.isalive)
                        {
                            if (cha.slashthrough(slime1.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime1.heartamount--;
                            }
                            else if (cha.slashthrough(slime2.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime2.heartamount--;
                            }
                            else if (cha.slashthrough(slime3.rect, isfacingright, isfacingup, isfacingleft, isfacingdown))
                            {
                                slime3.heartamount--;
                            }
                        }
                        for (int i = 1;i <= 7;i++)
                        {
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            if (key.showing)
                            {
                                graphics.renderTexture(keyimg, key.x, key.y);

                            }
                            if (slime1.isalive)
                            {
                                if (slime1.isfacingright)
                                {
                                    slime1.turnEast();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                    if (i <= 6)slimeright1.tick();
                                }
                                else
                                {
                                    slime1.turnWest();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                    if (i <= 6) slimeleft1.tick();
                                }
                            }
                            if (slime2.isalive)
                            {
                                if (slime2.isfacingright)
                                {
                                    slime2.turnEast();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                    if (i <= 6) slimeright2.tick();
                                }
                                else
                                {
                                    slime2.turnWest();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                    if (i <= 6) { slimeleft2.tick(); }
                                }
                            }
                            if (slime3.isalive)
                            {
                                if (slime3.isfacingright)
                                {
                                    slime3.turnEast();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                    if (i <= 6)slimeright3.tick();
                                }
                                else
                                {
                                    slime3.turnWest();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                    if (i <= 6) slimeleft3.tick();
                                }
                            }
                            graphics.render(cha.x, cha.y, slashleft);

                            if (i <= 4)  slashleft.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);
                        }
                        if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive)
                        {
                            cha.heartamount--;
                        }
                    }
                    slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                    if (slime1.limitcount == 1)
                    {
                        slime1.isfacingright = !slime1.isfacingright;
                    }
                    slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                    if (slime2.limitcount == 1)
                    {
                        slime2.isfacingright = !slime2.isfacingright;
                    }
                    slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                    if (slime3.limitcount == 1)
                    {
                        slime3.isfacingright = !slime3.isfacingright;
                    }
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_X)
                {
                    if (isfacingright)
                    {
                        if (cha.notborderjump('r'))
                        {
                            idling = 0;

                            for (int i = 1;i <= 14;i++)
                            {
                                cha.turnEast();
                                graphics.prepareScene(background);
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if (key.showing)
                                {
                                    graphics.renderTexture(keyimg, key.x, key.y);

                                }
                                cha.move();
                                
                                if (slime1.isalive)
                                {
                                    if (slime1.isfacingright)
                                    {
                                        if (i <= 7)
                                        {
                                            slime1.turnEast();
                                            slime1.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime1.x, slime1.y, slimeidle1);
                                        }
                                        else { graphics.render(slime1.x, slime1.y, slimeright1); }

                                        if (i <= 6)slimeright1.tick();
                                    }
                                    else
                                    {
                                        if (i <= 7)
                                        {
                                            slime1.turnWest();
                                            slime1.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime1.x, slime1.y, slimeidle2);
                                        }
                                        else { graphics.render(slime1.x, slime1.y, slimeleft1); }

                                        if (i <= 6) slimeleft1.tick();
                                    }
                                }
                                if (slime2.isalive)
                                {
                                    if (slime2.isfacingright)
                                    {
                                        if (i <= 7)
                                        {
                                            slime2.turnEast();
                                            slime2.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime2.x, slime2.y, slimeidle1);
                                        }
                                        else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                        if (i <= 6) slimeright2.tick();
                                    }
                                    else
                                    {
                                        if (i <= 7)
                                        {
                                            slime2.turnWest();
                                            slime2.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime2.x, slime2.y, slimeidle2);
                                        }
                                        else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                        if (i <= 6) { slimeleft2.tick(); }
                                    }
                                }
                                if (slime3.isalive)
                                {
                                    if (slime3.isfacingright)
                                    {
                                        if (i <= 7)
                                        {
                                            slime3.turnEast();
                                            slime3.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime3.x, slime3.y, slimeidle1);
                                        }
                                        else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                        if (i <= 6)slimeright3.tick();
                                    }
                                    else
                                    {
                                        if (i <= 7)
                                        {
                                            slime3.turnWest();
                                            slime3.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime3.x, slime3.y, slimeidle2);
                                        }
                                        else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                        if (i <= 6) slimeleft3.tick();
                                    }
                                }
                                if (i <= 7)
                                {
                                    graphics.render(cha.x, cha.y - 4 * i, moveright);
                                }
                                else
                                {
                                    graphics.render(cha.x, cha.y - (14 - i) * 4, moveright);
                                }
                                moveright.tick();
                                graphics.presentScene();
                                
                                SDL_Delay(35);


                            }
                     
                            slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                            if (slime1.limitcount == 1)
                            {
                                slime1.isfacingright = !slime1.isfacingright;
                            }
                            slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                            if (slime2.limitcount == 1)
                            {
                                slime2.isfacingright = !slime2.isfacingright;
                            }
                            slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                            if (slime3.limitcount == 1)
                            {
                                slime3.isfacingright = !slime3.isfacingright;
                            }
                            if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive)
                            {
                                cha.heartamount--;
                            }
                            idling = true;
                            isfacingright = true;
                            isfacingup = false;
                            isfacingdown = false;
                            isfacingleft = false;
                        }
                    }
                    else if (isfacingup)
                    {
                        if (cha.notborderjump('u'))
                        {
                            idling = 0;
                            for (int i = 1;i <= 14;i++)
                            {

                                cha.turnNorth();
                                graphics.prepareScene(background);
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if (key.showing)
                                {
                                    graphics.renderTexture(keyimg, key.x, key.y);

                                }
                                cha.move();
                               
                                if (slime1.isalive)
                                {
                                    if (slime1.isfacingright)
                                    {
                                        if (i <= 7)
                                        {
                                            slime1.turnEast();
                                            slime1.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime1.x, slime1.y, slimeidle1);
                                        }
                                        else { graphics.render(slime1.x, slime1.y, slimeright1); }

                                        if (i <= 6)slimeright1.tick();
                                    }
                                    else
                                    {
                                        if (i <= 7)
                                        {
                                            slime1.turnWest();
                                            slime1.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime1.x, slime1.y, slimeidle2);
                                        }
                                        else { graphics.render(slime1.x, slime1.y, slimeleft1); }

                                        if (i <= 6) slimeleft1.tick();
                                    }
                                }
                                if (slime2.isalive)
                                {
                                    if (slime2.isfacingright)
                                    {
                                        if (i <= 7)
                                        {
                                            slime2.turnEast();
                                            slime2.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime2.x, slime2.y, slimeidle1);
                                        }
                                        else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                        if (i <= 6) slimeright2.tick();
                                    }
                                    else
                                    {
                                        if (i <= 7)
                                        {
                                            slime2.turnWest();
                                            slime2.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime2.x, slime2.y, slimeidle2);
                                        }
                                        else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                        if (i <= 6) { slimeleft2.tick(); }
                                    }
                                }
                                if (slime3.isalive)
                                {
                                    if (slime3.isfacingright)
                                    {
                                        if (i <= 7)
                                        {
                                            slime3.turnEast();
                                            slime3.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime3.x, slime3.y, slimeidle1);
                                        }
                                        else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                        if (i <= 6)slimeright3.tick();
                                    }
                                    else
                                    {
                                        if (i <= 7)
                                        {
                                            slime3.turnWest();
                                            slime3.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime3.x, slime3.y, slimeidle2);
                                        }
                                        else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                        if (i <= 6) slimeleft3.tick();
                                    }
                                }
                                if (i <= 7)
                                {
                                    graphics.render(cha.x, cha.y - 4 * i, moveup);
                                }
                                else
                                {
                                    graphics.render(cha.x, cha.y - (14 - i) * 4, moveup);
                                }
                                moveup.tick();
                                graphics.presentScene();
                                
                                SDL_Delay(35);
                            }
                   
                            slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                            if (slime1.limitcount == 1)
                            {
                                slime1.isfacingright = !slime1.isfacingright;
                            }
                            slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                            if (slime2.limitcount == 1)
                            {
                                slime2.isfacingright = !slime2.isfacingright;
                            }
                            slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                            if (slime3.limitcount == 1)
                            {
                                slime3.isfacingright = !slime3.isfacingright;
                            }
                            if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive)
                            {
                                cha.heartamount--;
                            }
                            idling = true;
                            isfacingup = true;
                            isfacingright = false;
                            isfacingleft = false;
                            isfacingdown = false;
                        }
                    }
                    else if (isfacingdown)
                    {
                        if (cha.notborderjump('d'))
                        {
                            idling = 0;

                            for (int i = 1;i <= 14;i++)
                            {
                                cha.turnSouth();
                                graphics.prepareScene(background);
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if (key.showing)
                                {
                                    graphics.renderTexture(keyimg, key.x, key.y);

                                }

                                cha.move();
                               
    

                                if (slime1.isalive)
                                {
                                    if (slime1.isfacingright)
                                    {
                                        if (i <= 7)
                                        {
                                            slime1.turnEast();
                                            slime1.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime1.x, slime1.y, slimeidle1);
                                        }
                                        else { graphics.render(slime1.x, slime1.y, slimeright1); }

                                        if (i <= 6)slimeright1.tick();
                                    }
                                    else
                                    {
                                        if (i <= 7)
                                        {
                                            slime1.turnWest();
                                            slime1.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime1.x, slime1.y, slimeidle2);
                                        }
                                        else { graphics.render(slime1.x, slime1.y, slimeleft1); }

                                        if (i <= 6) slimeleft1.tick();
                                    }
                                }
                                if (slime2.isalive)
                                {
                                    if (slime2.isfacingright)
                                    {
                                        if (i <= 7)
                                        {
                                            slime2.turnEast();
                                            slime2.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime2.x, slime2.y, slimeidle1);
                                        }
                                        else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                        if (i <= 6) slimeright2.tick();
                                    }
                                    else
                                    {
                                        if (i <= 7)
                                        {
                                            slime2.turnWest();
                                            slime2.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime2.x, slime2.y, slimeidle2);
                                        }
                                        else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                        if (i <= 6) { slimeleft2.tick(); }
                                    }
                                }
                                if (slime3.isalive)
                                {
                                    if (slime3.isfacingright)
                                    {
                                        if (i <= 7)
                                        {
                                            slime3.turnEast();
                                            slime3.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime3.x, slime3.y, slimeidle1);
                                        }
                                        else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                        if (i <= 6)slimeright3.tick();
                                    }
                                    else
                                    {
                                        if (i <= 7)
                                        {
                                            slime3.turnWest();
                                            slime3.move();
                                        }
                                        if (i >= 2 && i <= 4)
                                        {
                                            graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                        }
                                        else if (i > 7)
                                        {
                                            graphics.render(slime3.x, slime3.y, slimeidle2);
                                        }
                                        else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                        if (i <= 6) slimeleft3.tick();
                                    }
                                }
                                if (i <= 7)
                                {
                                    graphics.render(cha.x, cha.y - 4 * i, movedown);
                                }
                                else
                                {
                                    graphics.render(cha.x, cha.y - (14 - i) * 4, movedown);
                                }
                                movedown.tick();
                                graphics.presentScene();
                                
                                SDL_Delay(35);


                            }
                 
                            slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                            if (slime1.limitcount == 1)
                            {
                                slime1.isfacingright = !slime1.isfacingright;
                            }
                            slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                            if (slime2.limitcount == 1)
                            {
                                slime2.isfacingright = !slime2.isfacingright;
                            }
                            slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                            if (slime3.limitcount == 1)
                            {
                                slime3.isfacingright = !slime3.isfacingright;
                            }
                            if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive)
                            {
                                cha.heartamount--;
                            }
                            idling = true;
                            isfacingdown = true;
                            isfacingleft = false;
                            isfacingright = false;
                            isfacingup = false;
                        }
                    }
                    else if (isfacingleft)
                    {
                            if (cha.notborderjump('l'))
                            {
                                idling = 0;

                                for (int i = 1;i <= 14;i++)
                                {
                                    graphics.prepareScene(background);
                                    graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                    if (key.showing)
                                    {
                                        graphics.renderTexture(keyimg, key.x, key.y);

                                    }
                                    cha.turnWest();

                                    cha.move();
                                    if (slime1.isalive)
                                    {
                                        if (slime1.isfacingright)
                                        {
                                            if (i <= 7)
                                            {
                                                slime1.turnEast();
                                                slime1.move();
                                            }
                                            if (i >= 2 && i <= 4)
                                            {
                                                graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                            }
                                            else if (i > 7)
                                            {
                                                graphics.render(slime1.x, slime1.y, slimeidle1);
                                            }
                                            else { graphics.render(slime1.x, slime1.y, slimeright1); }

                                            if (i <= 6)slimeright1.tick();
                                        }
                                        else
                                        {
                                            if (i <= 7)
                                            {
                                                slime1.turnWest();
                                                slime1.move();
                                            }
                                            if (i >= 2 && i <= 4)
                                            {
                                                graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                            }
                                            else if (i > 7)
                                            {
                                                graphics.render(slime1.x, slime1.y, slimeidle2);
                                            }
                                            else { graphics.render(slime1.x, slime1.y, slimeleft1); }

                                            if (i <= 6) slimeleft1.tick();
                                        }
                                    }
                                    if (slime2.isalive)
                                    {
                                        if (slime2.isfacingright)
                                        {
                                            if (i <= 7)
                                            {
                                                slime2.turnEast();
                                                slime2.move();
                                            }
                                            if (i >= 2 && i <= 4)
                                            {
                                                graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                            }
                                            else if (i > 7)
                                            {
                                                graphics.render(slime2.x, slime2.y, slimeidle1);
                                            }
                                            else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                            if (i <= 6) slimeright2.tick();
                                        }
                                        else
                                        {
                                            if (i <= 7)
                                            {
                                                slime2.turnWest();
                                                slime2.move();
                                            }
                                            if (i >= 2 && i <= 4)
                                            {
                                                graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                            }
                                            else if (i > 7)
                                            {
                                                graphics.render(slime2.x, slime2.y, slimeidle2);
                                            }
                                            else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                            if (i <= 6) { slimeleft2.tick(); }
                                        }
                                    }
                                    if (slime3.isalive)
                                    {
                                        if (slime3.isfacingright)
                                        {
                                            if (i <= 7)
                                            {
                                                slime3.turnEast();
                                                slime3.move();
                                            }
                                            if (i >= 2 && i <= 4)
                                            {
                                                graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                            }
                                            else if (i > 7)
                                            {
                                                graphics.render(slime3.x, slime3.y, slimeidle1);
                                            }
                                            else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                            if (i <= 6)slimeright3.tick();
                                        }
                                        else
                                        {
                                            if (i <= 7)
                                            {
                                                slime3.turnWest();
                                                slime3.move();
                                            }
                                            if (i >= 2 && i <= 4)
                                            {
                                                graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                            }
                                            else if (i > 7)
                                            {
                                                graphics.render(slime3.x, slime3.y, slimeidle2);
                                            }
                                            else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                            if (i <= 6) slimeleft3.tick();
                                        }
                                    }
                                    if (i <= 7)
                                    {
                                        graphics.render(cha.x, cha.y - 4 * i, moveleft);
                                    }
                                    else
                                    {
                                        graphics.render(cha.x, cha.y - (14 - i) * 4, moveleft);
                                    }
                                    moveleft.tick();
                                    graphics.presentScene();
                                    
                                    SDL_Delay(35);
                                }
                      
                                slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                                if (slime1.limitcount == 1)
                                {
                                    slime1.isfacingright = !slime1.isfacingright;
                                }
                                slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                                if (slime2.limitcount == 1)
                                {
                                    slime2.isfacingright = !slime2.isfacingright;
                                }
                                slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                                if (slime3.limitcount == 1)
                                {
                                    slime3.isfacingright = !slime3.isfacingright;
                                }
                                if (cha.touchacid() || cha.ishurt(slime1.rect)&&slime1.isalive || cha.ishurt(slime2.rect)&&slime2.isalive || cha.ishurt(slime3.rect)&&slime3.isalive)
                                {
                                    cha.heartamount--;
                                }
                                idling = true;
                                isfacingleft = true;
                                isfacingdown = false;
                                isfacingup = false;
                                isfacingright = false;
                            }
                        }

                }
                if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    if (cha.notborder('u'))
                    {
                        idling = 0;
                        for (int i = 1;i <= 7;i++)
                        {

                            cha.turnNorth();
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            if (key.showing)
                            {
                                graphics.renderTexture(keyimg, key.x, key.y);

                            }
                            cha.move();
                            graphics.render(cha.x, cha.y, moveup);
                            if (slime1.isalive)
                            {
                                if (slime1.isfacingright)
                                {
                                    slime1.turnEast();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                    if (i <= 6)slimeright1.tick();
                                }
                                else
                                {
                                    slime1.turnWest();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                    if (i <= 6) slimeleft1.tick();
                                }
                            }
                            if (slime2.isalive)
                            {
                                if (slime2.isfacingright)
                                {
                                    slime2.turnEast();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                    if (i <= 6) slimeright2.tick();
                                }
                                else
                                {
                                    slime2.turnWest();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                    if (i <= 6) { slimeleft2.tick(); }
                                }
                            }
                            if (slime3.isalive)
                            {
                                if (slime3.isfacingright)
                                {
                                    slime3.turnEast();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                    if (i <= 6)slimeright3.tick();
                                }
                                else
                                {
                                    slime3.turnWest();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                    if (i <= 6) slimeleft3.tick();
                                }
                            }
                            moveup.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);





                        }
             
                        slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                        if (slime1.limitcount == 1)
                        {
                            slime1.isfacingright = !slime1.isfacingright;
                        }
                        slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                        if (slime2.limitcount == 1)
                        {
                            slime2.isfacingright = !slime2.isfacingright;
                        }
                        slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                        if (slime3.limitcount == 1)
                        {
                            slime3.isfacingright =! slime3.isfacingright;
                        }
                        if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive)
                        {
                            cha.heartamount--;
                        }
                        idling = true;
                        isfacingup = true;
                        isfacingright = false;
                        isfacingleft = false;
                        isfacingdown = false;
                    }
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)

                {
                    if (cha.notborder('l'))
                    {
                        idling = 0;
                        bool collided = false;
                        for (int i = 1;i <= 7;i++)
                        {

                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            cha.turnWest();
                            if (key.showing)
                            {
                                graphics.renderTexture(keyimg, key.x, key.y);

                            }
                            cha.move();
                            graphics.render(cha.x, cha.y, moveleft);
                            if (slime1.isalive)
                            {
                                if (slime1.isfacingright)
                                {
                                    slime1.turnEast();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                    if (i <= 6)slimeright1.tick();
                                }
                                else
                                {
                                    slime1.turnWest();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                    if (i <= 6) slimeleft1.tick();
                                }
                            }
                            if (slime2.isalive)
                            {
                                if (slime2.isfacingright)
                                {
                                    slime2.turnEast();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                    if (i <= 6) slimeright2.tick();
                                }
                                else
                                {
                                    slime2.turnWest();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                    if (i <= 6) { slimeleft2.tick(); }
                                }
                            }
                            if (slime3.isalive)
                            {
                                if (slime3.isfacingright)
                                {
                                    slime3.turnEast();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                    if (i <= 6)slimeright3.tick();
                                }
                                else
                                {
                                    slime3.turnWest();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                    if (i <= 6) slimeleft3.tick();
                                }
                            }
                           if (i == 4 && !collided && (cha.ishurt(slime1.rect)&&slime1.isalive || cha.ishurt(slime2.rect)&&slime2.isalive || cha.ishurt(slime3.rect)&&slime3.isalive))
                       
                            {
                                cha.heartamount--;
                                collided = true;
                            }
                            moveleft.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);





                        }
               
                        slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                        if (slime1.limitcount == 1)
                        {
                            slime1.isfacingright = !slime1.isfacingright;
                        }
                        slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                        if (slime2.limitcount == 1)
                        {
                            slime2.isfacingright = !slime2.isfacingright;
                        }
                        slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                        if (slime3.limitcount == 1)
                        {
                            slime3.isfacingright =!slime3.isfacingright;
                        }
                        if (!collided&&(cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive))
                        {
                            cha.heartamount--;
                        }
                        idling = true;
                        isfacingleft = true;
                        isfacingdown = false;
                        isfacingup = false;
                        isfacingright = false;
                    }
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    if (cha.notborder('r'))
                    {
                        bool collided = false;
                        idling = 0;
                        graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                        for (int i = 1;i <= 7;i++)
                        {
                            cha.turnEast();
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            if (key.showing)
                            {
                                graphics.renderTexture(keyimg, key.x, key.y);

                            }
                            cha.move();
                            graphics.render(cha.x, cha.y, moveright);
                            if (slime1.isalive)
                            {
                                if (slime1.isfacingright)
                                {
                                    slime1.turnEast();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                    if (i <= 6)slimeright1.tick();
                                }
                                else
                                {
                                    slime1.turnWest();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                    if (i <= 6) slimeleft1.tick();
                                }
                            }
                            if (slime2.isalive)
                            {
                                if (slime2.isfacingright)
                                {
                                    slime2.turnEast();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                    if (i <= 6) slimeright2.tick();
                                }
                                else
                                {
                                    slime2.turnWest();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                    if (i <= 6) { slimeleft2.tick(); }
                                }
                            }
                            if (slime3.isalive)
                            {
                                if (slime3.isfacingright)
                                {
                                    slime3.turnEast();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                    if (i <= 6)slimeright3.tick();
                                }
                                else
                                {
                                    slime3.turnWest();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                    if (i <= 6) slimeleft3.tick();
                                }
                                if (i == 4 && !collided && (cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive))
                                {
                                    collided = true;
                                    cha.heartamount--;
                                }

                            }

                          
                            moveright.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);


                        }

                   
                        slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                        if (slime1.limitcount == 1)
                        {
                            slime1.isfacingright = !slime1.isfacingright;
                        }
                        slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                        if (slime2.limitcount == 1)
                        {
                            slime2.isfacingright = !slime2.isfacingright;
                        }
                        slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                        if (slime3.limitcount == 1)
                        {
                            slime3.isfacingright =! slime3.isfacingright;
                        }
                        if (!collided && (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive))
                        {
                            cha.heartamount--;
                        }
                        idling = true;
                        isfacingright = true;
                        isfacingup = false;
                        isfacingdown = false;
                        isfacingleft = false;
                    }
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    if (cha.notborder('d'))
                    {
                        idling = 0;

                        for (int i = 1;i <= 7;i++)
                        {
                            cha.turnSouth();
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            if (key.showing)
                            {
                                graphics.renderTexture(keyimg, key.x, key.y);

                            }
                            cha.move();
                            graphics.render(cha.x, cha.y, movedown);
                            if (slime1.isalive)
                            {
                                if (slime1.isfacingright)
                                {
                                    slime1.turnEast();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeright1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeright1); }
                                    if (i <= 6)slimeright1.tick();
                                }
                                else
                                {
                                    slime1.turnWest();
                                    slime1.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime1.x, slime1.y - i * 5, slimeleft1);
                                    }
                                    else { graphics.render(slime1.x, slime1.y, slimeleft1); }
                                    if (i <= 6) slimeleft1.tick();
                                }
                            }
                            if (slime2.isalive)
                            {
                                if (slime2.isfacingright)
                                {
                                    slime2.turnEast();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeright2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeright2); }
                                    if (i <= 6) slimeright2.tick();
                                }
                                else
                                {
                                    slime2.turnWest();
                                    slime2.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime2.x, slime2.y - i * 5, slimeleft2);
                                    }
                                    else { graphics.render(slime2.x, slime2.y, slimeleft2); }
                                    if (i <= 6) { slimeleft2.tick(); }
                                }
                            }
                            if (slime3.isalive)
                            {
                                if (slime3.isfacingright)
                                {
                                    slime3.turnEast();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeright3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeright3); }
                                    if (i <= 6)slimeright3.tick();
                                }
                                else
                                {
                                    slime3.turnWest();
                                    slime3.move();
                                    if (i >= 2 && i <= 4)
                                    {
                                        graphics.render(slime3.x, slime3.y - i * 5, slimeleft3);
                                    }
                                    else { graphics.render(slime3.x, slime3.y, slimeleft3); }
                                    if (i <= 6) slimeleft3.tick();
                                }
                            }
                            movedown.tick();

                            graphics.presentScene();
                            
                            SDL_Delay(50);


                        }
                      
                        slime1.limitcount = (slime1.limitcount) % slime1.movelimit + 1;
                        if (slime1.limitcount == 1)
                        {
                            slime1.isfacingright = !slime1.isfacingright;
                        }
                        slime2.limitcount = (slime2.limitcount) % slime2.movelimit + 1;
                        if (slime2.limitcount == 1)
                        {
                            slime2.isfacingright = !slime2.isfacingright;
                        }
                        slime3.limitcount = (slime3.limitcount) % slime3.movelimit + 1;
                        if (slime3.limitcount == 1)
                        {
                            slime3.isfacingright =! slime3.isfacingright;
                        }
                        if (cha.touchacid() || cha.ishurt(slime1.rect) && slime1.isalive || cha.ishurt(slime2.rect) && slime2.isalive || cha.ishurt(slime3.rect) && slime3.isalive)
                        {
                            cha.heartamount--;
                        }
                        idling = true;
                        isfacingdown = true;
                        isfacingleft = false;
                        isfacingright = false;
                        isfacingup = false;
                    }
                }

                break;
            case SDL_KEYUP:

                break;

            }
            if (slime1.heartamount <= 0&&slime1.isalive)
            {
                slime1.died = 1;
            }
            if (slime2.heartamount <= 0&&slime2.isalive)
            {
                slime2.died = 1;
            }
            if (slime3.heartamount <= 0&&slime3.isalive)
            {
                slime3.died = 1;
            }
  
            if (slime1.died||slime2.died||slime3.died)
            {
                if (slime1.died)
                {
                    slime1.isalive = 0;
                }
                 if (slime2.died)
                {
                    slime2.isalive = 0;
                }
                 if (slime3.died)
                {
                    slime3.isalive = 0;
                }
                for (int i = 0;i < 5;i++)
                {
                        graphics.prepareScene(background);
                        if (isfacingup)
                        {

                            graphics.render(cha.x, cha.y, moveup);
                            moveup.tick();

                        }
                        else if (isfacingleft)
                        {
                            moveleft.tick();
                            graphics.render(cha.x, cha.y, moveleft);
                        }
                        else if (isfacingright)
                        {
                            graphics.render(cha.x, cha.y, moveright);
                            moveright.tick();

                        }
                        else
                        {
                            graphics.render(cha.x, cha.y, idle);
                            idle.tick();

                        }
                        if (slime1.isalive)
                        {
                            if (slime1.isfacingright)
                            {
                                graphics.render(slime1.x, slime1.y, slimeidle1);
                            }
                            else
                            {
                                graphics.render(slime1.x, slime1.y, slimeidle2);
                            }
                        }
                        else if(!slime1.isalive&&slime1.died)
                        {
                            graphics.render(slime1.x, slime1.y, slimedeath1);
                            slimedeath1.tick();
                        }
                        if (slime2.isalive)
                        {
                            if (slime2.isfacingright)
                            {
                                graphics.render(slime2.x, slime2.y, slimeidle1);
                            }
                            else
                            {
                                graphics.render(slime2.x, slime2.y, slimeidle2);
                            }
                        }
                        else if(!slime2.isalive &&slime2.died)
                        {
                            graphics.render(slime2.x, slime2.y, slimedeath2);
                            slimedeath2.tick();
                        }
                        if (slime3.isalive)
                        {
                            if (slime3.isfacingright)
                            {
                                graphics.render(slime3.x, slime3.y, slimeidle1);
                            }
                            else
                            {
                                graphics.render(slime3.x, slime3.y, slimeidle2);
                            }
                        }
                        else if(!slime3.isalive && slime3.died)
                        {
                            graphics.render(slime3.x, slime3.y, slimedeath3);
                            slimedeath3.tick();
                        }
                        slimeidle1.tick();
                        slimeidle2.tick();
                        graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                        if (key.showing)
                        {
                            graphics.renderTexture(keyimg, key.x, key.y);

                        }
                        graphics.presentScene();
                        
                        SDL_Delay(50);
                }
                if (slime1.died)
                {
                    slime1.died = 0;
                }
                 if (slime2.died)
                {
                    slime2.died = 0;
                }
                 if (slime3.died)
                {
                    slime3.died = 0;
                }
            }
            if (idling)
            {
                
                graphics.prepareScene(background);
                if (key.showing)
                {
                    graphics.renderTexture(keyimg, key.x, key.y);

                }
                if (isfacingup)
                {

                    graphics.render(cha.x, cha.y, moveup);
                    moveup.tick();

                }
                else if (isfacingleft)
                {
                    moveleft.tick();
                    graphics.render(cha.x, cha.y, moveleft);
                }
                else if (isfacingright)
                {
                    graphics.render(cha.x, cha.y, moveright);
                    moveright.tick();

                }
                else
                {
                    graphics.render(cha.x, cha.y, idle);
                    idle.tick();
            
                }
                if (slime1.isalive)
                {
                    if (slime1.isfacingright)
                    {
                        graphics.render(slime1.x, slime1.y, slimeidle1);
                    }
                    else
                    {
                        graphics.render(slime1.x, slime1.y, slimeidle2);
                    }
                }
                if (slime2.isalive)
                {
                    if (slime2.isfacingright)
                    {
                        graphics.render(slime2.x, slime2.y, slimeidle1);
                    }
                    else
                    {
                        graphics.render(slime2.x, slime2.y, slimeidle2);
                    }
                }
                if (slime3.isalive)
                {
                    if (slime3.isfacingright)
                    {
                        graphics.render(slime3.x, slime3.y, slimeidle1);
                    }
                    else
                    {
                        graphics.render(slime3.x, slime3.y, slimeidle2);
                    }
                }
                slimeidle1.tick();
                slimeidle2.tick();
                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
         
                graphics.presentScene();
                
            }  
        if (!slime1.isalive && !slime2.isalive && !slime3.isalive && !key.obtained&&currentmap==0)
        {
            key.showing = true;
        }
        if (inside(key.locx, key.locy, cha.rect)&&!key.obtained&&key.showing)
        {
            key.obtained = true;
            key.showing = false;
            currentmap = 1;
            changedmap = 1;
           // Mix_HaltMusic();
           // graphics.play(nc);
        }
        if (inside(405,510, cha.rect)&&key.obtained)
        {
            changedmap = 1;
            currentmap = 2;
            //cha.x = ;
               // cha.y = ;
                isfacingdown = true;
                isfacingleft = false;
                isfacingright = false;
                isfacingup = false;
                cha.x = 380;
                cha.y = 50;
        }
        if (cha.heartamount <= 0)
        {
            for (int i = 1;i <= 3;i++)
            {
                graphics.prepareScene(background);
                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                graphics.render(cha.x, cha.y + 10, chadeath);
                chadeath.tick();
                graphics.presentScene();
                
                SDL_Delay(500);
              
            }
            quit = true;
        }     
        break;
        }
        //start of map2
        case 2:
        {
            switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
        
                    for (int i = 1;i <= 7;i++)
                    {

                        graphics.prepareScene(background);
                        graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                        if (isfacingup)
                        {
                            graphics.render(cha.x, cha.y, moveup);
                        }
                        else if (isfacingleft)
                        {
                            graphics.render(cha.x, cha.y, moveleft);

                        }
                        else if (isfacingdown)
                        {
                            graphics.render(cha.x, cha.y, movedown);
                        }
                        else
                        {
                            graphics.render(cha.x, cha.y, moveright);
                        }
                          graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                        graphics.presentScene();
                        
                        SDL_Delay(50);
                    }
                    if (cha.isburnt(vecfire, firestate))
                    {
                        cha.heartamount--;
                    }
                    firestate = (firestate + 1) % 3;
               
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_Z)
                {
                   
                    if (isfacingright)
                    {
                        for (int i = 1;i <= 7;i++)
                        {
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            graphics.render(cha.x, cha.y, slashright);
                            if (i <= 4)
                            {
                                slashright.tick();
                            }
                            graphics.presentScene();
                            
                            SDL_Delay(50);

                        }
                    }
                    else if (isfacingleft)
                    {
                        for (int i = 1;i <= 7;i++)
                        {
                            if (slime1.heartamount <= 0 && slime1.isalive)
                            {
                                slime1.canhurt = 0;
                            }
                            if (slime2.heartamount <= 0 && slime2.isalive)
                            {
                                slime2.canhurt = 0;
                            }
                            if (slime3.heartamount <= 0 && slime3.isalive)
                            {
                                slime3.canhurt = 0;
                            }
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                    if (slashleft.currentFrame != 1)
                            {
                                graphics.render(cha.x, cha.y, slashleft);
                            }
                            else
                            {
                                graphics.render(cha.x - 30, cha.y, slashleft);
                            }
                            if (i <= 4) slashleft.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);
                        }
                     
                    }
                    else if (isfacingdown)
                    {
                      
                        for (int i = 1;i <= 7;i++)
                        {
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            graphics.render(cha.x, cha.y, slashdown);
                            if (i <= 4) slashdown.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);
                        }
                    }
                    else if (isfacingup)
                    {
                        for (int i = 1;i <= 7;i++)
                        {
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            graphics.render(cha.x, cha.y, slashup);

                            if (i <= 4)  slashup.tick();

                            graphics.presentScene();
                            
                            SDL_Delay(50);
                        }
                    
                    }
                    if (cha.isburnt(vecfire, firestate))
                    {
                        cha.heartamount--;
                    }
                    firestate = (firestate + 1) % 3;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_X)
                {
                    if (isfacingright)
                    {
                        if (cha.notborderjump('r'))
                        {
                            idling = 0;

                            for (int i = 1;i <= 14;i++)
                            {
                                cha.turnEast();
                                graphics.prepareScene(background);
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                cha.move();
                                if (i <= 7)
                                {
                                    graphics.render(cha.x, cha.y - 4 * i, moveright);
                                }
                                else
                                {
                                    graphics.render(cha.x, cha.y - (14 - i) * 4, moveright);
                                }
                                graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                moveright.tick();
                                graphics.presentScene();
                                
                                SDL_Delay(35);


                            }
                            idling = true;
                            isfacingright = true;
                            isfacingup = false;
                            isfacingdown = false;
                            isfacingleft = false;
                        }
                    }
                    else if (isfacingup)
                    {
                        if (cha.notborderjump('u'))
                        {
                            idling = 0;
                            for (int i = 1;i <= 14;i++)
                            {

                                cha.turnNorth();
                                graphics.prepareScene(background);
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                cha.move();
                                if (i <= 7)
                                {
                                    graphics.render(cha.x, cha.y - 4 * i, moveup);
                                }
                                else
                                {
                                    graphics.render(cha.x, cha.y - (14 - i) * 4, moveup);
                                }
                                graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                moveup.tick();
                                graphics.presentScene();
                                
                                SDL_Delay(35);
                            }
                            idling = true;
                            isfacingup = true;
                            isfacingright = false;
                            isfacingleft = false;
                            isfacingdown = false;
                        }
                    }
                    else if (isfacingdown)
                    {
                        if (cha.notborderjump('d'))
                        {
                            idling = 0;

                            for (int i = 1;i <= 14;i++)
                            {
                                cha.turnSouth();
                                graphics.prepareScene(background);
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                      
                                cha.move();
                                if (i <= 7)
                                {
                                    graphics.render(cha.x, cha.y - 4 * i, movedown);
                                }
                                else
                                {
                                    graphics.render(cha.x, cha.y - (14 - i) * 4, movedown);
                                }
                                graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                movedown.tick();
                                graphics.presentScene();
                                
                                SDL_Delay(35);


                            }
                            idling = true;
                            isfacingdown = true;
                            isfacingleft = false;
                            isfacingright = false;
                            isfacingup = false;
                        }
                    }
                    else if (isfacingleft)
                    {
                        if (cha.notborderjump('l'))
                        {
                            idling = 0;

                            for (int i = 1;i <= 14;i++)
                            {
                                graphics.prepareScene(background);
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                cha.turnWest();
                                cha.move();
                                if (i <= 7)
                                {
                                    graphics.render(cha.x, cha.y - 4 * i, moveleft);
                                }
                                else
                                {
                                    graphics.render(cha.x, cha.y - (14 - i) * 4, moveleft);
                                }
                                graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                moveleft.tick();
                                graphics.presentScene();
                                
                                SDL_Delay(35);
                            }
                            idling = true;
                            isfacingleft = true;
                            isfacingdown = false;
                            isfacingup = false;
                            isfacingright = false;
                        }
                    }
                    if (cha.isburnt(vecfire, firestate))
                    {
                        cha.heartamount--;
                    }
                    firestate = (firestate + 1) % 3;

                }
                if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    if (cha.notborder('u'))
                    {
                        idling = 0;
                        for (int i = 1;i <= 7;i++)
                        {

                            cha.turnNorth();
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            
                            cha.move();
                            graphics.render(cha.x, cha.y, moveup);
                            graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                            moveup.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);
                        }
                        idling = true;
                        isfacingup = true;
                        isfacingright = false;
                        isfacingleft = false;
                        isfacingdown = false;
                    
                    }
                    if (cha.isburnt(vecfire, firestate))
                    {
                        cha.heartamount--;
                    }
                    firestate = (firestate + 1) % 3;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)

                {
                    if (cha.notborder('l'))
                    {
                        idling = 0;
                        bool collided = false;
                        for (int i = 1;i <= 7;i++)
                        {
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            cha.turnWest();
                         
                            cha.move();
                            graphics.render(cha.x, cha.y, moveleft);
                            graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                            moveleft.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);


                        }
                        idling = true;
                        isfacingleft = true;
                        isfacingdown = false;
                        isfacingup = false;
                        isfacingright = false;
                    }
                    if (cha.isburnt(vecfire, firestate))
                    {
                        cha.heartamount--;
                    }
                    firestate = (firestate + 1) % 3;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    if (cha.notborder('r'))
                    {
                        bool collided = false;
                        idling = 0;
                        graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                        for (int i = 1;i <= 7;i++)
                        {
                            cha.turnEast();
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            cha.move();
                            graphics.render(cha.x, cha.y, moveright);
                            graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                            moveright.tick();
                            graphics.presentScene();
                            
                            SDL_Delay(50);
                        }
                        idling = true;
                        isfacingright = true;
                        isfacingup = false;
                        isfacingdown = false;
                        isfacingleft = false;
                    }
                    if (cha.isburnt(vecfire, firestate))
                    {
                        cha.heartamount--;
                    }
                    firestate = (firestate + 1) % 3;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    if (cha.notborder('d'))
                    {
                        idling = 0;

                        for (int i = 1;i <= 7;i++)
                        {
                            cha.turnSouth();
                            graphics.prepareScene(background);
                            graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                            cha.move();
                            graphics.render(cha.x, cha.y, movedown);
                            graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                            movedown.tick();

                            graphics.presentScene();
                            
                            SDL_Delay(50);


                        }
                        idling = true;
                        isfacingdown = true;
                        isfacingleft = false;
                        isfacingright = false;
                        isfacingup = false;
                    }
                    if (cha.isburnt(vecfire, firestate))
                    {
                        cha.heartamount--;
                    }
                    firestate = (firestate + 1) % 3;
                }

                break;
            case SDL_KEYUP:

                break;

            }
            if (idling)
            {

                graphics.prepareScene(background);
               
                if (isfacingup)
                {

                    graphics.render(cha.x, cha.y, moveup);
                    moveup.tick();

                }
                else if (isfacingleft)
                {
                    moveleft.tick();
                    graphics.render(cha.x, cha.y, moveleft);
                }
                else if (isfacingright)
                {
                    graphics.render(cha.x, cha.y, moveright);
                    moveright.tick();

                }
                else
                {
                    graphics.render(cha.x, cha.y, idle);
                    idle.tick();

                }
                graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                graphics.presentScene();
               
            }
            break;
         
        }

        
        SDL_Delay(100);


 
       }
    }
}
   
    graphics.quit();
    return 0;
}
