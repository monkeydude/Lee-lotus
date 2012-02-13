#ifndef STATEAI_H
#define STATEAI_H

#include "foundation.h"
using namespace std;

//example and sources at http://advancedcppwithexamples.blogspot.com/2010/11/c-example-of-state-design-pattern.html
 
// The 'BaseState' abstract class for the stateAI
class BaseState{
public:
  string GetStateName(void);
  void printValidMoves(void);

  //these will occur in the concrete states
  virtual void movePiece(void) = 0;
  virtual boolean StateChangeCheck(int, int) = 0;
  int findState(PIECE);

protected:
  vector<int> validMoves_;
  PIECE playernum;
  string stateName_;
  boolean stateThreeCheck(int, int);
  boolean checkPos(vector<int>, int);
  int isState(int);
};
 
// A 'ConcreteState' class
// The AI has a piece that can exit this turn
class exitingPieceState : BaseState{
public:
  exitingPieceState(PIECE);
  void movePiece();
  boolean StateChangeCheck(int, int);
};

// A 'ConcreteState' class
// The AI can use the trampoline space
class useTrampState : BaseState{
public:
  useTrampState(PIECE);
  void movePiece();
  boolean StateChangeCheck(int, int);
};

// A 'ConcreteState' class
// The AI can capture a stack so that its next move allows it to use the trampoline
class captureTrampState : BaseState{
public:
  captureTrampState(PIECE);
  void movePiece();
  boolean StateChangeCheck(int, int);
};

// A 'ConcreteState' class
// The AI has a piece that can exit this turn
class captureStackState : BaseState{
public:
  captureStackState(PIECE);
  void movePiece();
  boolean StateChangeCheck(int, int);
};

// A 'ConcreteState' class
// The AI only has pieces which don't fit into higher priority states.
class movePieceState : BaseState{
public:
  movePieceState(PIECE);
  void movePiece();
  boolean StateChangeCheck(int, int);//two ints
};

// A 'ConcreteState' class
// The AI has no available pieces of its own to move
class cantMoveState : BaseState
{
public:
  cantMoveState(PIECE);
  void movePiece();
  boolean StateChangeCheck(int, int);//two ints
private:
	int deepest;
};
 
#endif