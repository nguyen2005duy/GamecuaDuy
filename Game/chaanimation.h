#ifndef _CHAANIMATION__H
#define _CHANAIMATION__H
const char* CHARACTERFILE = "img//player2.png";
const int IDLE_CLIPS[][4] = {
    {  54, 66, 40, 64},
    { 198, 66, 40, 64},
    {342, 66,40, 64},
    {486, 66, 40, 64},
       {630, 66,40, 64},
           {774, 66,40, 64},
};
const int MOVERIGHT_CLIPS[][4]
{
    {52,210,44,64},
    {196,210,44,64},
    {340,210,44,64},
    {484,214,44,64},
    {628,214,44,64},
{772, 214, 44, 64},
};
const int MOVELEFT_CLIPS[][4]
{
    {51,629,45,63},
    {195,629,45,63},
    {339,629,45,63},
    {483,629,45,63},
    {627,629,45,63},
{771, 629, 45, 63},
};
const int MOVEUP_CLIPS[][4]
{
    {55,355,41,64},
    {198,354,40,64},
    {342,354,40,64},
    {486,358,40,60},
    {630,358,40,60},
{774, 358, 40, 60},
};
const int MOVEDOWN_CLIPS[][4]
{
    {54,492,42,71},
    {197,492,42,71},
    {341,492,42,71},
    {485,492,42,71},
    {629,492,42,71},
{773, 492, 42, 71},
};
const int SLASHRIGHT_CLIPS[][4]
{
     {57,1078,50,61},
    {167,1078,104,80},
    {321,1078,63,75},
    {483,1078,45,60},
};
const int SLASHLEFT_CLIPS[][4]
{
    {470,776,50,65},
       {306,776,103,71},
    {194,776,61,64},
    {49,782,44,59},

};
const int SLASHDOWN_CLIPS[][4]
{
     {46,933,49,62},
    {194,930,62,97},
    {341,933,60,63},
    {484,936,41,58},
};
const int SLASHUP_CLIPS[][4]
{
    {53,1222,51,61},
    {178,1217,67,66},
    {322,1221,61,64},
    {483,1223,43,60}
};
const int CHADEATH_CLIPS[][4]
{
    {53,1372,49,58},
    {195,1372,56,57},
    {
        340,1372,62,53
    }

};
const int CHADEATH_FRAME = sizeof(CHADEATH_CLIPS) / sizeof(int) / 4;
const int SLASHUP_FRAME = sizeof(SLASHUP_CLIPS) / sizeof(int) / 4;
const int SLASHDOWN_FRAME = sizeof(SLASHDOWN_CLIPS) / sizeof(int) / 4;
const int SLASHLEFT_FRAME = sizeof(SLASHLEFT_CLIPS) / sizeof(int) / 4;
const int SLASHRIGHT_FRAME = sizeof(SLASHRIGHT_CLIPS) / sizeof(int) / 4;
const int MOVEDOWN_FRAME = sizeof(MOVEDOWN_CLIPS) / sizeof(int) / 4;
const int MOVEUP_FRAME = sizeof(MOVEUP_CLIPS) / sizeof(int) / 4;
const int MOVELEFT_FRAME = sizeof(MOVEUP_CLIPS) / sizeof(int) / 4;
const int MOVERIGHT_FRAME = sizeof(MOVERIGHT_CLIPS) / sizeof(int) / 4;
const int IDLE_FRAME = sizeof(IDLE_CLIPS) / sizeof(int) / 4;
#endif