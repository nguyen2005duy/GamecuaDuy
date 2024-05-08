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
#include <SDL_ttf.h>
#define FIRELIMIT 30
#define FIREBALLLIMIT 4
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
    SDL_Texture* warninglane = loadTexture("img\\warninglane.png", graphics.renderer);
    SDL_Texture* chadial = loadTexture("img\\characterdialogue.png", graphics.renderer);
    dialogue dial;
    Key key;
    int fireballoc = 0;
    key.x = 370;
    key.y = 280;
    key.locx= 408;
    key.locy = 308;
    int firestate = 0;
    int fireballloc = 0;
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
    Sprite bossidle;
    SDL_Texture* bossi = graphics.loadTexture("img\\boss.png");
    bossidle.init(bossi, BOSS_IDLE_FRAME, BOSS_IDLE_CLIPS);
    Sprite bossidlel;
    Sprite bossdeath;
    SDL_Texture*bossdeathi = graphics.loadTexture("img\\bossdeath.png");
    bossdeath.init(bossdeathi, BOSS_DEATH_FRAME, BOSS_DEATH_CLIPS);
    SDL_Texture* bossil = graphics.loadTexture("img\\bossl.png");
    bossidlel.init(bossil, BOSS_IDLEL_FRAME, BOSS_IDLEL_CLIPS);
    SDL_Texture* fireballL = graphics.loadTexture("img\\fireballleft.png");
    SDL_Texture* fireballR = graphics.loadTexture("img\\fireballright.png");
    Sprite fireballleft;
    Sprite fireballright;
    fireballleft.init(fireballL, FIREBALL_LEFT_FRAME, FIREBALL_LEFT_CLIPS);
    fireballright.init(fireballR, FIREBALL_RIGHT_FRAME, FIREBALL_RIGHT_CLIPS);
    bool isfacingup = 0;
    bool isfacingdown = 1;
    bool isfacingright = 0;
    bool isfacingleft = 0;
    int fireballstate = 0;
    SDL_Texture * pauseui = loadTexture("img\\pauseui.png", graphics.renderer);
    Cha cha(65, 50);
    vector<fire> vecfire(30);
    vector<fireball> vecfireball(4);
    bool changedmap = 1;
    En slime1(65+ 2*INITIAL_SPEEDX * 7,  70 + 2*INITIAL_SPEEDY * 7,6,210,238);
    slime1.isfacingright = 1;
    En slime2(65 + 10 * INITIAL_SPEEDX * 7, 70 + 4 * INITIAL_SPEEDY * 7,10,718,373);
    slime2.isfacingright = 0;
    En slime3(65 + 0* INITIAL_SPEEDX * 7, 70 + 6 * INITIAL_SPEEDY * 7, 10,85,510);
    slime3.isfacingright = 1;
    En boss(375, 230, 1, 400, 270);
    int x;
    int y;
    bool GAMEOVER = 0;
    bool pause = false;
    bool quit = false;
    bool idling = 1;
    cha.heartamount = 3;
    int m = 1;
    int currentmap = -3;
    SDL_Event event;
    int flimit = rand() % FIRELIMIT + 1;
    Mix_Music* nc = graphics.loadMusic("audio\\nightdancer.mp3");
    Mix_Music* rn = graphics.loadMusic("audio\\renai.mp3");
    graphics.play(nc);
    TTF_Font* menufont = graphics.loadFont("font\\menu.ttf", 30);
    TTF_Font* uifont = graphics.loadFont("font\\ui.ttf", 20);
    TTF_Font* pausefont = graphics.loadFont("font\\ui.ttf", 25);
    TTF_Font* gameoverfont = graphics.loadFont("font\\gameover.otf", 90);
    TTF_Font* replayfont = graphics.loadFont("font\\gameover.otf", 60);
    TTF_Font* dialfont = graphics.loadFont("font\\charactersdialouge.otf", 20);
    SDL_Color color = { 21, 21, 21, 225 };
    SDL_Color colorhover = { 226,226,226,226 };
    SDL_Color colorhovere = {255,255,255,255};
    SDL_Color chadialcolour = { 32,201,207,255 };
    SDL_Color bossdialcolour = { 218,50,4,255 };

    SDL_Texture* gameover = graphics.renderText("Game Over", gameoverfont, colorhover);
    SDL_Texture* gamename = graphics.renderText("Mystic Dungeon", gameoverfont, colorhover);
    SDL_Texture* menu = graphics.renderText("Menu", menufont, color);
    SDL_Texture* PAUSE = graphics.renderText("Pause", menufont, color);
    SDL_Texture* start = graphics.renderText("Start", uifont, color);
    SDL_Texture* starthovered = graphics.renderText("Start", uifont, colorhover);
    ui startui;
    SDL_Texture* controlshovered = graphics.renderText("Controls", uifont, colorhover);
    ui continuesui;
    SDL_Texture* continues = graphics.renderText("Continue", pausefont, color);
    SDL_Texture* continueshovered = graphics.renderText("Continue", pausefont, colorhover);
    ui controlsui;
    SDL_Texture* quitfonthovered = graphics.renderText("Quits", uifont, colorhover);
    SDL_Texture* menupause = graphics.renderText("Menu", pausefont, color);
    SDL_Texture* menupausehovered = graphics.renderText("Menu", pausefont, colorhover);

    ui menupauseui;
    ui quitui;
    bool muted = 0;
    SDL_Texture* controls = graphics.renderText("Controls", uifont, color);
    SDL_Texture* quitfont = graphics.renderText("Quits", uifont, color);
    SDL_Texture* volume = loadTexture("img\\volume.png", graphics.renderer);
    SDL_Texture* volumemuted = loadTexture("img\\volumemuted.png", graphics.renderer);
    ui replay;
    SDL_Texture* playagain = graphics.renderText("Replay?", replayfont, colorhover);
    SDL_Texture* playagainhover = graphics.renderText("Replay?", replayfont, colorhovere);
    SDL_Texture* ztoslash = graphics.renderText("Z to slash", uifont, colorhovere);
    SDL_Texture* xtojump = graphics.renderText("X to jump", uifont, colorhovere);
    SDL_Texture* upto = graphics.renderText("UP to go up", uifont, colorhovere);
    SDL_Texture* downto = graphics.renderText("DOWN to go down", uifont, colorhovere);
    SDL_Texture* rightto = graphics.renderText("RIGHT to go right", uifont, colorhovere);
    SDL_Texture* leftto = graphics.renderText("LEFT to go left", uifont, colorhovere);
    SDL_Texture* pto = graphics.renderText("P to pause",uifont, colorhovere);
    SDL_Texture* xto = graphics.renderText("Space to wait a turn",uifont, colorhovere);
    SDL_Texture* back = graphics.renderText("Back", uifont, colorhover);
    SDL_Texture* backhovered = graphics.renderText("Back", uifont, colorhovere);
    SDL_Texture* next = graphics.renderText("Next", uifont, colorhover);
    SDL_Texture* nexthover = graphics.renderText("Next", uifont, colorhovere);

    SDL_Texture* madeby = graphics.renderText("This game was made and owned by Nguyen Duc Duy", uifont, colorhovere);
    SDL_Texture* goal = graphics.renderText("Your goal is to reach to the end of the dungeon ", uifont, colorhovere);
    SDL_Texture* goalnext = graphics.renderText("and obtain the master sword", uifont, colorhovere);
    SDL_Texture* nextto = graphics.renderText("Please refer to the next page for the controls of the game", uifont, colorhovere);
    SDL_Texture* purpose = graphics.renderText("This was made as a school project", uifont, colorhovere);
    SDL_Texture* looklike = graphics.renderText("Looks like this dungeon is getting slimy right from the start.", dialfont, chadialcolour);
    SDL_Texture* pressany = graphics.renderText("Press any key to continue.........", dialfont, colorhover);
    SDL_Texture* tips = graphics.renderText("Tips: Hit the boss's heart!!!!", dialfont, bossdialcolour);
    SDL_Texture* keyshow = graphics.renderText("Victory shines as the key appears, unlocking the path", dialfont, colorhover);
    SDL_Texture* keyshow2 = graphics.renderText("to greater challenges ahead.", dialfont, colorhover);
    SDL_Texture* enter1 = graphics.renderText("So, you're the one who's been causing all this chaos.", dialfont, chadialcolour);
    SDL_Texture* boss1 = graphics.renderText("Ah, the brave hero has finally arrived.", dialfont, bossdialcolour);
    SDL_Texture* boss2 = graphics.renderText("I must say, I expected more from you.", dialfont, bossdialcolour);
    SDL_Texture* cha1 = graphics.renderText("You won't get away with this. Your reign of terror ends here.", dialfont, chadialcolour);
    SDL_Texture* bossdial = graphics.loadTexture("img\\bossdialogue.png");
    ui nextui;
    ui backui;
    bossheart bossh;
    SDL_Texture *msword = graphics.loadTexture("img\\mastersword.png");
    Key ms;
    ms.x = 378;
    ms.y = 258;
    ms.locx = 400;
    ms.locy = 300;
    bossh.generatebossheartlocation();
    SDL_Texture* bossheartt = loadTexture("img\\bossheart.png",graphics.renderer);

    while (!quit && !gameOver(cha)) {
        SDL_GetMouseState(&x, &y);
        //  cerr << x << "," << y;
      //  graphics.prepareScene(background);

        while (SDL_PollEvent(&event)) {
            if (changedmap)
            {
                switch (currentmap)
                {
                case -3:

                    changedmap = 0;
                    SDL_DestroyTexture(background);
                    background = loadTexture("img//menu.png", graphics.renderer);
                    break;
                case -2:

                    changedmap = 0;
                    SDL_DestroyTexture(background);
                    background = loadTexture("img//aboutthegame.png", graphics.renderer);
                    break;
                case -1:
                    changedmap = 0;
                    SDL_DestroyTexture(background);
                    background = loadTexture("img//controls.png", graphics.renderer);
                    break;
                case 0:
                   
                    changedmap = 0;
                    cha.reset();
                    slime1.reset();
                    slime1.isfacingright = 1;
                    slime2.isfacingright = 0;
                    slime3.isfacingright = 1;
                    slime2.reset();
                    slime3.reset();
                    isfacingup = 0;
                    isfacingdown = 1;
                    isfacingright = 0;
                    isfacingleft = 0;
                    fireballstate = 0;
                    firestate = 0;
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
                    background = loadTexture("img//map2butbetter.png", graphics.renderer);
                    break;
                case 3:
                    changedmap = 0;
                    cha.reset();
                    slime1.reset();
                    slime1.isfacingright = 1;
                    slime2.isfacingright = 0;
                    slime3.isfacingright = 1;
                    slime2.reset();
                    slime3.reset();
                    isfacingup = 0;
                    isfacingdown = 1;
                    isfacingright = 0;
                    isfacingleft = 0;
                    fireballstate = 0;
                    firestate = 0;
                    SDL_DestroyTexture(background);
                    background = loadTexture("img//gameover.png", graphics.renderer);
                    break;
                }
            }
            switch(currentmap)
            {
            case -3:
                SDL_GetMouseState(&x, &y);
                if ((x >= 155 && x <= 269) && (y >= 289 && y <= 334))
                {
                    startui.hovered = true;
                }
                else
                {
                    startui.hovered = false;
                }
                if ((153 <= x && x <= 153 + 116) && (y >= 343 && y <= 343 + 49))
                {
                    controlsui.hovered = true;
                }
                else
                {
                    controlsui.hovered = false;
                }
                if ((151 <= x && x <= 151 + 117) && (y >= 400 && y <= 400 + 44))
                {
                    quitui.hovered = true;
                }
                else
                {
                    quitui.hovered = false;
                }
                graphics.prepareScene(background);
                graphics.renderTexture(gamename,100,50);
                graphics.renderTexture(menu, 158, 216);
                if (!muted)
                {
                    graphics.renderTexture(volume, 193, 255);
                    Mix_ResumeMusic();
                }
                else
                {
                    graphics.renderTexture(volumemuted, 193, 255);
                    Mix_PauseMusic();
                }
                if (!startui.hovered)
                {
                    graphics.renderTexture(start, 179, 296);
                }
                else
                {
                    graphics.renderTexture(starthovered, 179, 299);
                }
                if (!controlsui.hovered)
                {
                    graphics.renderTexture(controls, 160, 351);
                }
                else
                {
                    graphics.renderTexture(controlshovered, 160, 354);
                }
                if (!quitui.hovered)
                {
                    graphics.renderTexture(quitfont, 177, 405);
                }
                else
                {
                    graphics.renderTexture(quitfonthovered, 177, 408);
                }
                switch (event.type) {
                case SDL_QUIT:
                    exit(0);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if ((x >= 193 && x <= 193 + 42) && (y >= 255 && y <= 255 + 33))
                    {
                        muted = !muted;
                    }
                    if ((x >= 155 && x <= 269) && (y >= 289 && y <= 334))
                    {
                        currentmap = 0;
                        changedmap = 1;
                    }
                    else if ((151 <= x && x <= 151 + 117) && (y >= 400 && y <= 400 + 44))
                    {
                        quit = true;
                    }
                    else  if ((153 <= x && x <= 153 + 116) && (y >= 343 && y <= 343 + 49))
                    {
                        currentmap = -2;
                        changedmap = 1;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    cerr << "Up at (" << x << ", " << y << ")\n";
                    break;
                }
                graphics.presentScene();
                break;
            case -2:
                graphics.prepareScene(background);
                graphics.renderTexture(gamename, 100, 50);
                SDL_GetMouseState(&x, &y);
                if ((x <= 749 && x >= 649) && (y <= 567 && y >= 529))
                {
                    nextui.hovered = true;
                }
                else
                {
                    nextui.hovered = false;
                }
           
                switch (event.type)
                {
                case SDL_MOUSEBUTTONDOWN:
                    if ((x <= 749 && x >= 649) && (y <= 567 && y >= 529))
                    {
                        changedmap = 1;
                        currentmap = -1;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    cerr << x << " " << y << endl;
                    break;
                }
                graphics.renderTexture(madeby, 95, 200);
                graphics.renderTexture(goal, 95, 250);
                graphics.renderTexture(goalnext, 220, 300);
                graphics.renderTexture(nextto, 15, 350);
                graphics.renderTexture(purpose, 180, 400);
                if (!nextui.hovered)
                {
                    graphics.renderTexture(next, 671, 537);
                }
                else { graphics.renderTexture(nexthover, 671, 537); }
                graphics.presentScene();
                break;
            case -1:
                graphics.prepareScene(background);
                graphics.renderTexture(ztoslash, 240, 86);
                graphics.renderTexture(rightto,525,95);
                graphics.renderTexture(upto, 200,280);
                graphics.renderTexture(downto,515,282);
                graphics.renderTexture(leftto,185,445);
                graphics.renderTexture(pto, 520, 450);
                graphics.renderTexture(xto,270,530 );
                SDL_GetMouseState(&x, &y);
                if ((x <= 749 && x >= 649) && (y <= 567 && y >= 529))
                {
                    backui.hovered = true;
                }
                else
                {
                    backui.hovered = false;
                }
                switch (event.type)
                {
                case SDL_MOUSEBUTTONDOWN:
                    if ((x <= 749 && x >= 649) && (y <= 567 && y >= 529))
                    {
                        changedmap = 1;
                        currentmap = -3;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
  
                    break;
                }
                if (!backui.hovered)
                {
                    graphics.renderTexture(back, 671, 537);
                }
                else { graphics.renderTexture(backhovered, 671, 537); }
                graphics.presentScene();
                break;
            case 0:
            case 1:
        {
        //start of map1 and zero
                while (dial.first)
                {
                    graphics.prepareScene(background);
                    graphics.render(slime1.x, slime1.y, slimeright1);
                    graphics.render(slime2.x, slime2.y, slimeleft2);
                    graphics.render(cha.x, cha.y, movedown);
                    graphics.renderTexture(chadial,36, 384);
                    graphics.renderTexture(looklike, 205, 446);
                    graphics.renderTexture(pressany, 210, 486);
                    graphics.presentScene();
                    waitUntilKeyPressed();
                    dial.first = 0;
            }
            switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_P)
                {
                    pause =! pause;
                }
                if(!pause)
                {
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
                    if (!pause)
                    {
                        moveup.tick();
                    }
                }
                else if (isfacingleft)
                {
                    if (!pause)
                    {
                        moveleft.tick();
                    }
                    graphics.render(cha.x, cha.y, moveleft);
                }
                else if (isfacingright)
                {
                    

                    graphics.render(cha.x, cha.y, moveright);
                    if (!pause)
                    {
                        moveright.tick();
                    }
                }
                else
                {
                    graphics.render(cha.x, cha.y, idle);
                    if (!pause)
                    {
                        idle.tick();
                    }
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
                if (!pause)
                {
                    slimeidle1.tick();
                    slimeidle2.tick();
                }

                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                if (pause)
                {
                    graphics.renderTexture(pauseui, 215, 110);
                    graphics.renderTexture(PAUSE, 320, 134);
                    SDL_GetMouseState(&x, &y);
                    if ((315 <= x && x <= 480) && (y <= 295 && y >= 240))
                    {
                        continuesui.hovered = true;
                    }
                    else { continuesui.hovered = false; }
                    if ((x >= 314 && x <= 480) && (y >= 327 && y <= 384))
                    {
                        menupauseui.hovered = true;
                    }
                    else
                    {
                        menupauseui.hovered = false;
                    }
                   
                    if (!continuesui.hovered)
                    {
                        graphics.renderTexture(continues, 333, 252);
                    }
                    else
                    {
                        graphics.renderTexture(continueshovered, 333, 254);
                    }
                    if (!menupauseui.hovered)
                    {
                        graphics.renderTexture(menupause, 360, 336);
                    }
                    else
                    {
                        graphics.renderTexture(menupausehovered, 360, 338);
                    }
                    if (!muted)
                    {
                        graphics.renderTexture(volume, 373, 427);
                        Mix_ResumeMusic();
                    }
                    else
                    {
                        graphics.renderTexture(volumemuted, 373, 427);
                        Mix_PauseMusic();
                    }
                    switch (event.type)
                    {
                    case SDL_MOUSEBUTTONDOWN:
                        cerr << x << " " << y << endl;
                        if ((315 <= x && x <= 480) && (y <= 295 && y >= 240))
                        {
                            pause = false;
                        }
                        if ((x >= 376 && x <= 429) && (y >= 428 && y <= 453))
                        {
                            muted = !muted;
                        }                       
                        if ((x >= 314 && x <= 480) && (y >= 327 && y <= 384))
                        {
                            changedmap = 1;
                            currentmap = -3;
                            pause = false;
                        }
                            break;
                    case SDL_MOUSEBUTTONUP:
                        break;
                   }
                }
               // graphics.renderTexture(helloText, 200, 200);
                graphics.presentScene();
                
            }  
        if (!slime1.isalive && !slime2.isalive && !slime3.isalive && !key.obtained&&currentmap==0)
        {
            key.showing = true;
            while (dial.second)
            {
                graphics.prepareScene(background);
                if (isfacingdown)
                {
                    graphics.render(cha.x, cha.y, movedown);
                }
                else if (isfacingup)
                {
                    graphics.render(cha.x, cha.y, moveup);

                }
                else if (isfacingright)
                {
                    graphics.render(cha.x, cha.y, moveright);

                }
                else
                {
                    graphics.render(cha.x, cha.y, moveleft);
                }
                graphics.renderTexture(chadial, 36, 384);
                graphics.renderTexture(keyshow, 205, 446);
                graphics.renderTexture(keyshow2, 205, 466);
                graphics.renderTexture(pressany, 205, 506);
                graphics.presentScene();
                waitUntilKeyPressed();
                dial.second = 0;
            }
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
            changedmap = true;
            currentmap = 3;
        }     
        break;
        }
        //start of map2
        case 2:
            while (dial.third)
            {
                graphics.prepareScene(background);
                if (isfacingdown)
                {
                    graphics.render(cha.x, cha.y, movedown);
                }
                else if (isfacingup)
                {
                    graphics.render(cha.x, cha.y, moveup);

                }
                else if (isfacingright)
                {
                    graphics.render(cha.x, cha.y, moveright);

                }
                else
                {
                    graphics.render(cha.x, cha.y, moveleft);
                }
                graphics.renderTexture(chadial, 36, 384);
                graphics.render(boss.x, boss.y, bossidle);
                graphics.renderTexture(enter1, 205, 446);
                graphics.renderTexture(pressany, 205, 506);
                graphics.presentScene();
                waitUntilKeyPressed();
                graphics.prepareScene(background);
                if (isfacingdown)
                {
                    graphics.render(cha.x, cha.y, movedown);
                }
                else if (isfacingup)
                {
                    graphics.render(cha.x, cha.y, moveup);

                }
                else if (isfacingright)
                {
                    graphics.render(cha.x, cha.y, moveright);

                }
                else
                {
                    graphics.render(cha.x, cha.y, moveleft);
                }
                graphics.renderTexture(bossdial, 36, 384);
                graphics.render(boss.x, boss.y, bossidle);
                graphics.renderTexture(boss1, 205, 446);
                graphics.renderTexture(boss2, 205, 466);
                graphics.renderTexture(pressany, 205, 506);
                graphics.presentScene();
                waitUntilKeyPressed();
                graphics.prepareScene(background);
                if (isfacingdown)
                {
                    graphics.render(cha.x, cha.y, movedown);
                }
                else if (isfacingup)
                {
                    graphics.render(cha.x, cha.y, moveup);

                }
                else if (isfacingright)
                {
                    graphics.render(cha.x, cha.y, moveright);

                }
                else
                {
                    graphics.render(cha.x, cha.y, moveleft);
                }
                graphics.renderTexture(chadial, 36, 384);
                graphics.render(boss.x, boss.y, bossidle);
                graphics.renderTexture(cha1, 205, 446);
                graphics.renderTexture(pressany, 205, 506);
                graphics.renderTexture(tips, 205, 526);
                graphics.presentScene();
                waitUntilKeyPressed();
                dial.third = 0;
            }
        {
            switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_P)
                {
                    pause = !pause;
                   
                }
                if (!pause)
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_T)
                    {
                        boss.heartamount -= 3;
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                    {

                        for (int i = 1;i <= 7;i++)
                        {

                            graphics.prepareScene(background);
                            if (ms.showing)
                            {
                                graphics.renderTexture(msword,ms.x, ms.y);
                            }
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
                            if (boss.isalive)
                            {
                                if (boss.lookingright(cha))
                                {
                                    graphics.render(boss.x, boss.y, bossidle);
                                    bossidle.tick();

                                }
                                else
                                {
                                    graphics.render(boss.x, boss.y, bossidlel);
                                    bossidlel.tick();
                                }
                                bossidle.tick();

                                if (!boss.usingfireball)
                                {
                                    graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                }
                                else
                                {
                                    graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                }
                                graphics.renderTexture(bossheartt, bossh.x, bossh.y);
                            }
                            graphics.presentScene();

                            SDL_Delay(50);
                        }

                        if (boss.isalive)
                        {
                            if ((cha.isburnt(vecfire, firestate, flimit) && !boss.usingfireball) || (boss.usingfireball, cha.isfireballed(vecfireball, fireballstate)))
                            {
                                cha.heartamount--;
                            }
                        }
                        if (boss.isalive)
                        {
                            if (!boss.usingfireball)
                            {
                                firestate = (firestate + 1) % 4;
                            }
                            else
                            {
                                fireballstate = (fireballstate + 1) % 4;
                            }
                            if (firestate == 3)
                            {
                                boss.usingfireball = 1;
                                firestate = (firestate + 1) % 4;
                            }
                            else if (fireballstate == 3)
                            {
                                boss.usingfireball = 0;
                                fireballstate = (fireballstate + 1) % 4;
                            }
                        }

                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_Z)
                    {

                        if (isfacingright)
                        {
                            for (int i = 1;i <= 7;i++)
                            {
                                graphics.prepareScene(background);
                                if (ms.showing)
                            {
                                graphics.renderTexture(msword,ms.x, ms.y);
                            }
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if(boss.isalive) graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                graphics.render(cha.x, cha.y, slashright);

                                if (boss.isalive)
                                {
                                    if (!boss.usingfireball)
                                    {
                                        graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                    }
                                    else
                                    {
                                        graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                    }
                                    
                                }
                                if (i <= 4)
                                {
                                    slashright.tick();
                                }
                                if (boss.isalive)
                                {
                                    if (boss.lookingright(cha))
                                    {
                                        graphics.render(boss.x, boss.y, bossidle);
                                        bossidle.tick();

                                    }
                                    else
                                    {
                                        graphics.render(boss.x, boss.y, bossidlel);
                                        bossidlel.tick();
                                    }
                                }
                                graphics.presentScene();

                                SDL_Delay(50);

                            }
                        }
                        else if (isfacingleft)
                        {
                            for (int i = 1;i <= 7;i++)
                            {
                                
                                graphics.prepareScene(background);
                                if (ms.showing)
                                {
                                    graphics.renderTexture(msword, ms.x, ms.y);
                                }
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if(boss.isalive) graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                if (slashleft.currentFrame != 1)
                                {
                                    graphics.render(cha.x, cha.y, slashleft);
                                }
                                else
                                {
                                    graphics.render(cha.x - 30, cha.y, slashleft);
                                }
                                if (boss.isalive)
                                {
                                    if (boss.lookingright(cha))
                                    {
                                        graphics.render(boss.x, boss.y, bossidle);
                                        bossidle.tick();

                                    }
                                    else
                                    {
                                        graphics.render(boss.x, boss.y, bossidlel);
                                        bossidlel.tick();
                                    }
                                }

                                if (boss.isalive)
                                {
                                    if (!boss.usingfireball)
                                    {
                                        graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                    }
                                    else
                                    {
                                        graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                    }

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
                                if (ms.showing)
                                {
                                    graphics.renderTexture(msword, ms.x, ms.y);
                                }
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if(boss.isalive) graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                graphics.render(cha.x, cha.y, slashdown);
                                if (boss.isalive)
                                {
                                    if (boss.lookingright(cha))
                                    {
                                        graphics.render(boss.x, boss.y, bossidle);
                                        bossidle.tick();

                                    }
                                    else
                                    {
                                        graphics.render(boss.x, boss.y, bossidlel);
                                        bossidlel.tick();
                                    }
                                }
                                if (boss.isalive)
                                {
                                    if (!boss.usingfireball)
                                    {
                                        graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                    }
                                    else
                                    {
                                        graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                    }

                                }
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
                                if (ms.showing)
                                {
                                    graphics.renderTexture(msword, ms.x, ms.y);
                                }
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if(boss.isalive) graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                graphics.render(cha.x, cha.y, slashup);
                                if (boss.isalive)
                                {
                                    if (boss.lookingright(cha))
                                    {
                                        graphics.render(boss.x, boss.y, bossidle);
                                        bossidle.tick();

                                    }
                                    else
                                    {
                                        graphics.render(boss.x, boss.y, bossidlel);
                                        bossidlel.tick();
                                    }
                                }
                                if (boss.isalive)
                                {
                                    if (!boss.usingfireball)
                                    {
                                        graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                    }
                                    else
                                    {
                                        graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                    }

                                }

                                if (i <= 4)  slashup.tick();

                                graphics.presentScene();

                                SDL_Delay(50);
                            }

                        }
                        if (cha.isburnt(vecfire, firestate, flimit))
                        {
                            cha.heartamount--;
                        }
                        if (boss.isalive)
                        {
                            if (!boss.usingfireball)
                            {
                                firestate = (firestate + 1) % 4;
                            }
                            else
                            {
                                fireballstate = (fireballstate + 1) % 4;
                            }
                            if (firestate == 3)
                            {
                                boss.usingfireball = 1;
                                firestate = (firestate + 1) % 4;
                            }
                            else if (fireballstate == 3)
                            {
                                boss.usingfireball = 0;
                                fireballstate = (fireballstate + 1) % 4;
                            }
                        }
                        if (cha.hitheart(bossh, isfacingright, isfacingleft, isfacingup, isfacingdown))
                        {
                            boss.heartamount--;
                        }
 
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_X)
                    {
                        if (isfacingright)
                        {
                            if (cha.notborderjump('r')&&cha.notbossloc(1,'r',ms.showing) && cha.notheartloc(1, 'r',bossh))
                            {
                                idling = 0;

                                for (int i = 1;i <= 14;i++)
                                {
                                    cha.turnEast();
                                    graphics.prepareScene(background);
                                    if (ms.showing)
                                    {
                                        graphics.renderTexture(msword, ms.x, ms.y);
                                    }
                                    graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                   if(boss.isalive) graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                    cha.move();
                                    if (i <= 7)
                                    {
                                        graphics.render(cha.x, cha.y - 4 * i, moveright);
                                    }
                                    else
                                    {
                                        graphics.render(cha.x, cha.y - (14 - i) * 4, moveright);
                                    }
                                    if (boss.isalive)
                                    {
                                        if (boss.lookingright(cha))
                                        {
                                            graphics.render(boss.x, boss.y, bossidle);
                                            bossidle.tick();

                                        }
                                        else
                                        {
                                            graphics.render(boss.x, boss.y, bossidlel);
                                            bossidlel.tick();
                                        }

                                        if (!boss.usingfireball)
                                        {
                                            graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                        }
                                        else
                                        {
                                            graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                        }
                                    }
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
                            if (cha.notborderjump('u') && cha.notbossloc(1,'u',ms.showing) && cha.notheartloc(1, 'u', bossh))
                            {
                                idling = 0;
                                for (int i = 1;i <= 14;i++)
                                {

                                    cha.turnNorth();
                                    graphics.prepareScene(background);
                                    if (ms.showing)
                                    {
                                        graphics.renderTexture(msword, ms.x, ms.y);
                                    }
                                    graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                    if (boss.isalive) graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                    cha.move();
                                    if (i <= 7)
                                    {
                                        graphics.render(cha.x, cha.y - 4 * i, moveup);
                                    }
                                    else
                                    {
                                        graphics.render(cha.x, cha.y - (14 - i) * 4, moveup);
                                    }
                                    if (boss.isalive)
                                    {
                                        if (boss.lookingright(cha))
                                        {
                                            graphics.render(boss.x, boss.y, bossidle);
                                            bossidle.tick();

                                        }
                                        else
                                        {
                                            graphics.render(boss.x, boss.y, bossidlel);
                                            bossidlel.tick();
                                        }
                                        if (!boss.usingfireball)
                                        {
                                            graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                        }
                                        else
                                        {
                                            graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                        }
                                    }
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
                            if (cha.notborderjump('d') && cha.notbossloc(1,'d',ms.showing) && cha.notheartloc(1, 'd', bossh))
                            {
                                idling = 0;

                                for (int i = 1;i <= 14;i++)
                                {
                                    cha.turnSouth();
                                    graphics.prepareScene(background);
                                    if (ms.showing)
                                    {
                                        graphics.renderTexture(msword, ms.x, ms.y);
                                    }
                                    graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                    if (boss.isalive)  graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                    cha.move();
                                    if (i <= 7)
                                    {
                                        graphics.render(cha.x, cha.y - 4 * i, movedown);
                                    }
                                    else
                                    {
                                        graphics.render(cha.x, cha.y - (14 - i) * 4, movedown);
                                    }
                                    if (boss.isalive)
                                    {
                                        if (boss.lookingright(cha))
                                        {
                                            graphics.render(boss.x, boss.y, bossidle);
                                            bossidle.tick();

                                        }
                                        else
                                        {
                                            graphics.render(boss.x, boss.y, bossidlel);
                                            bossidlel.tick();
                                        }
                                        if (!boss.usingfireball)
                                        {
                                            graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                        }
                                        else
                                        {
                                            graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                        }
                                    }
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
                            if (cha.notborderjump('l') && cha.notbossloc(1,'l',ms.showing) && cha.notheartloc(1, 'l', bossh))
                            {
                                idling = 0;

                                for (int i = 1;i <= 14;i++)
                                {
                                    graphics.prepareScene(background);
                                    if (ms.showing)
                                    {
                                        graphics.renderTexture(msword, ms.x, ms.y);
                                    }
                                    graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                    if (boss.isalive)  graphics.renderTexture(bossheartt, bossh.x, bossh.y);

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
                                    if (boss.isalive)
                                    {
                                        if (boss.lookingright(cha))
                                        {
                                            graphics.render(boss.x, boss.y, bossidle);
                                            bossidle.tick();

                                        }
                                        else
                                        {
                                            graphics.render(boss.x, boss.y, bossidlel);
                                            bossidlel.tick();
                                        }
                                        if (!boss.usingfireball)
                                        {
                                            graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                        }
                                        else
                                        {
                                            graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                        }
                                    }
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
                        if (boss.isalive)
                        {
                            if ((cha.isburnt(vecfire, firestate, flimit) && !boss.usingfireball) || (boss.usingfireball, cha.isfireballed(vecfireball, fireballstate)))
                            {
                                cha.heartamount--;
                            }
                        }
                        if (boss.isalive)
                        {
                            if (cha.moved)
                            {
                                if (!boss.usingfireball)
                                {
                                    firestate = (firestate + 1) % 4;
                                }
                                else
                                {
                                    fireballstate = (fireballstate + 1) % 4;
                                }
                            }
                        }
                        if (boss.isalive)
                        {
                            if (firestate == 3)
                            {
                                boss.usingfireball = 1;
                                firestate = (firestate + 1) % 4;
                            }
                            else if (fireballstate == 3)
                            {
                                boss.usingfireball = 0;
                                fireballstate = (fireballstate + 1) % 4;
                            }
                        }


                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                    {
                        if (cha.notborder('u') && cha.notbossloc( 0,'u',ms.showing) && cha.notheartloc(0, 'u', bossh))
                        {
                            idling = 0;
                            for (int i = 1;i <= 7;i++)
                            {

                                cha.turnNorth();
                                graphics.prepareScene(background);
                                if (ms.showing)
                                {
                                    graphics.renderTexture(msword, ms.x, ms.y);
                                }
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if (boss.isalive)graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                cha.move();
                                graphics.render(cha.x, cha.y, moveup);
                                if (boss.isalive)
                                {
                                    if (boss.lookingright(cha))
                                    {
                                        graphics.render(boss.x, boss.y, bossidle);
                                        bossidle.tick();

                                    }
                                    else
                                    {
                                        graphics.render(boss.x, boss.y, bossidlel);
                                        bossidlel.tick();
                                    }
                                    if (!boss.usingfireball)
                                    {
                                        graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                    }
                                    else
                                    {
                                        graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                    }
                                }
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
                        if (boss.isalive)
                        {
                            if ((cha.isburnt(vecfire, firestate, flimit) && !boss.usingfireball) || (boss.usingfireball, cha.isfireballed(vecfireball, fireballstate)))
                            {
                                cha.heartamount--;
                            }
                            if (cha.moved)
                            {
                                if (!boss.usingfireball)
                                {
                                    firestate = (firestate + 1) % 4;
                                }
                                else
                                {
                                    fireballstate = (fireballstate + 1) % 4;
                                }
                            }
                            if (firestate == 3)
                            {
                                boss.usingfireball = 1;
                                firestate = (firestate + 1) % 4;
                            }
                            else if (fireballstate == 3)
                            {
                                boss.usingfireball = 0;
                                fireballstate = (fireballstate + 1) % 4;
                            }
                        }
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                    {
                        if (cha.notborder('l') && cha.notbossloc(0,'l',ms.showing) && cha.notheartloc(0, 'l', bossh))
                        {
                            idling = 0;
                            bool collided = false;
                            for (int i = 1;i <= 7;i++)
                            {
                                graphics.prepareScene(background);
                                if (ms.showing)
                                {
                                    graphics.renderTexture(msword, ms.x, ms.y);
                                }
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if (boss.isalive)   graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                cha.turnWest();

                                cha.move();
                                graphics.render(cha.x, cha.y, moveleft);
                                if (boss.isalive)
                                {
                                    if (boss.lookingright(cha))
                                    {
                                        graphics.render(boss.x, boss.y, bossidle);
                                        bossidle.tick();

                                    }
                                    else
                                    {
                                        graphics.render(boss.x, boss.y, bossidlel);
                                        bossidlel.tick();
                                    }
                                    if (!boss.usingfireball)
                                    {
                                        graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                    }
                                    else
                                    {
                                        graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                    }
                                }
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
                        if (boss.isalive)
                        {
                            if ((cha.isburnt(vecfire, firestate, flimit) && !boss.usingfireball) || (boss.usingfireball, cha.isfireballed(vecfireball, fireballstate)))
                            {
                                cha.heartamount--;
                            }
                            if (cha.moved)
                            {
                                if (!boss.usingfireball)
                                {
                                    firestate = (firestate + 1) % 4;
                                }
                                else
                                {
                                    fireballstate = (fireballstate + 1) % 4;
                                }
                            }
                            if (firestate == 3)
                            {
                                boss.usingfireball = 1;
                                firestate = (firestate + 1) % 4;
                            }
                            else if (fireballstate == 3)
                            {
                                boss.usingfireball = 0;
                                fireballstate = (fireballstate + 1) % 4;
                            }
                        }

                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                    {
                        if (cha.notborder('r') && cha.notbossloc(0,'r',ms.showing) && cha.notheartloc(0, 'r', bossh))
                        {
                            bool collided = false;
                            idling = 0;

                            for (int i = 1;i <= 7;i++)
                            {
                                cha.turnEast();
                                graphics.prepareScene(background);
                                if (ms.showing)
                                {
                                    graphics.renderTexture(msword, ms.x, ms.y);
                                }
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if (boss.isalive)   graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                cha.move();
                                graphics.render(cha.x, cha.y, moveright);
                                if (boss.isalive)
                                {
                                    if (boss.lookingright(cha))
                                    {
                                        graphics.render(boss.x, boss.y, bossidle);
                                        bossidle.tick();

                                    }
                                    else
                                    {
                                        graphics.render(boss.x, boss.y, bossidlel);
                                        bossidlel.tick();
                                    }

                                    if (!boss.usingfireball)
                                    {
                                        graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                    }
                                    else
                                    {
                                        graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                    }
                                }
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
                        if (boss.isalive)
                        {
                            if ((cha.isburnt(vecfire, firestate, flimit) && !boss.usingfireball) || (boss.usingfireball, cha.isfireballed(vecfireball, fireballstate)))
                            {
                                cha.heartamount--;
                            }
                            if (cha.moved)
                            {
                                if (!boss.usingfireball)
                                {
                                    firestate = (firestate + 1) % 4;
                                }
                                else
                                {
                                    fireballstate = (fireballstate + 1) % 4;
                                }
                            }
                            if (firestate == 3)
                            {
                                boss.usingfireball = 1;
                                firestate = (firestate + 1) % 4;
                            }
                            else if (fireballstate == 3)
                            {
                                boss.usingfireball = 0;
                                fireballstate = (fireballstate + 1) % 4;
                            }
                        }

                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                    {
                        if (cha.notborder('d') && cha.notbossloc(0,'d',ms.showing) && cha.notheartloc(0, 'd', bossh))
                        {
                            idling = 0;

                            for (int i = 1;i <= 7;i++)
                            {
                                cha.turnSouth();
                                graphics.prepareScene(background);
                                if (ms.showing)
                                {
                                    graphics.renderTexture(msword, ms.x, ms.y);
                                }
                                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                                if (boss.isalive)graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                                cha.move();
                                if (boss.isalive)
                                {
                                    if (boss.lookingright(cha))
                                    {
                                        graphics.render(boss.x, boss.y, bossidle);
                                        bossidle.tick();

                                    }
                                    else
                                    {
                                        graphics.render(boss.x, boss.y, bossidlel);
                                        bossidlel.tick();
                                    }
                                    if (!boss.usingfireball)
                                    {
                                        graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                                    }
                                    else
                                    {
                                        graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, i, warninglane);
                                    }
                                }
                                graphics.render(cha.x, cha.y, movedown);

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
                        if ((cha.isburnt(vecfire, firestate, flimit) && !boss.usingfireball) || (boss.usingfireball, cha.isfireballed(vecfireball, fireballstate)))
                        {
                            cha.heartamount--;
                        }
                        if (boss.isalive)
                        {
                            if (cha.moved)
                            {
                                if (!boss.usingfireball)
                                {
                                    firestate = (firestate + 1) % 4;
                                }
                                else
                                {
                                    fireballstate = (fireballstate + 1) % 4;
                                }
                            }
                            if (firestate == 3)
                            {
                                boss.usingfireball = 1;
                                firestate = (firestate + 1) % 4;
                            }
                            else if (fireballstate == 3)
                            {
                                boss.usingfireball = 0;
                                fireballstate = (fireballstate + 1) % 4;
                            }
                        }

                    }
                }
                break;
            case SDL_KEYUP:

                break;

            }
            if (idling)
            {
                cha.moved = 0;
                graphics.prepareScene(background);
                if (ms.showing)
                {
                    graphics.renderTexture(msword, ms.x, ms.y);
                }
                if (boss.isalive)
                    graphics.renderTexture(bossheartt, bossh.x, bossh.y);

                if (boss.isalive)
                {
                    if (!boss.usingfireball)
                    {
                        graphics.renderfire(firestate, graphics, vecfire, warning, burning, flimit);
                    }
                    else if (fireballstate != 2)
                    {
                        graphics.renderfireball(graphics, vecfireball, fireballstate, fireballleft, fireballright, fireballloc, 0, warninglane);
                    }
                }
                if (isfacingup)
                {

                    graphics.render(cha.x, cha.y, moveup);
                    if (!pause)
                    {
                        moveup.tick();
                    }
                }
                else if (isfacingleft)
                {
                    if (!pause)  moveleft.tick();
                    graphics.render(cha.x, cha.y, moveleft);
                }
                else if (isfacingright)
                {
                    graphics.render(cha.x, cha.y, moveright);
                    if (!pause) moveright.tick();

                }
                else
                {
                    graphics.render(cha.x, cha.y, idle);
                    if (!pause) idle.tick();

                }
                if (boss.isalive)
                {
                    if (boss.lookingright(cha))
                    {
                        graphics.render(boss.x, boss.y, bossidle);
                        if (!pause)  bossidle.tick();

                    }
                    else
                    {
                        graphics.render(boss.x, boss.y, bossidlel);
                        if (!pause)  bossidlel.tick();
                    }
                }
                graphics.renderheart(heartless, heart, cha.heartamount, graphics);
                if (pause)
                {
                    graphics.renderTexture(pauseui, 215, 110);
                    graphics.renderTexture(PAUSE, 320, 134);
                    SDL_GetMouseState(&x, &y);
                    if ((315 <= x && x <= 480) && (y <= 295 && y >= 240))
                    {
                        continuesui.hovered = true;
                    }
                    else { continuesui.hovered = false; }
                    if ((x >= 314 && x <= 480) && (y >= 327 && y <= 384))
                    {
                        menupauseui.hovered = true;
                    }
                    else
                    {
                        menupauseui.hovered = false;
                    }

                    if (!continuesui.hovered)
                    {
                        graphics.renderTexture(continues, 333, 252);
                    }
                    else
                    {
                        graphics.renderTexture(continueshovered, 333, 254);
                    }
                    if (!menupauseui.hovered)
                    {
                        graphics.renderTexture(menupause, 360, 336);
                    }
                    else
                    {
                        graphics.renderTexture(menupausehovered, 360, 338);
                    }
                    if (!muted)
                    {
                        graphics.renderTexture(volume, 373, 427);
                        Mix_ResumeMusic();
                    }
                    else
                    {
                        graphics.renderTexture(volumemuted, 373, 427);
                        Mix_PauseMusic();
                    }
                    switch (event.type)
                    {
                    case SDL_MOUSEBUTTONDOWN:
                        cerr << x << " " << y << endl;
                        if ((315 <= x && x <= 480) && (y <= 295 && y >= 240))
                        {
                            pause = false;
                        }
                        if ((x >= 376 && x <= 429) && (y >= 428 && y <= 453))
                        {
                            muted = !muted;
                        }
                        if ((x >= 314 && x <= 480) && (y >= 327 && y <= 384))
                        {
                            changedmap = 1;
                            currentmap = -3;
                            pause = false;
                        }
                        break;
                    case SDL_MOUSEBUTTONUP:
                        break;
                    }
                }
                graphics.presentScene();
               
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
                changedmap = true;
                currentmap = 3;
            }
            if (boss.heartamount <= 0 && boss.isalive)
            {
                for (int i = 1;i <= 5;i++)
                {
                    graphics.prepareScene(background);
                    switch (i)
                    {
            case 1:
                graphics.render(boss.x-10, boss.y, bossdeath);
                break;
            case 2:
                graphics.render(368, 156, bossdeath);
                break;
            case 3:
                graphics.render(367, 195, bossdeath);
                break;
            case 4:
                graphics.render(358, 258, bossdeath);
                break;
            case 5:
                graphics.render(363,299 , bossdeath);
                break;
     
             
                }
                    bossdeath.tick();
                    graphics.presentScene();

                    SDL_Delay(500);
                }
                boss.isalive = 0;
                ms.showing = true;
            }
            if (inside(ms.locx, ms.locy, cha.getrect()))
            {
                pause = true;
            }
            break;
            case 3:
                graphics.prepareScene(background);
               
                graphics.renderTexture(gameover,205,101);           
                SDL_GetMouseState(&x, &y);
                if ((x <= 492 && x >= 295) && (y <= 313 && y >= 270))
                {
                    replay.hovered = true;
                }
                else
                {
                    replay.hovered = false;
                }
                if (!replay.hovered)
                {
                    graphics.renderTexture(playagain, 295, 270);
                }
                else
                {
                    graphics.renderTexture(playagainhover, 295, 270);
                }
                switch (event.type)
                {             
                case SDL_MOUSEBUTTONDOWN:
                    if ((x <= 492 && x >= 295) && (y <= 313 && y >= 270))
                    {
                        changedmap = 1;
                        currentmap = -3;
                    }
                   
                    break;
                case SDL_MOUSEBUTTONUP:
                    break;
                }
                graphics.presentScene();
            break;
         
        }

        
        SDL_Delay(100);


 
       }
    }
  }
    graphics.quit();
    return 0;
}
