// Electronic Farkle Version 1.0


#include "stdafx.h"
#include "GameLoop.h"
using namespace std;

GameLoop game;

int main()
{
	game.GameStart();
	game.GetPlayers();
	game.PlayerTurn();
	game.FinalTurn();
	game.GameEnd();
	return 0;
}
