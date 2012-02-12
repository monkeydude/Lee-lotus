#include "foundation.h"

// Create the database of Game Data
GD gd;

// Return pointer to the database
GD * GameData()
{
	return &gd;
}

// Constructor
GD::GD()
{
	// Game has not yet loaded.
	this->gLoaded = 0;
	this->gRunning = 1;

	// Append default moves
	this->Moves.push_back(TM_NONE);
	this->Moves.push_back(TM_NONE);
	this->Moves.push_back(TM_NONE);

	// Create Rules
	this->CreateRules();

	// Set the scene to default
	this->SceneState = SCENE_DEFAULT;
}

// Mouse Input ([-1, -1] is valid)
void GD::SetLastClick(int x, int y)
{
	this->lastclick.x = x;
	this->lastclick.y = y;
}

Point2i GD::GetLastClick()
{
	return this->lastclick;
}

void GD::ResetLastClick()
{
	this->lastclick.x = -1;
	this->lastclick.y = -1;
}

// Create a player
void GD::CreatePlayer()
{
	Player newplayer;
	
	//Set the start state for this player as NULL, it will receive a proper state later once more game data is available for the state to process
	this->states.push_back(NULL);
	this->players.push_back(newplayer);
	this->numplayers++;
}

// Get Values
int GD::GetNumPlayers()
{
	return this->numplayers;
}

// Create the board
void GD::CreateBoard()
{
	this->board.CreateBoard(this->numplayers);
	this->bCreated = 1;
}

// Set Board BackgroundBoard Common Functions
void GD::SetBoardBackground(PNG * bg)
{
	this->board.SetBackground(bg);
}

//Load in the pieces...
void GD::SetBoardPieces(PNG * unit_white, PNG * unit_black, PNG * unit_red, PNG * unit_blue)
{
	this->board.SetPieces(unit_white, unit_black, unit_red, unit_blue);
}

//Load array of images to numbers...
void GD::SetBoardNumbers(PNG img_num[24])
{
	this->board.SetNumbers(img_num);
}

// Record a move
void GD::RecordMove(TMove move)
{
	// Add new and erase previous
	this->Moves.push_back(move);
	this->Moves.erase(this->Moves.begin());
}

// Create all of the possible rules
#define TMOVE_CONDITIONS 4
#define TMOVE_RESULTS TMOVE_CONDITIONS - 1

void GD::CreateRules()
{
	// Cycle through the possible conditions and results
	this->Rules.resize(TMOVE_CONDITIONS * TMOVE_CONDITIONS * TMOVE_CONDITIONS * TMOVE_RESULTS);
	int rulenum = 0;

	for (int A = 0; A < TMOVE_CONDITIONS; A++)
	{
		for (int B = 0; B < TMOVE_CONDITIONS; B++)
		{
			for (int C = 0; C < TMOVE_CONDITIONS; C++)
			{
				for (int D = 1; D <= TMOVE_RESULTS; D++)
				{
					this->Rules.at(rulenum).SetRule(rulenum, (TMove)A, (TMove)B, (TMove)C, (TMove)D);
					rulenum++;
				}
			}
		}
	}
}