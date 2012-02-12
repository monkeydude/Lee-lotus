#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "StateAI.h"

enum Scene {SCENE_DEFAULT, SCENE_SELECT_PLAYERS, SCENE_MAINBOARD, SCENE_RESULTS};

class GD
{
public:
	Board board;

	PNG LoadingImage;
	PNG LotusBoard;
	PNG LotusWhite;
	PNG LotusBlack;
	PNG LotusRed;
	PNG LotusBlue;
	PNG LotusNumbers[24];

	PNG LotusTurn;
	PNG LotusWinner;
	PNG LotusFrontBoard;
	PNG LotusPlayers;
	PNG LotusPlayer1;
	PNG LotusPlayer2;
	PNG LotusPlayer3;
	PNG LotusPlayer4;
	PNG LotusAIRule;
	PNG LotusAIState;
	PNG LotusHuman;
	PNG LotusFinished;
	PNG LotusNotPlaying;
	PlayerType player1;
	PlayerType player2;
	PlayerType player3;
	PlayerType player4;

	bool gLoaded;
	bool gRunning;
	bool bCreated;

	bool selectPlayerType;

	int numplayers;
	int currentPlayer;
	vector<Player> players;
	//vector of states where each position refers to the state of that player (some positions will never be used if that player is not a state AI)
	vector<BaseState*> states;


	Scene SceneState;

	vector<TRule> Rules;
	vector<TMove> Moves;

private:
	Point2i lastclick;

public:
	GD();

	void CreateBoard();
	void SetBoardBackground(PNG * bg);
	void SetBoardPieces(PNG * unit_white, PNG * unit_black, PNG * unit_red, PNG * unit_blue);
	void SetBoardNumbers(PNG img_num[24]);
	
	void SetLastClick(int x, int y);
	Point2i GetLastClick();
	void ResetLastClick();

	void CreatePlayer();
	int GetNumPlayers();

	void RecordMove(TMove move);
	void CreateRules();
};

// Return pointer to this class
GD * GameData();

#endif // GAMEDATA_H