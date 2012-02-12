#include<iostream>
#include<string>
#include "foundation.h"


string BaseState::GetStateName(void)
{
 return stateName_;
}

exitingPieceState::exitingPieceState(/*BaseState* state*/)
{
	/* populate valid moves list

 this->stack_ = state->GetStack();
 this->ai_ = state->GetAI();*/
	validMoves_.push_back(2);
	validMoves_.push_back(5);
	stateName_ = "exitingPiece";
}

void exitingPieceState::movePiece()
{
 //move a piece, this funct may not be needed as it's not being over written
	printf("I moved a piece! (not really)\n");
}

void exitingPieceState::StateChangeCheck(int source, int destination)
{
	printf("I'm checking stack %d and %d\n", source, destination);
/*condition for moving to the next state to be implemented
 if (stack_ > STUFF_WHICH_IDK)
 {
   ai_->SetState(reinterpret_cast<BaseState*>(new useTrampState(this)));
   delete this;
   return;
 }
 */
}

