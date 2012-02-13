// Here it is. The core to our entire project :)

#include "foundation.h"
#include <vector>

class Move //CHG-D-01
{
  public:
	  Move(TMove useMov): useMov_(useMov){}

	  void format(TMove useMove, int ruleToFire, int topWeight, int highest,
		  int distance, int movepos, int potend, int endpos, int moveid, int path, bool moveUseful, 
		  vector<int> possibleActiveMoves, vector<int> possibleStartMoves, Player &player){ ////CHG-D-05

		executeMove(useMove, ruleToFire, topWeight, highest,
		   distance, movepos, potend, endpos, moveid,  path, moveUseful, 
		   possibleActiveMoves, possibleStartMoves, player); //CHG-D-06


	  }
protected:
	TMove useMov_;
private:
    virtual void executeMove(TMove useMove, int ruleToFire, int topWeight, int highest,
		  int distance, int movepos, int potend, int endpos, int moveid, int path, bool moveUseful, 
		  vector<int> possibleActiveMoves, vector<int> possibleStartMoves, Player &player) = 0; //CHG-D-06
};





class Attack: public Move //CHG-D-02
{
  public:
	  Attack(TMove useMov): Move(useMov){}


	private:
     /* virtual */void executeMove(TMove useMove, int ruleToFire, int topWeight, int highest,
		  int distance, int movepos, int potend, int endpos, int moveid, int path, bool moveUseful, 
		  vector<int> possibleActiveMoves, vector<int> possibleStartMoves, Player &player) //CHG-D-06
    {


	highest = -1; // reset
			for (int i = 0; i < (signed)possibleActiveMoves.size(); i++)
			{
				// Get numbers
				distance = GameData()->board.GetSizeOfStack(possibleActiveMoves.at(i)); //size of current stack
				potend = possibleActiveMoves.at(i) + distance;

				// Special case for left track
				if (possibleActiveMoves.at(i) <= 2 && potend > 2)
					potend += 3;

				if (GameData()->board.GetSizeOfStack(potend) >= highest)
				{
					highest = GameData()->board.GetSizeOfStack(potend);
					movepos = possibleActiveMoves.at(i);
				}
			}

			// Move piece
			GameData()->board.MovePiece(movepos, -1);

			if (highest < 2) // small stack -> not very useful
				moveUseful = 0;



    }
};

class Forward: public Move //CHG-D-03
{
  public:
	  Forward(TMove useMov): Move(useMov){}
	 private:
     /* virtual */void executeMove(TMove useMove, int ruleToFire, int topWeight, int highest,
		  int distance, int movepos, int potend, int endpos, int moveid, int path, bool moveUseful, 
		  vector<int> possibleActiveMoves, vector<int> possibleStartMoves, Player &player) //CHG-D-06
    {

		distance = -1; //reset

			for (int i = 0; i < (signed)possibleActiveMoves.size(); i++)
			{
				// Get numbers
				static int tempdist;
				tempdist = GameData()->board.GetSizeOfStack(possibleActiveMoves.at(i));

				// Can hit Lotus?
				if (i + tempdist == LOTUS_TRAMPOLINE)
					tempdist *= 2;

				if (tempdist >= distance)
				{
					distance = GameData()->board.GetSizeOfStack(possibleActiveMoves.at(i));
					movepos = possibleActiveMoves.at(i);
				}
			}

			// Move the piece
			if (distance < 3) // not particularly wortwhile
				moveUseful = 0;

			GameData()->board.MovePiece(movepos, -1);
    }
};

