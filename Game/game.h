
#ifndef GAME_H
#define GAME_H

#define INITIAL_SPEEDX 9//9.1714
#define INITIAL_SPEEDY 10//9.857142
#define INITIAL_RECSPEEDX 64/7
#define INITIAL_RECSPEEDY 70/7//68
#include "defs.h"
#include <SDL.h>;
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <cstdlib> // For rand() and srand()
#include <ctime>
using namespace std;
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
struct dialogue
{
    bool first=1;
    bool second=1;
    bool third=1;

};
const char* KEYIMAGE = "img//key.png";
struct fire
{

    int x;
    int y;
    int rx;
    int ry;
    void generatefirelocation()
    {
        int randx = rand() % 11;
        int randy = rand() % 7;
        while (randx == 5 || randy == 3)
        {
            randx = rand() % 11;
            randy = rand() % 7;
        }
        x = 65 + 61 * randx;
        y = 68 + 67 * randy;
        rx = 92 + 61 * randx;
        ry = 94 + 75 * randy;
    }//65  58 //

};
struct bossheart
{

    int x;
    int y;
    int rx;
    int ry;
    void generatebossheartlocation()
    {
        int randx = rand() % 11;
        int randy = rand() % 7;
        while (randx == 5 || randy == 3)
        {
            randx = rand() % 11;
            randy = rand() % 7;
        }
        x = 65 + 61 * randx;
        y = 68 + 67 * randy;
        rx = 92 + 61 * randx;
        ry = 94 + 75 * randy;
    }//65  58 //

};
struct fireball
{
    int x;
    int y;
    int ry;
    void generatefireballlocation()
    {
        int randy = rand() % 7;
        while ( randy == 3)
        {
            randy = rand() % 7;
        }
        x = 62;
        y = 61 + 69 * randy;
        ry = 94 + 70 * randy;
    }
};
struct Key
{
    int x, y;
    bool showing = false;
    bool obtained = false;
    int locx;
    int locy;
};
struct Cha {
    SDL_Rect rect;
    bool isfacingup = 0;
    bool isfacingdown = 1;
    bool isfacingright = 0;
    bool isfacingleft = 0;
    double x, y;
    bool moved;
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
    void reset()
    {
         x = 65;
        y = 50;
        rect.x = 54;
        rect.y = 66;
        rect.w = 62;
        rect.h = 67;
        heartamount = 3;
    }
    void newmap()
    {
        rect.x = 367;
        rect.y = 64;
        rect.w = 67;
        rect.h = 74;
        recspeedx = 63/7;
        recspeedy = 70 / 7;
    }
    void move() {
        moved = true;
        x += dx;
        y += dy;
        rect.x += drecx;
        rect.y += drecy;
    }
    bool touchacid()
    {
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

        return inside(monster.x, monster.y, rect);
    }
    bool slashthrough(SDL_Rect monster, bool isfacingright, bool isfacingup, bool isfacingleft, bool isfacingdown)
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
    bool isburnt(vector <fire> vecfire, int firestate,int flimit)
    {
        bool burnt = 0;
        if (firestate == 1)
        {
            for (size_t i = 0;i <flimit;i++)
            {
                if (inside(vecfire[i].rx, vecfire[i].ry, rect))
                {
                    burnt = true;
                }
            }
        }
        return burnt;
    }
    bool fireballed(int y, SDL_Rect rect)
    {
        return (rect.y+rect.h >= y) && (rect.y <= y);
    }
    bool isfireballed(vector<fireball>vecfireball, int fireballstate)
    {
        bool burnt = 0;
        if (fireballstate == 2)
        {
            for (size_t i = 0;i < 4;i++)
            {
                if (fireballed(vecfireball[i].ry,rect))
                {
                    burnt = 1;
                }
            }
        }
        return burnt;
    }
    SDL_Rect getrect()
    {
        return rect;
    }
    bool notbossloc( bool jump,char direction,bool showing)
    {
        SDL_Rect boss;
        if (direction=='r' && jump)
        {
            boss.x = rect.x + rect.w * 2;
            boss.y = rect.y;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction=='l' && jump)
        {
            boss.x = rect.x - rect.w * 2;
            boss.y = rect.y;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction=='u' && jump)
        {
            boss.x = rect.x;
            boss.y = rect.y - rect.h * 2;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction=='d' && jump)
        {
            boss.x = rect.x;
            boss.y = rect.y + rect.h * 2;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction=='r' && !jump)
        {
            boss.x = rect.x + rect.w;
            boss.y = rect.y;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction=='l' && !jump)
        {
            boss.x = rect.x - rect.w;
            boss.y = rect.y;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction=='u' && !jump)
        {
            boss.x = rect.x;
            boss.y = rect.y - rect.h;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction=='d' && !jump)
        {
            boss.x = rect.x;
            boss.y = rect.y + rect.h;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        return !inside(399, 303, boss)||showing;
    }
    bool notheartloc(bool jump, char direction,bossheart bossh)
    {
        SDL_Rect boss;
        if (direction == 'r' && jump)
        {
            boss.x = rect.x + rect.w * 2;
            boss.y = rect.y;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction == 'l' && jump)
        {
            boss.x = rect.x - rect.w * 2;
            boss.y = rect.y;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction == 'u' && jump)
        {
            boss.x = rect.x;
            boss.y = rect.y - rect.h * 2;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction == 'd' && jump)
        {
            boss.x = rect.x;
            boss.y = rect.y + rect.h * 2;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction == 'r' && !jump)
        {
            boss.x = rect.x + rect.w;
            boss.y = rect.y;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction == 'l' && !jump)
        {
            boss.x = rect.x - rect.w;
            boss.y = rect.y;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction == 'u' && !jump)
        {
            boss.x = rect.x;
            boss.y = rect.y - rect.h;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        else if (direction == 'd' && !jump)
        {
            boss.x = rect.x;
            boss.y = rect.y + rect.h;
            boss.w = rect.w;
            boss.h = rect.h;
        }
        return !inside(bossh.rx, bossh.ry, boss);
    }
    bool hitheart(bossheart bossh,bool isfacingright, bool isfacingleft, bool isfacingup,bool isfacingdown)
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
        if (inside(bossh.rx, bossh.ry, sr))
        {
            bossh.generatebossheartlocation();
            return 1;

        }
        return 0;
     
    }
};

struct En {
    SDL_Rect rect;
    bool usingfireball = 0;
    bool canhurt = 1;
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
    int resetx;
    int resety;
    int resetrectx;
    int resetrecty;
    En(double _x, double _y, int _movelimit, int _lx, int _ly)
    {
        movelimit = _movelimit;
        x = _x;
        resetx = x;
        y = _y;
        resety = y;
        rect.x = _lx;
        rect.y = _ly;
        resetrectx = rect.x;
        resetrecty = rect.y;
        rect.w = 1;
        rect.h = 1;
    }
    void reset()
    {
        x = resetx;
        y = resety;
        rect.x = resetrectx;
        rect.y = resetrecty;
        limitcount = 1;
        heartamount = 3;
        isalive = 1;
        died = 0;
    }
    bool lookingright(Cha cha)
    {
        if (cha.x >= SCREEN_WIDTH / 2)
        {
            return 1;

        }
        else return false;
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





bool gameOver(const Cha& mouse) {
    return mouse.x < 0 || mouse.x >= SCREEN_WIDTH ||
        mouse.y < 0 || mouse.y >= SCREEN_HEIGHT;
}

#endif // GAME_H