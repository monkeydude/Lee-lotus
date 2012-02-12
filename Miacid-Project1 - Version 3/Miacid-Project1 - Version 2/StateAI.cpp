#include<iostream>
#include<string>
#include "foundation.h"


string BaseState::GetStateName(void){
	return stateName_;
}

//checks if a move exists within a vector
boolean BaseState::checkPos(vector<int> vec, int pos){
	for each (int a in vec){
		if (a == pos){
			return true;
		}
	}
	return false;
}

//returns the state of the piece on the top of this stack
int BaseState::isState(int pos){
	int start = pos;
	int dest;

	//if a piece is in the start state, set it to position -1 so it doesn't have a larger negative number
	if (start < 0){
		start = -1;
	}

	//if the start position is greater than 2, assign destination normally
	if (pos > 2){
		dest = GameData()->board.GetSizeOfStack(pos)+start;
	}
	//otherwise account for the gap of numbered spaces on the left hand starting track
	else{
		dest = GameData()->board.GetSizeOfStack(pos)+start;
		if (dest > 2){
			dest = dest + 3;
		}
	}

	if (dest >= 17){
		return 1;
	}
	else if ((dest == 10) && (GameData()->board.GetSizeOfStack(pos) >= 2)){
		return 2;
	}
	else if ((dest < 9) && ((GameData()->board.GetSizeOfStack(dest) + dest) == 10)){
		return 3;
	}
	else if (GameData()->board.GetSizeOfStack(dest) >= 2){
		return 4;
	}
	else{
		return 5;
	}
}

//find the new current state
int BaseState::findState(PIECE playnum){
	//start the new state in the worst possible state
	int optstate = 6;
	//check every piece on the board
	for (int a=-12; a<17; a++){
		//if the piece belongs to this AI
		//if all of the AIs pieces are covered, this if will never be true and it will stay in state 6
		if (GameData()->board.IsPieceOnTop(playnum, a)){
			//if that piece satisfies a better state than the current one, then move up to that state
			if (isState(a) < optstate){
				optstate = isState(a);
			}
		}
	}
	return optstate;
}


exitingPieceState::exitingPieceState(PIECE playnum){

	playernum = playnum;

	//populate valid moves list
	for (int a=-12; a<17; a++){
		if (GameData()->board.IsPieceOnTop(playernum, a)){
			if (isState(a) == 1)
				validMoves_.push_back(a);
		}
	}

	stateName_ = "exitingPiece";
}

void exitingPieceState::movePiece(){
	int toMove;
	//sort vector to have piece closest to the end at its tail
	std::sort(validMoves_.begin(), validMoves_.end());
	//get a piece from the vector list
	toMove = validMoves_.back();
	//remove last element
	validMoves_.pop_back();

	GameData()->board.MovePiece(toMove, -1);
}


boolean exitingPieceState::StateChangeCheck(int source, int destination){

	//if the new top piece on the source belongs to this AI and it was not in the list of potential moves
	//then check if it should be added
	if ((GameData()->board.IsPieceOnTop(playernum, source)) && !checkPos(validMoves_, source)){
		//if the piece can move off the board then add it to the vector
		if (isState(source) == 1)
			validMoves_.push_back(source);
	}

	//if the new top piece on the source does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, source)) && checkPos(validMoves_, source)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == source){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	//if the new top piece on the destination belongs to this AI and it was not in the list of potential moves
	//then check if it should be added
	if ((GameData()->board.IsPieceOnTop(playernum, destination)) && !checkPos(validMoves_, destination)){
		//if the piece can move off the board then add it to the vector
		if (isState(source) == 1)
			validMoves_.push_back(destination);
	}

	//if the new top piece on the destination does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, destination)) && checkPos(validMoves_, destination)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == destination){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	if (validMoves_.empty()){
		return true;
	}

	return false;
}


useTrampState::useTrampState(PIECE playnum){

	playernum = playnum;

	//populate valid moves list
	for (int a=-12; a<17; a++){
		if (GameData()->board.IsPieceOnTop(playernum, a)){
			if (isState(a) == 2)
				validMoves_.push_back(a);
		}
	}

	stateName_ = "useTrampPiece";
}

