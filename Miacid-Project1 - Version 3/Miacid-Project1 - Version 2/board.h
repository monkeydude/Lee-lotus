#ifndef BOARD_H
#define BOARD_H

// Includes
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

#include "foundation.h"

//Player macros?
#define MAX_START_STACKS 12 //maximum amount of stacks for players at beginning of the game
#define MAX_GAME_POSITIONS 17 //17 spots on the game board
#define PCS_PER_PLAYER_2 10
#define PCS_PER_PLAYER_3OR4 6
#define LOTUS_TRAMPOLINE 10 //the id is 10, actual piece is 11 by the document given

class Board
{
private:
	int numplayers; //the number of players
	stack<PIECE> start[MAX_START_STACKS]; //12 original stack (4, 3, 2, 1 x 2 OR 3, 2, 1 x 4)
	stack<PIECE> position[MAX_GAME_POSITIONS]; //white, black, red, blue
	vector<PIECE> finish; //pieces that are in the finish location

	bool movingpiece; //if we are in the state of moving a piece (used for rendering)
	int movingstart; //start here...
	int movingend; //go to here

	Point2f locations[18];
	Point2f slocations[12];

	PNG * background;
	PNG * units[4];
	PNG * numbers[24];

	void UpdateStateAIs(int, int);

public:
	int numstartstacks;

public:
	Board();

	bool CreateBoard(int players);
	void CreateLocationsTable();
	void CreateStartLocationsTable();

	void SetBackground(PNG * bg);
	void SetPieces(PNG * unit_white, PNG * unit_black, PNG * unit_red, PNG * unit_blue);
	void SetNumbers(PNG img_num[24]);

	PIECE GetTopPiece(int pos);
	int GetDeepestPiece(PIECE player, int pos);

	bool GetLocationFromXY(int x, int y, int &result);

	bool IsPieceOnTop(PIECE player, int pos);
	bool IsPosEmpty(int pos);

	bool MovePiece(int begin, int end);
	bool PossibleMove(PIECE player);
	bool PlayerHasWon(PIECE player);
	int IsLocGood(int x1, int y1);

	int GetSizeOfStack(int pos);
	int PlayerPiecesRemaining(PIECE player);
	int BoardPiecesRemaining();

	void Render();
};

#endif