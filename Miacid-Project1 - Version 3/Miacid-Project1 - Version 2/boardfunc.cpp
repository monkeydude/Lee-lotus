// Include the foundation
#include "foundation.h"

// Board Common Functions
void Board_SetBackground(PNG * bg)
{
	GameData()->board.SetBackground(bg);
}

// Load in the pieces...
void Board_SetPieces(PNG * unit_white, PNG * unit_black, PNG * unit_red, PNG * unit_blue)
{
	GameData()->board.SetPieces(unit_white, unit_black, unit_red, unit_blue);
}

// Load array of images to numbers...
void Board_SetNumbers(PNG img_num[24])
{
	GameData()->board.SetNumbers(img_num);
}

// Check based on click...
int Board_IsLocGood(int x1, int y1)
{
	return GameData()->board.IsLocGood(x1, y1);
}

// Draw the board
void Board_Render()
{
	GameData()->board.Render();
}