void useTrampState::movePiece(){
	int toMove;
	//sort vector to have piece whith lowest value at its tail
	std::sort(validMoves_.begin(), validMoves_.end(),greater<int>());
	//get a piece from the vector list
	toMove = validMoves_.back();
	//remove last element
	validMoves_.pop_back();

	//tells to move piece
	GameData()->board.MovePiece(toMove, -1);
}

boolean useTrampState::StateChangeCheck(int source, int destination){

	//if the new top piece on the source belongs to this AI
	if (GameData()->board.IsPieceOnTop(playernum, source)){
		//if the move is better than the current state, invalidate this state
		if (isState(source) < 2){
			return true;
		}
		//otherwise, if it was not in the list of potential moves
		else if (!checkPos(validMoves_, source)){
			//if the piece satisfies this state then add it to the vector
			if (isState(source) == 2){
				validMoves_.push_back(source);
			}
		}
	}

	//if the new top piece on the source does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, source)) && checkPos(validMoves_, source)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == source){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	//if the new top piece on the destination belongs to this AI
	if (GameData()->board.IsPieceOnTop(playernum, destination)){
		//if the move is better than the current state, invalidate this state
		if (isState(destination) < 2){
			return true;
		}
		//otherwise, if it was not in the list of potential moves
		else if (!checkPos(validMoves_, destination)){
			//if the piece satisfies this state then add it to the vector
			if (isState(destination) == 2){
				validMoves_.push_back(destination);
			}
		}
	}

	//if the new top piece on the destination does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, destination)) && checkPos(validMoves_, destination)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == destination){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	//if the list of available moves is empty, invalidate the state
	if (validMoves_.empty()){
		return true;
	}

	return false;
}

captureTrampState::captureTrampState(PIECE playnum){

	playernum = playnum;

	//populate valid moves list
	for (int a=-12; a<17; a++){
		if (GameData()->board.IsPieceOnTop(playernum, a)){
			if (isState(a) == 3)
				validMoves_.push_back(a);
		}
	}

	stateName_ = "captureTrampPiece";
}

void captureTrampState::movePiece(){
	int toMove;

	//get a piece from the vector list
	toMove = validMoves_.back();
	//remove last element
	validMoves_.pop_back();

	//tells to move piece
	GameData()->board.MovePiece(toMove, -1);
}

boolean captureTrampState::StateChangeCheck(int source, int destination){

	//if the new top piece on the source belongs to this AI
	if (GameData()->board.IsPieceOnTop(playernum, source)){
		//if the move is better than the current state, invalidate this state
		if (isState(source) < 3){
			return true;
		}
		//otherwise, if it was not in the list of potential moves
		else if (!checkPos(validMoves_, source)){
			//if the piece satisfies this state then add it to the vector
			if (isState(source) == 3){
				validMoves_.push_back(source);
			}
		}
	}

	//if the new top piece on the source does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, source)) && checkPos(validMoves_, source)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == source){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	//if the new top piece on the destination belongs to this AI
	if (GameData()->board.IsPieceOnTop(playernum, destination)){
		//if the move is better than the current state, invalidate this state
		if (isState(destination) < 3){
			return true;
		}
		//otherwise, if it was not in the list of potential moves
		else if (!checkPos(validMoves_, destination)){
			//if the piece satisfies this state then add it to the vector
			if (isState(destination) == 3){
				validMoves_.push_back(destination);
			}
		}
	}

	//if the new top piece on the destination does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, destination)) && checkPos(validMoves_, destination)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == destination){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	//if the list of available moves is empty, invalidate the state
	if (validMoves_.empty()){
		return true;
	}

	return false;
}


captureStackState::captureStackState(PIECE playnum){

	playernum = playnum;

	//populate valid moves list
	for (int a=-12; a<17; a++){
		if (GameData()->board.IsPieceOnTop(playernum, a)){
			if (isState(a) == 4)
				validMoves_.push_back(a);
		}
	}

	stateName_ = "captureStackPiece";
}

