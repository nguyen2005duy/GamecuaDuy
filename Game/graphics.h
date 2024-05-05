﻿

// các hàm khác
#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <vector>
#include "game.h"
#define FIRELIMIT 30;
#define FIREBALLLIMIT 4;
using namespace std;
void renderTexture(SDL_Texture* texture, int x, int y,
    SDL_Renderer* renderer)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

SDL_Texture* loadTexture(const char* filename, SDL_Renderer* renderer)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
        "Loading %s", filename);

    SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (texture == NULL) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
            "Load texture %s", IMG_GetError());
    }

    return texture;
}
struct ui
{
    bool hovered = 0;
};
struct Sprite {
    SDL_Texture* texture;
    std::vector<SDL_Rect> clips;
    int currentFrame = 0;

    void init(SDL_Texture* _texture, int frames, const int _clips[][4]) {
        texture = _texture;

        SDL_Rect clip;
        for (int i = 0; i < frames; i++) {
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];
            clips.push_back(clip);
        }
    }
    void tick() {
        currentFrame = (currentFrame + 1) % clips.size();
    }

    const SDL_Rect* getCurrentClip() const {
        return &(clips[currentFrame]);
    }
};

struct Graphics {
    SDL_Renderer* renderer;
    SDL_Window* window;

    void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

    void init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        //full screen
        //window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit("SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC);
        //Khi chạy trong máy ảo (ví dụ phòng máy ở trường)
        //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        //Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            logErrorAndExit("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                Mix_GetError());
        }
        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
                TTF_GetError());
        }


    }

    void prepareScene(SDL_Texture* background = nullptr)
    {
        SDL_RenderClear(renderer);
        if (background != nullptr) SDL_RenderCopy(renderer, background, NULL, NULL);
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture* loadTexture(const char* filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture* texture, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }
   

    void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;

        SDL_RenderCopy(renderer, texture, src, &dest);
    }

    void quit()
    {
        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        Mix_Quit();
        TTF_Quit();

    }
    TTF_Font* loadFont(const char* path, int size)
    {
        TTF_Font* gFont = TTF_OpenFont(path, size);
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                SDL_LOG_PRIORITY_ERROR,
                "Load font %s", TTF_GetError());
        }
        return gFont;
    }
    SDL_Texture* renderText(const char* text,
        TTF_Font* font, SDL_Color textColor)
    {
        SDL_Surface* textSurface =
            TTF_RenderText_Solid(font, text, textColor);
        if (textSurface == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                SDL_LOG_PRIORITY_ERROR,
                "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture =
            SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                SDL_LOG_PRIORITY_ERROR,
                "Create texture from text %s", SDL_GetError());
        }
        SDL_FreeSurface(textSurface);
        return texture;
    }
    void render(int x, int y, const Sprite& sprite) {
        const SDL_Rect* clip = sprite.getCurrentClip();
        SDL_Rect renderQuad = { x, y, clip->w, clip->h };
        SDL_RenderCopy(renderer, sprite.texture, clip, &renderQuad);
    }
    void renderheart(SDL_Texture* heartless, SDL_Texture* heart, int heartamount, Graphics graphics)
    {
        for (int i = 0;i < heartamount;i++)
        {
            renderTexture(heart, 80 + i * 50, 556);

        }
        for (int i = 0;i < 3;i++)
        {
            renderTexture(heartless, 180 - 50 * (i), 556);
        }
    }
    Mix_Music* loadMusic(const char* path)
    {
        Mix_Music* gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                SDL_LOG_PRIORITY_ERROR,
                "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
    }
    void renderfireball(Graphics graphics, vector<fireball>& vecfireball, int fireballstate, Sprite& fireballleft, Sprite& fireballright,int &fireballloc,int check, SDL_Texture* warninglane)
    {
        switch (fireballstate)
        {
        case 0:
            for (int i = 0;i < 4; i++)
            {
                fireball newfireball;
                newfireball.generatefireballlocation();
                vecfireball[i] = newfireball;
            }
            break;
        case 1:
            for (int i = 0;i < 4; i++)
            {
                graphics.renderTexture(warninglane,62, vecfireball[i].y);
            }
            break;
        case 2:
            for (int i = 0;i < 4; i++)
            {
                graphics.render(62 + vecfireball[i].x * fireballloc*1, vecfireball[i].y, fireballleft);
                graphics.render(671 - vecfireball[i].x * fireballloc*1, vecfireball[i].y, fireballright);
            }
            fireballloc = fireballloc % 5 + 1;
            if (check<=3||check>=5)
            {
                fireballleft.tick();
                fireballright.tick();
            }
            
            break;
        }
   
    }
    void renderfire(int firestate, Graphics graphics, vector<fire> &vecfire, SDL_Texture* warning, SDL_Texture* burning, int &flimit)
    {
        switch (firestate)
        {
        case 0:
            flimit = rand() % FIRELIMIT + 1;
            while (flimit <= 10)
            {
                flimit = rand() % FIRELIMIT + 1;
            }
            for (int i = 0;i < flimit; i++)
            {
                fire newfire;
                newfire.generatefirelocation();
                vecfire[i] = newfire;
            }
            break;
        case 1:
            for (int i = 0;i < flimit;i++)
            {
                graphics.renderTexture(warning, vecfire[i].x, vecfire[i].y);
            }
            break;
        case 2:

            for (int i = 0;i < flimit;i++)
            {
                graphics.renderTexture(burning, vecfire[i].x, vecfire[i].y);
            }
            break;
        }
    }
    void play(Mix_Music* gMusic)
    {
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic(gMusic, -1);
        }
        else if (Mix_PausedMusic() == 1) {
            Mix_ResumeMusic();
        }
    }
    Mix_Chunk* loadSound(const char* path) {
        Mix_Chunk* gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                SDL_LOG_PRIORITY_ERROR,
                "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
    }
    void play(Mix_Chunk* gChunk) {
        if (gChunk != nullptr) {
            Mix_PlayChannel(-1, gChunk, 0);
        }
    }



  

};

#endif // _GRAPHICS__H
