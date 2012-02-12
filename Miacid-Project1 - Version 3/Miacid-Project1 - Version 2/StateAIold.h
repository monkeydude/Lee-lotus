#ifndef STATEAI_H
#define STATEAI_H

#include "foundation.h"
using namespace std;

//example and sources at http://advancedcppwithexamples.blogspot.com/2010/11/c-example-of-state-design-pattern.html
 
 // The 'BaseState' abstract class for the stateAI
class BaseState
{
public:

  string GetStateName(void);

  //these will occur in the concrete states
  virtual void movePiece(void) = 0;
  virtual void StateChangeCheck(int, int) = 0;

protected:
  vector<int> validMoves_;
  string stateName_;;
};
 
// A 'ConcreteState' class
// The AI has a piece that can exit this turn
class exitingPieceState : BaseState
{
public:
  exitingPieceState(/*BaseState* state*/);
  void movePiece();
  void StateChangeCheck(int, int);//two ints
 
private:
};

// A 'ConcreteState' class
// The AI can use the trampoline space
class useTrampState : BaseState
{
public:
  useTrampState(BaseState* state);
  void movePiece();
  void StateChangeCheck(int, int);//two ints
 
private:
};

// A 'ConcreteState' class
// The AI can capture a stack so that its next move allows it to use the trampoline
class captureTrampState : BaseState
{
public:
  captureTrampState(BaseState* state);
  void movePiece();
  void StateChangeCheck(int, int);//two ints
 
private:
};

// A 'ConcreteState' class
// The AI has a piece that can exit this turn
class captureStackState : BaseState
{
public:
  captureStackState(BaseState* state);
  void movePiece();
  void StateChangeCheck(int, int);//two ints
 
private:
};

// A 'ConcreteState' class
// The AI only has pieces which don't fit into higher priority states.
class movePieceState : BaseState
{
public:
  movePieceState(BaseState* state);
  void movePiece();
  void StateChangeCheck(int, int);//two ints
 
private:
};

// A 'ConcreteState' class
// The AI has no available pieces of its own to move
class cantMoveState : BaseState
{
public:
  cantMoveState(BaseState* state);
  void movePiece();
  void StateChangeCheck(int, int);//two ints
 
private:
};
 
#endif