void captureStackState::movePiece(){
	int tempEnd=-1;
	int captureValue=0;

	int toErase=0;

	int finalLoc = validMoves_.front();
	int finalEnd = -1;
	int finalCap = 0;

	for (int x = 0; x<validMoves_.size();x++){

		if(validMoves_[x]<0){//if it's in starting area  i need to figure out it's best capture location (L|R)
					 //and potential capture stack size

			switch (GameData()->board.GetSizeOfStack(validMoves_[x])){
				case 1:
					if(GameData()->board.GetSizeOfStack(0)<GameData()->board.GetSizeOfStack(3)){
							tempEnd=3;
							captureValue=GameData()->board.GetSizeOfStack(3);
					}
						else{
							tempEnd=0;
							captureValue=GameData()->board.GetSizeOfStack(0);
						}
					break;

				case 2:
					if(GameData()->board.GetSizeOfStack(1)<GameData()->board.GetSizeOfStack(4)){
							tempEnd=4;
							captureValue=GameData()->board.GetSizeOfStack(4);
					}
						else{
							tempEnd=1;
							captureValue=GameData()->board.GetSizeOfStack(1);				
						}
					break;

				case 3:
					if(GameData()->board.GetSizeOfStack(2)<GameData()->board.GetSizeOfStack(5)){
							tempEnd=5;
							captureValue=GameData()->board.GetSizeOfStack(5);
					}
						else{
							tempEnd=2;
							captureValue=GameData()->board.GetSizeOfStack(2);				
						}
					break;

				case 4:
					tempEnd=6;
					captureValue=GameData()->board.GetSizeOfStack(6);
					break;

				default: cout << "I BROKE: captureStackState" <<endl;
					break;
			}

		}

		else{
		// itmust be a piece on the board, aka no L|R checking
			tempEnd=-1;
			captureValue=GameData()->board.GetSizeOfStack(validMoves_[x]);
		}

		//now to check vs previous vector
		if (captureValue>finalCap){
			finalCap=captureValue;
			finalEnd=tempEnd;
			finalLoc=validMoves_[x];
			toErase=x;
		}

	}
	//add it to the board
	GameData()->board.MovePiece(finalLoc, finalEnd);
	//remove the stack from possible choices
	validMoves_.erase(validMoves_.begin()+toErase);
}

boolean captureStackState::StateChangeCheck(int source, int destination){

	//if the new top piece on the source belongs to this AI
	if (GameData()->board.IsPieceOnTop(playernum, source)){
		//if the move is better than the current state, invalidate this state
		if (isState(source) < 4){
			return true;
		}
		//otherwise, if it was not in the list of potential moves
		else if (!checkPos(validMoves_, source)){
			//if the piece satisfies this state then add it to the vector
			if (isState(source) == 4){
				validMoves_.push_back(source);
			}
		}
	}

	//if the new top piece on the source does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, source)) && checkPos(validMoves_, source)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == source){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	//if the new top piece on the destination belongs to this AI
	if (GameData()->board.IsPieceOnTop(playernum, destination)){
		//if the move is better than the current state, invalidate this state
		if (isState(destination) < 4){
			return true;
		}
		//otherwise, if it was not in the list of potential moves
		else if (!checkPos(validMoves_, destination)){
			//if the piece satisfies this state then add it to the vector
			if (isState(destination) == 4){
				validMoves_.push_back(destination);
			}
		}
	}

	//if the new top piece on the destination does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, destination)) && checkPos(validMoves_, destination)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == destination){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	//if the list of available moves is empty, invalidate the state
	if (validMoves_.empty()){
		return true;
	}

	return false;
}


movePieceState::movePieceState(PIECE playnum){

	playernum = playnum;

	//populate valid moves list
	for (int a=-12; a<17; a++){
		if (GameData()->board.IsPieceOnTop(playernum, a)){
			if (isState(a) == 5)
				validMoves_.push_back(a);
		}
	}

	stateName_ = "movePiece";
}

