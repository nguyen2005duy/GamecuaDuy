#ifndef _KEY__H
#define _KEY__H
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <vector>
const char* KEYIMAGE = "img//key.png";
struct Key
{
	int x, y;
	bool showing = false;
	bool obtained = false;
};
#endif