#pragma once
class GameLoop
{
public:
	GameLoop();
	~GameLoop();

	void GameStart();
	void PlayerTurn();
	void GetPlayers();
	int FirstScore();
	int Score();
	void FinalTurn();
	void ShowScores();
	void GameEnd();

};

