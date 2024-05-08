#ifndef _MONSTER__H
#define _MONSTER__H
const char* SLIMEFILE = "img//slime.png";
const int SLIME_IDLE_RIGHT_CLIPS[][4] = {
    {  23, 35, 51, 38},
    { 120,35 , 48, 38},
    { 216,35 , 48, 38},
    {312, 35, 48, 38},
};
const int SLIME_IDLE_LEFT_CLIPS[][4] = {
    {  24, 318, 51, 38},
    { 121,318 , 48, 38},
    { 217,318 , 48, 38},
    {313, 318, 48, 38},
};
const int SLIME_MOVE_RIGHT_CLIPS[][4] = {
    {  23, 134, 53, 35},
    { 126,108 , 40, 61},
    { 221,102 , 40, 67},
    {305, 112, 50, 55},
    {408,132,50,36},
    {504,135,51,33},
};
const int SLIME_MOVE_LEFT_CLIPS[][4] = {
    {  497, 228, 51, 33},
    { 409,200 , 40, 61},
    { 314,195 , 37, 67},
    {218, 207, 38, 53},
    {117,225,48,36},
    {21,228,49,33},
};
const int SLIME_DEATH_CLIPS[][4] = {
    {27,400,44,56},
    {123,398,40,57},
{216,393,43,61},
    {312,401,51,54},
{404,405,55,51}
};
const int BOSS_IDLE_CLIPS[][4] =
{
    {21,0,53,95},
{116, 0, 56, 95},
{214, 0, 54, 96},
    {308,0,57,96}
};
const int BOSS_IDLEL_CLIPS[][4] =
{
    {308,0,56,96},
    {212,0,53,96},
    {117,0,53,96},
    {20,0,54,96}
};
const int FIREBALL_LEFT_CLIPS[][4] =
{
    {8,15,70,70},
    {88,15,70,70},
    {160,15,70,70},
    {
        244,15,70,70
    },
    {322,16,70,70}
};
const int FIREBALL_RIGHT_CLIPS[][4] =
{
    {341,15,69,69},
    {261,15,69,69},
    {191,15,70,70},
    {103,14,69,69},
    {28,16,61,47},
};
const int BOSS_DEATH_CLIPS[][4]
{
    {67,105,60,87},
    {258,22,64,170},
    {444,59,66,133},
    {631,125,80,67},
    {829,163,80,30},
    {1020,156,72,35}
};
const int BOSS_DEATH_FRAME = sizeof(BOSS_DEATH_CLIPS) / sizeof(int) / 4;
const int FIREBALL_LEFT_FRAME = sizeof(FIREBALL_LEFT_CLIPS) / sizeof(int) / 4;
const int FIREBALL_RIGHT_FRAME = FIREBALL_LEFT_FRAME;
const int BOSS_IDLEL_FRAME = sizeof(BOSS_IDLEL_CLIPS) / sizeof(int) / 4;
const int BOSS_IDLE_FRAME = sizeof(BOSS_IDLE_CLIPS) / sizeof(int) / 4;
const int SLIME_DEATH_FRAME = sizeof(SLIME_DEATH_CLIPS) / sizeof(int) / 4;
const int SLIME_MOVE_RIGHT_FRAME = sizeof(SLIME_MOVE_RIGHT_CLIPS) / sizeof(int) / 4;
const int SLIME_MOVE_LEFT_FRAME = SLIME_MOVE_RIGHT_FRAME;
const int SLIME_IDLE_RIGHT_FRAME = sizeof(SLIME_IDLE_RIGHT_CLIPS) / sizeof(int) / 4;
const int SLIME_IDLE_LEFT_FRAME = SLIME_IDLE_RIGHT_FRAME;
#endif 