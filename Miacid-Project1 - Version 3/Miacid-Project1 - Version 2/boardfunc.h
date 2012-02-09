#ifndef BOARDFUNC_H
#define BOARDFUNC_H

#include "foundation.h"

//Board Common Functions
void Board_SetBackground(PNG * bg);
void Board_SetPieces(PNG * unit_white, PNG * unit_black, PNG * unit_red, PNG * unit_blue);
void Board_SetNumbers(PNG img_num[24]);
int Board_IsLocGood(int x1, int y1);
void Board_Render();

#endif //BOARDFUNC_H