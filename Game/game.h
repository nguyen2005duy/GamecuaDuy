
#ifndef GAME_H
#define GAME_H

#define INITIAL_SPEEDX 9//9.1714
#define INITIAL_SPEEDY 10//9.857142
#define INITIAL_RECSPEEDX 64/7
#define INITIAL_RECSPEEDY 70/7//68
#include <SDL.h>;
#include <SDL_image.h>
#include <iostream>
#include <vector>

bool inside(int x, int y, SDL_Rect r) {
    return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h;
}

bool overlap(const SDL_Rect& r1, const SDL_Rect& r2) {
    return inside(r1.x, r1.y, r2) || inside(r1.x + r1.w, r1.y, r2) ||
        inside(r1.x, r1.y + r1.h, r2) || inside(r1.x + r1.w, r1.y + r1.h, r2);
}
struct point
{
    int x;
    int y;
};
const char* KEYIMAGE = "img//key.png";
struct Key
{
    int x, y;
    bool showing = false;
    bool obtained = false;
    int locx;
    int locy;
};
struct En {
    SDL_Rect rect;
    
    double x, y;
    bool isfacingright = 0;
    bool isfacingleft = 0;
    bool isfacingup = 0;
    bool isfacingdown = 0;
    bool died = 0;
    int movelimit;
    int heartamount = 3;
    int limitcount=1;
    bool isalive = 1;
    double dx = 0, dy = 0;
    double recspeedx = INITIAL_RECSPEEDX;
    double recspeedy = INITIAL_RECSPEEDY;
    double drecx=0;
    double drecy=0;
    double speedx = INITIAL_SPEEDX;
    double speedy = INITIAL_SPEEDY;
    En(double _x, double _y,int _movelimit,int _lx, int _ly)
    {
        movelimit = _movelimit;
        x = _x;
        y = _y;
        rect.x = _lx;
        rect.y = _ly;
        rect.w = 1;
        rect.h = 1;
    }

    void move() {
        x += dx;
        y += dy;
        rect.x += drecx;
        rect.y += drecy;
    }
    
     void turnNorth() {
        dy = -speedy;
        dx = 0;
        drecx =  0;
        drecy = -recspeedy;
    }
    void turnSouth() {
        dy = speedy;
        dx = 0;
        drecx = 0;
        drecy = recspeedy;
    }
    void turnWest() {
        dy = 0;
        dx = -speedx;
        drecx = -recspeedx;
        drecy = 0;
    }
    void turnEast() {
        dy = 0;
        dx = speedx;
        drecx = recspeedx;
        drecy = 0;
    }
    int cx()
    {
        return rect.x;
    }
    int cy()
    {
        return rect.y;
    }
    bool notborder(char direction)
    {
        switch (direction)
        {
        case 'u':

            if (y - INITIAL_SPEEDY * 7 <= 30)
            {
                return false;
            }

            break;
        case 'd':

            if (y + INITIAL_SPEEDY * 7 >= 520)
            {
                return false;
            }
            break;
        case 'r':
            if (x + INITIAL_SPEEDX * 7 >= 700)
            {
                return false;
            }
            break;
        case 'l':
            if (x - INITIAL_SPEEDX * 7 <= 30)
            {
                return false;
            }
            break;
        }
        return true;
    }

};

struct Cha {
    SDL_Rect rect ;
    bool isfacingup = 0;
    bool isfacingdown = 1;
    bool isfacingright = 0;
    bool isfacingleft = 0;
    double x, y;
    // rect.x = 64;
     //rect.y = 82;
    int heartamount;
    double dx = 0, dy = 0;
    double drecx = 0, drecy = 0;
    double recspeedx = INITIAL_RECSPEEDX;
    double recspeedy = INITIAL_RECSPEEDY;
    double speedx = INITIAL_SPEEDX;
    double speedy = INITIAL_SPEEDY;
    Cha(int _x, int _y)
    {
        x = _x;
        y = _y;
        rect.x = 54;
        rect.y = 66;
        rect.w = 62;
        rect.h = 67;
     
    }
    void move() {
        x += dx;
        y += dy;
        rect.x += drecx;
        rect.y += drecy;
    }
    bool touchacid()
    {
        cout << rect.x <<" " << rect.y << endl;
        if (inside(214, 173, rect))
        {
            return true;
        }
        else if (inside(596, 172, rect))
        {
            return true;
        }
        else if (inside(405, 238, rect))
        {
            return true;
        }
        else if (inside(340, 310, rect))
        {
            return true;
        }
        else if (inside(468, 308, rect))
        {
            return true;
        }
        else if (inside(400, 380, rect))
        {
            return true;
        }
        else if (inside(214, 447, rect))
        {
            return true;
        }
        else if (inside(595, 448, rect))
        {
            return true;
        }
        return false;
    }
    int cx()
    {
        return rect.x;
    }
    int cy()
    {
        return rect.y;
    }
    void turnNorth() {
        dy = -speedy;
        drecy = -recspeedy;
        drecx = 0;
        dx = 0;
    }
    void turnSouth() {
        dy = speedy;
        drecy = recspeedy;
        drecx = 0;
        dx = 0;
    }
    void turnWest() {
        dy = 0;
        dx = -speedx;
        drecy = 0;
        drecx = -recspeedx;
    }
    void turnEast() {
        dy = 0;
        dx = speedx;
        drecy = 0;
        drecx = recspeedx;
    }
    bool notborder(char direction)
    {
        switch (direction)
        {
        case 'u':

            if (y - INITIAL_SPEEDY * 7 <= 30)
            {
                return false;
            }

            break;
        case 'd':

            if (y + INITIAL_SPEEDY * 7 >= 520)
            {
                return false;
            }
            break;
        case 'r':
            if (x + INITIAL_SPEEDX * 7 >= 700)
            {
                return false;
            }
            break;
        case 'l':
            if (x - INITIAL_SPEEDX * 7 <= 30)
            {
                return false;
            }
            break;
        }
        return true;
    }
    bool notborderjump(char direction)
    {
        switch (direction)
        {
        case 'u':

            if (y - INITIAL_SPEEDY * 14 <= 30)
            {
                return false;
            }

            break;
        case 'd':

            if (y + INITIAL_SPEEDY * 14 >= 520)
            {
                return false;
            }
            break;
        case 'r':
            if (x + INITIAL_SPEEDX * 14 >= 700)
            {
                return false;
            }
            break;
        case 'l':
            if (x - INITIAL_SPEEDX * 14 <= 30)
            {
                return false;
            }
            break;
        }
        return true;
    }
    bool ishurt(SDL_Rect monster)
    {
   
        return inside(monster.x,monster.y,rect);
    }
    bool slashthrough(SDL_Rect monster,bool isfacingright, bool isfacingup, bool isfacingleft, bool isfacingdown)
    {
        SDL_Rect sr;
        sr.w = 62;
        sr.h = 67;
        if (isfacingright)
        {
            sr.x = rect.x + 64;
                sr.y = rect.y;
        }
        else if (isfacingleft)
        {
            sr.x = rect.x - 64;
            sr.y = rect.y;
        }
        else if (isfacingdown)
        {
            sr.x = rect.x;
            sr.y = rect.y + 70;
        }
        else if (isfacingup)
        {
            sr.x = rect.x;
            sr.y = rect.y - 70;
        }
        
        return  inside(monster.x, monster.y, sr);

    }

};




bool gameOver(const Cha& mouse) {
    return mouse.x < 0 || mouse.x >= SCREEN_WIDTH ||
        mouse.y < 0 || mouse.y >= SCREEN_HEIGHT;
}

#endif // GAME_H