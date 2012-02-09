#include "foundation.h"

void PerformHumanTurn(Player &player)
{
	// Variables
	Point2i click = Point2i(-1, -1); //invalid click
	int position1 = 0;
	int position2 = 0;
	PIECE curpiece = player.piece;

	// If the player has no possible moves, then they can move any piece OR SKIP THEIR TURN...
	if (!GameData()->board.PossibleMove(player.piece))
		curpiece = PIECE_BAD;

	// Loop until a valid move has been completed
	while (true)
	{
		// Get first valid position
		while (true)
		{
			// Reset last click
			GameData()->ResetLastClick();

			// Wait for a click to come through...
			do
			click = GameData()->GetLastClick();
			while (click == Point2i(-1, -1));

			// Check if click is on a piece
			if (GameData()->board.GetLocationFromXY(click.x, click.y, position1))
			{
				PIECE testpiece = GameData()->board.GetTopPiece(position1);

				// Must be a playable piece
				if (testpiece == PIECE_BAD)
					continue;

				// Move allowable pieces
				if (curpiece == PIECE_BAD || curpiece == testpiece)
					break;
			}
		}

		// Get second position
		while (true)
		{
			// Reset last click
			GameData()->ResetLastClick();

			// Wait for a click to come through...
			do
			click = GameData()->GetLastClick();
			while (click == Point2i(-1, -1));

			// Check if click is on a piece
			if (GameData()->board.GetLocationFromXY(click.x, click.y, position2))
			{
				if (position2 >= 0)
					break;
			}
		}

		// Attempt to move piece
		if (GameData()->board.MovePiece(position1, position2))
			break;
	}
}