#ifndef PLAYER_H
#define PLAYER_H

// Includes
#include "foundation.h"
#include <vector>

// Defines
#define PLAYERS_MAX 4

enum PTYPE {PLAYER_HUMAN, PLAYER_RULEBASED, PLAYER_STATEBASED};
enum PIECE {PIECE_BAD, PIECE_P1, PIECE_P2, PIECE_P3, PIECE_P4};

class Player
{
public:
	// General
	PIECE piece;
	bool isPlaying;
	bool isHuman;
	bool isRule; //if not, it is state or human

	// Rules
	// nothing here

	// States
	// to do...

	Player()
	{
		this->piece = PIECE_BAD;
		this->isPlaying = 0;
		this->isHuman = 1;
		this->isRule = 0;

		// some state code may go here..
	}
};

// Somewhat different from Player
class PlayerType
{
public:
	bool isPlaying;
	bool isHuman;
	bool isRule;
	
	PlayerType()
	{
		isPlaying=true;
		isHuman=true;
		isRule=false;
	}
};

#endif // PLAYER_H