class Start: public Move //CHG-D-04
{
  public:
	  Start(TMove useMov): Move(useMov){}
	  private:
     /* virtual */void executeMove(TMove useMove, int ruleToFire, int topWeight, int highest,
		  int distance, int movepos, int potend, int endpos, int moveid, int path, bool moveUseful, 
		  vector<int> possibleActiveMoves, vector<int> possibleStartMoves, Player &player) //CHG-D-06
    {

		// Take out a piece from the start if possible
			// Otherwise perform a random action...
			// Useful if the start piece can jump on top of another stack
			// Not useful if a random move needed to be performed

			movepos = possibleStartMoves.at(0);

			// Decide on a path by taking the one with fewer pieces
			for (int i = 0; i < 7; i++)
			{
				if (i <= 2)
					path -= GameData()->board.GetSizeOfStack(i);
				else
					path += GameData()->board.GetSizeOfStack(i);
			}

			path *= -1; //flip it so you go down the path with fewer pieces

			// If there's only one stack left you must select from there
			if (possibleStartMoves.size() == 1)
				movepos = possibleStartMoves.at(0);

			// Go for the first stack that can jump on a piece (otherwise assume the above ^)
			for (int i = 0; i < possibleStartMoves.size(); i++)
			{
				static int tempend = -1;
				static PIECE temppc;

				if (path <= 0)
				{
					if (GameData()->board.GetSizeOfStack(possibleStartMoves.at(i)) == 4)
						tempend = 6;
					else
						tempend = GameData()->board.GetSizeOfStack(possibleStartMoves.at(i)) - 1;
				}
				else
					tempend = GameData()->board.GetSizeOfStack(possibleStartMoves.at(i)) + 2;

				temppc = GameData()->board.GetTopPiece(tempend);

				if (temppc != player.piece && temppc != PIECE_BAD)
				{
					moveid = i;
					movepos = possibleStartMoves.at(i);
					break;
				}
			}

			// Move the piece
			if (path <= 0) //left path
			{
				if (GameData()->board.GetSizeOfStack(possibleStartMoves.at(moveid)) == 4)
					endpos = 6;
				else
					endpos = GameData()->board.GetSizeOfStack(possibleStartMoves.at(moveid)) - 1;
			}
			else
				endpos = GameData()->board.GetSizeOfStack(possibleStartMoves.at(moveid)) + 2;

			// Update and and change your state
			GameData()->board.MovePiece(movepos, endpos);
    }
};

void setMove(TMove useMove, Move *move_){ //CHG-D-08
	delete move_;
	switch (useMove){

		case TM_ATTACK:
			cout<<"testtting"<<endl;
			move_ = new Attack(useMove);
			break;
		case TM_FORWARD:
			move_ = new Forward(useMove);
			break;
		case TM_START:
			move_ = new Start(useMove);
			break;
		default:
			move_ = NULL;
			break;
	}
}

void doIt(Move *move_, TMove useMove, int ruleToFire, int topWeight, int highest,
		  int distance, int movepos, int potend, int endpos, int moveid, int path, bool moveUseful, 
		  vector<int> possibleActiveMoves, vector<int> possibleStartMoves, Player &player){ //CHG-D-09
	//cout<<"test1"<<endl;

	move_->format(useMove, ruleToFire, topWeight, highest,
		   distance, movepos, potend, endpos, moveid,  path, moveUseful, 
		   possibleActiveMoves, possibleStartMoves, player);
}