void movePieceState::movePiece(){
	int toMove;

	//get a piece from the vector list
	toMove = validMoves_.back();
	//remove last element
	validMoves_.pop_back();

	//piece is in starting position
	if(toMove<0){

		switch (GameData()->board.GetSizeOfStack(toMove)){
			case 1:
				if(GameData()->board.GetSizeOfStack(0)<GameData()->board.GetSizeOfStack(3)){
						GameData()->board.MovePiece(toMove, 3); //move to right
				}
					else{
						GameData()->board.MovePiece(toMove, 0);	//move to left				
					}
				return;

			case 2:
				if(GameData()->board.GetSizeOfStack(1)<GameData()->board.GetSizeOfStack(4)){
						GameData()->board.MovePiece(toMove, 4); //move to right
				}
					else{
						GameData()->board.MovePiece(toMove, 1);	//move to left				
					}
				return;

			case 3:
				if(GameData()->board.GetSizeOfStack(2)<GameData()->board.GetSizeOfStack(5)){
						GameData()->board.MovePiece(toMove, 5); //move to right
				}
					else{
						GameData()->board.MovePiece(toMove, 2);	//move to left				
					}
				return;

			case 4:
				GameData()->board.MovePiece(toMove, 6);
				return;

			default: cout << "I BROKE" <<endl;
				break;
		}

	}

	GameData()->board.MovePiece(toMove, -1);
}

boolean movePieceState::StateChangeCheck(int source, int destination){

	//if the new top piece on the source belongs to this AI
	if (GameData()->board.IsPieceOnTop(playernum, source)){
		//if the move is better than the current state, invalidate this state
		if (isState(source) < 5){
			return true;
		}
		//otherwise, if it was not in the list of potential moves
		else if (!checkPos(validMoves_, source)){
			//if the piece satisfies this state then add it to the vector
			if (isState(source) == 5){
				validMoves_.push_back(source);
			}
		}
	}

	//if the new top piece on the source does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, source)) && checkPos(validMoves_, source)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == source){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	//if the new top piece on the destination belongs to this AI
	if (GameData()->board.IsPieceOnTop(playernum, destination)){
		//if the move is better than the current state, invalidate this state
		if (isState(destination) < 5){
			return true;
		}
		//otherwise, if it was not in the list of potential moves
		else if (!checkPos(validMoves_, destination)){
			//if the piece satisfies this state then add it to the vector
			if (isState(destination) == 5){
				validMoves_.push_back(destination);
			}
		}
	}

	//if the new top piece on the destination does not belong to this AI and it was in the list of potential moves
	//then remove it from the list
	if (!(GameData()->board.IsPieceOnTop(playernum, destination)) && checkPos(validMoves_, destination)){
		for (int a=0; a<validMoves_.size(); a++){
			if (validMoves_.at(a) == destination){
				validMoves_.erase(validMoves_.begin()+a);
			}
		}
	}

	//if the list of available moves is empty, invalidate the state
	if (validMoves_.empty()){
		return true;
	}

	return false;
}

//population of the list of moves is not needed here
//becuase it is subject to too much change, it will be done only when the AI is about to move
cantMoveState::cantMoveState(PIECE playnum){

	playernum = playnum;
	stateName_ = "cantMovePiece";
}

void cantMoveState::movePiece(){
	deepest = 0;
	int pos;

	//find the stack in which your piece is deepest
	for (int a=-12; a<17; a++){
		if (GameData()->board.GetDeepestPiece(playernum, a) > deepest){
			pos = GameData()->board.GetDeepestPiece(playernum, a);
		}
	}

	validMoves_.clear();
	validMoves_.push_back(pos);

	int toMove;

	//get a piece from the vector list
	toMove = validMoves_.back();
	//remove last element
	validMoves_.pop_back();

	//tells to move piece
	GameData()->board.MovePiece(toMove, -1);
}

//no need to check if the state can get worse (it can't)
//no need to maintain the list of potential moves, it is calculated just before the move is actually made
//only check itf the state gets better
boolean cantMoveState::StateChangeCheck(int source, int destination){

	//if the new top piece on the source belongs to this AI then invalidate the state (it is automatically a better move)
	if (GameData()->board.IsPieceOnTop(playernum, source)){
			return true;
	}

	//if the new top piece on the destination belongs to this AI then invalidate the state (it is automatically a better move)
	if (GameData()->board.IsPieceOnTop(playernum, destination)){
			return true;
	}

	return false;
}