// Rule-Based Artificial Intelligence
void PerformAIRuleTurn(Player &player)
{
	// Create a vector of rules that are of interest
	vector<TRule*> valid;
	TRule * curRule;
	Move *move_; //CHG-D-010
	move_=NULL;

	// Keep track of the last rule fired. We will need it later on to check if you were attacked to reduce the weight.
	static int lastRuleFired = -1;

	// Flag the rules that matched
	for (int i = 0; i < GameData()->Rules.size(); i++)
	{
		curRule = &(GameData()->Rules.at(i));

		if (curRule->last == GameData()->Moves.at(0) && curRule->secondLast == GameData()->Moves.at(1) && 
			curRule->thirdLast == GameData()->Moves.at(2))
			valid.push_back(&(GameData()->Rules.at(i)));
	}

	// Decide which rule to use based on the valid list and given weights
	int ruleToFire = -1;
	int topWeight = 0;
	int highest = 0;
	int distance = 0;
	int movepos = -1;
	int potend = -1;
	int endpos = 0;
	int moveid = 0;
	int path = 0; //left or right (0 and less or 1 or above)


	for (int i = 0; i < valid.size(); i++)
	{
		if (ruleToFire == -1)
		{
			ruleToFire = valid.at(i)->rulenum;
			topWeight = valid.at(i)->weight;
		}
		else if (valid.at(i)->weight > topWeight)
		{
			ruleToFire = valid.at(i)->rulenum;
			topWeight = valid.at(i)->weight;
		}
	}

	// Need to find the use in the move...
	bool moveUseful = 1; // assume the move was useful; tell us if it wasn't.
	TMove useMove = GameData()->Rules.at(ruleToFire).result;

	// Find active pieces on the board
	vector<int> possibleActiveMoves;

	for (int i = 0; i < MAX_GAME_POSITIONS; i++)
	{
		if (GameData()->board.IsPieceOnTop(player.piece, i))
			possibleActiveMoves.push_back(i);
	}

	vector<int> possibleStartMoves;

	for (int i = -1; i >= -GameData()->board.numstartstacks; i--)
	{
		if (GameData()->board.IsPieceOnTop(player.piece, i))
			possibleStartMoves.push_back(i);
	}

	if (possibleActiveMoves.empty())
	{
		switch (useMove)
		{
			case TM_ATTACK:
			case TM_FORWARD:
				moveUseful = 0;
				useMove = TM_START;
			break;
		}
	}

	if (possibleStartMoves.empty())
	{
		switch (useMove)
		{
			case TM_START:
				moveUseful = 0;
				useMove = TM_FORWARD;
			break;
		}
	}


	//setMove(useMove, move_);  
	//doIt(move_, useMove, ruleToFire, topWeight, highest, distance, movepos, potend, endpos, moveid, path,
	//				moveUseful, possibleActiveMoves, possibleStartMoves, player);
	//This is what is supposed to run, 
	//however the setMove function would not let me properly set the move variable, so it was done manually

	delete move_;
	// Actually perform the selected move
	switch (useMove) //CHG-D-07
	{
		case TM_ATTACK:
			move_ = new Attack(useMove);

			doIt(move_, useMove, ruleToFire, topWeight, highest, distance, movepos, potend, endpos, moveid, path,
					moveUseful, possibleActiveMoves, possibleStartMoves, player);
			break;

		case TM_FORWARD:

			move_ = new Forward(useMove);
			doIt(move_, useMove, ruleToFire, topWeight, highest, distance, movepos, potend, endpos, moveid, path,
					moveUseful, possibleActiveMoves, possibleStartMoves, player);
			break;

		case TM_START:
			move_ = new Start(useMove);
			doIt(move_, useMove, ruleToFire, topWeight, highest, distance, movepos, potend, endpos, moveid, path,
					moveUseful, possibleActiveMoves, possibleStartMoves, player);
			break;

		default:
			// Do nothing; your move was useless
			moveUseful = 0;
		break;
	}

	// Increase the weight of the selected rule
	if (moveUseful)
		GameData()->Rules.at(ruleToFire).weight++; // was useful
	else
		GameData()->Rules.at(ruleToFire).weight--; // was not useful
}

// State-Based Artificial Intelligence
void PerformAIStateTurn(Player &player, BaseState* &state)
{
	//if the state is not yet set, start out at movePieceState
	if (GameData()->states.at(GameData()->currentPlayer) == NULL){
		GameData()->states.at(GameData()->currentPlayer) = (BaseState*)(new movePieceState(GameData()->players.at(GameData()->currentPlayer).piece));
	}
	//else{
		//run the move piece code that pertains to the current state of this AI
		state->movePiece();
	//}
}