#include "stdafx.h"
#include "GameLoop.h"
#include "Dice.h"
#include "Rules.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// global variables
int numPlayers, initialScore = 0, scoreHolder, maxScore = 0;
string playerName, rulesAnswer;
vector<string> players; // stores player names
vector<int> playerScore;  // stores player scores
vector<bool> enteredGame; // determine if a player has scored 1000 points on first turn and entered game
vector<bool> wentOut; // false for each player until they are the first to reach 10000
vector<bool> winningPlayer; // becomes true for the player with the highest score after final round
Dice playerDice;


GameLoop::GameLoop()
{
}


GameLoop::~GameLoop()
{
}

void GameLoop::GameStart() // game introduction and option to display game rules
{
	cout << "Welcome to Farkle!!\nCoded By: Joshua Poitras" << endl;
	cout << endl << endl;
	RULES_ERROR:
	cout << "Would you like to read the rules before you play? (Y/N) " << endl;
	cin >> rulesAnswer;
		
	if (rulesAnswer == "y" || rulesAnswer == "Y")
	{
		Rules rules;
		rules.FarkleRules();
		system("PAUSE");
	}
	
	else if (rulesAnswer == "n" || rulesAnswer == "N")
	{
		cout << "So, you have played before. OK then, let's get started" << endl;
	}
	
	else
	{
		cout << "ERROR ----- Please enter only Y or N, y or n is also OK" << endl << endl;
		system("PAUSE");
		cout << endl << endl;
		goto RULES_ERROR;
	}
	
	 
}

void GameLoop::GetPlayers() // requests number of players and gets name input
{
	while (numPlayers < 2)
	{
		cout << "How many players will be playing? (Minimum of 2): ";
		cin >> numPlayers;
		if (numPlayers < 2)
		{
			cout << "Please enter at least 2 or more" << endl;
		}
	}
		
	for (int i = 0; i < numPlayers; i++)
	{
		cout << "Enter Player " << i + 1 << " name: ";
		cin >> playerName;
		
		// this is where the established vectors become populated
		players.push_back(playerName);
		playerScore.push_back(initialScore);
		enteredGame.push_back(false);
		winningPlayer.push_back(false);
		wentOut.push_back(false);
	}
}

void GameLoop::PlayerTurn() // player turn sequence, continues to run until a player reaches 10000 points
{
	RESTART_HERE:
	for (int i = 0; i < numPlayers; i++)
	{
		scoreHolder = 0;
		cout << "=============================================" << endl;
		cout << players[i] << "'s Turn" << endl;
		cout << players[i] << "'s Overall Score: " << playerScore[i] << endl;
		cout << "Get ready to roll!!" << endl;
		system("PAUSE");
		playerDice.DiceRoll();
		cout << endl;
		playerDice.DiceResults();
		cout << endl;
		system("PAUSE");
		
		// branch statement determines if a player is in the game or not and routes accordingly
		if (enteredGame[i] == 0)
		{
			playerScore[i] = FirstScore();
			if (playerScore[i] >= 1000)
			{
				enteredGame[i] = true;
			}
		}
		else if (enteredGame[i] == 1)
		{
			scoreHolder = Score();
			playerScore[i] += scoreHolder;
		}
		cout << players[i] << "'s Overall Score: " << playerScore[i] << endl << endl;
		system("PAUSE");
		ShowScores();
		if (playerScore[i] >= 10000) 
		{
			wentOut[i] = true;
			break;
		}

		if (i == numPlayers - 1) // this allows the player turn loop to reset and continue play 
		{
			goto RESTART_HERE;
		}

		
	}
}

int GameLoop::FirstScore() // scores the dice rolls for turns where a player has yet to enter the game
{
	int turnScore = 0, onesCounter = 0, twosCounter = 0, threesCounter = 0, foursCounter = 0, fivesCounter = 0, sixesCounter = 0, nonScore = 0;
	string playerContinue;
	
	for (int i = 0; i < 6; i++)
	{
		switch (playerDice.dice[i])
		{
		case 1:
			turnScore += 100;
			onesCounter++;
			if (onesCounter == 3)
			{
				turnScore = (turnScore - 300) + 1000;
				onesCounter = 0; // each die possiblility gets counted to check for 3 of a kind
				nonScore -= 3;  // nonScore keeps track of non scoring dice for FARKLEs
			}
			break;

		case 2:
			twosCounter++;
			nonScore++;
			if (twosCounter == 3)
			{
				turnScore += 200;
				twosCounter = 0;
				nonScore -= 3;
			}
			break;

		case 3:
			threesCounter++;
			nonScore++;
			if (threesCounter == 3)
			{
				turnScore += 300;
				threesCounter = 0;
				nonScore -= 3;
			}
			break;

		case 4:
			foursCounter++;
			nonScore++;
			if (foursCounter == 3)
			{
				turnScore += 400;
				foursCounter = 0;
				nonScore -= 3;
			}
			break;

		case 5:
			fivesCounter++;
			turnScore += 50;
			if (fivesCounter == 3)
			{
				turnScore = (turnScore - 150) + 500;
				fivesCounter = 0;
				nonScore -= 3;
			}
			break;

		case 6:
			sixesCounter++;
			nonScore++;
			if (sixesCounter == 3)
			{
				turnScore += 600;
				sixesCounter = 0;
				nonScore -= 3;
			}
			break;
		}

		
	}
	
	if (nonScore == 6) // if all 6 dice are non scoring then it triggers this
	{
		cout << "FARKLE!! No scoring dice, turn over" << endl;
		turnScore = 0;
	}

		
	cout << "Current turn score: " << turnScore << endl;
	if (turnScore < 1000 && nonScore != 6) // this loops for subsequent rolls until 1000 points reached or a FARKLE
	{
		onesCounter = 0, twosCounter = 0, threesCounter = 0, foursCounter = 0, fivesCounter = 0, sixesCounter = 0, nonScore = 0;
		cout << "Score is less than 1000, choose dice to roll again" << endl;
		cout << "Please type Y to reroll, or N to keep for each position" << endl;
		system("PAUSE");
		cout << endl << endl;
		
		while (turnScore < 1000 && nonScore != 6)
		{
			playerDice.DiceReroll();
			playerDice.DiceResults();
			turnScore = 0;
			for (int i = 0; i < 6; i++)
			{	// the if part of this branch is scoring dice that were held so that accurate total turn score can be tracked
				if (playerDice.rollAgain[i] == 0)
				{
					nonScore++;
					switch (playerDice.dice[i])
					{
					case 1:
						turnScore += 100;
						onesCounter++;
						if (onesCounter == 3)
						{
							turnScore = (turnScore - 300) + 1000;
							onesCounter = 0;
							nonScore -= 3;
						}
						break;

					case 2:
						twosCounter++;
						if (twosCounter == 3)
						{
							turnScore += 200;
							twosCounter = 0;
							nonScore -= 3;
						}
						break;

					case 3:
						threesCounter++;
						if (threesCounter == 3)
						{
							turnScore += 300;
							threesCounter = 0;
							nonScore -= 3;
						}
						break;

					case 4:
						foursCounter++;
						if (foursCounter == 3)
						{
							turnScore += 400;
							foursCounter = 0;
							nonScore -= 3;
						}
						break;

					case 5:
						turnScore += 50;
						fivesCounter++;
						if (fivesCounter == 3)
						{
							turnScore = (turnScore - 150) + 500;
							fivesCounter = 0;
							nonScore -= 3;
						}
						break;

					case 6:
						sixesCounter++;
						if (sixesCounter == 3)
						{
							turnScore += 600;
							sixesCounter = 0;
							nonScore -= 3;
						}
						break;

					}
				}
				
				else
				{
					switch (playerDice.dice[i])
					{
					case 1:
						turnScore += 100;
						onesCounter++;
						if (onesCounter == 3)
						{
							turnScore = (turnScore - 300) + 1000;
							onesCounter = 0;
							nonScore -= 3;
						}
						break;

					case 2:
						twosCounter++;
						nonScore++;
						if (twosCounter == 3)
						{
							turnScore += 200;
							twosCounter = 0;
							nonScore -= 3;
						}
						break;

					case 3:
						threesCounter++;
						nonScore++;
						if (threesCounter == 3)
						{
							turnScore += 300;
							threesCounter = 0;
							nonScore -= 3;
						}
						break;

					case 4:
						foursCounter++;
						nonScore++;
						if (foursCounter == 3)
						{
							turnScore += 400;
							foursCounter = 0;
							nonScore -= 3;
						}
						break;

					case 5:
						fivesCounter++;
						turnScore += 50;
						if (fivesCounter == 3)
						{
							turnScore = (turnScore - 150) + 500;
							fivesCounter = 0;
							nonScore -= 3;
						}
						break;

					case 6:
						sixesCounter++;
						nonScore++;
						if (sixesCounter == 3)
						{
							turnScore += 600;
							sixesCounter = 0;
							nonScore -= 3;
						}
						break;
					}
				}
				
			}
			
			if (nonScore == 6)
			{
				cout << "FARKLE!! No scoring dice, turn over" << endl;
				turnScore = 0;
				break;
			}

			
			onesCounter = 0, twosCounter = 0, threesCounter = 0, foursCounter = 0, fivesCounter = 0, sixesCounter = 0, nonScore = 0;
			cout << "Current turn score: " << turnScore << endl;
			if (turnScore < 1000)
			{
				cout << "Score is less than 1000, choose dice to roll again" << endl;
				cout << "Please type Y to reroll, or N to keep for each position" << endl;
				system("PAUSE");
				cout << endl << endl;
			}
			
		}
	}
	
	return turnScore;
}

int GameLoop::Score() // handles scoring for all turns after entering the game 
{
	ALL_SCORE:
	int turnScore = 0, onesCounter = 0, twosCounter = 0, threesCounter = 0, foursCounter = 0, fivesCounter = 0, sixesCounter = 0, nonScore = 0;
	string playerContinue, rerollAnswer;
	int allDiceScore = 0;
	for (int i = 0; i < 6; i++)
	{
		switch (playerDice.dice[i])
		{
		case 1:
			turnScore += 100;
			onesCounter++;
			if (onesCounter == 3)
			{
				turnScore = (turnScore - 300) + 1000;
				onesCounter = 0;
				nonScore -= 3;
			}
			break;

		case 2:
			twosCounter++;
			nonScore++;
			if (twosCounter == 3)
			{
				turnScore += 200;
				twosCounter = 0;
				nonScore -= 3;
			}
			break;

		case 3:
			threesCounter++;
			nonScore++;
			if (threesCounter == 3)
			{
				turnScore += 300;
				threesCounter = 0;
				nonScore -= 3;
			}
			break;

		case 4:
			foursCounter++;
			nonScore++;
			if (foursCounter == 3)
			{
				turnScore += 400;
				foursCounter = 0;
				nonScore -= 3;
			}
			break;

		case 5:
			fivesCounter++;
			turnScore += 50;
			if (fivesCounter == 3)
			{
				turnScore = (turnScore - 150) + 500;
				fivesCounter = 0;
				nonScore -= 3;
			}
			break;

		case 6:
			sixesCounter++;
			nonScore++;
			if (sixesCounter == 3)
			{
				turnScore += 600;
				sixesCounter = 0;
				nonScore -= 3;

			}
			break;
		}
	}

	if (nonScore == 6)
	{
		cout << "FARKLE!! No scoring dice, turn over" << endl;
		turnScore = 0;
		goto FARKLE_END;
	}

	if (nonScore == 0)
	{
		cout << "You have rolled all scoring dice!! This score gets banked and you get to roll again!!" << endl;
		system("PAUSE");
		cout << endl << endl;
		allDiceScore = turnScore;
		playerDice.DiceRoll();
		cout << endl;
		playerDice.DiceResults();
		cout << endl;
		system("PAUSE");
		goto ALL_SCORE;
	}

	onesCounter = 0, twosCounter = 0, threesCounter = 0, foursCounter = 0, fivesCounter = 0, sixesCounter = 0, nonScore = 0;
	cout << "Current turn score: " << turnScore << endl;
	
	while (playerContinue != "n" || playerContinue != "N")
	{
		CONTINUE_ERROR:
		cout << "Would you like to roll any dice again? (Y/N) " << endl;
		cin >> playerContinue;
		if (playerContinue == "n" || playerContinue == "N")
		{
			break;
		}

		else if (playerContinue == "y" || playerContinue == "Y")
		{
			cout << endl;
		}

		else
		{
			cout << "Please enter Y or N, y or n is OK too" << endl << endl;
			goto CONTINUE_ERROR;
		}

		playerDice.DiceReroll();
		playerDice.DiceResults();
		turnScore = 0;
		for (int i = 0; i < 6; i++)
		{
			if (playerDice.rollAgain[i] == 0)
			{
				nonScore++;
				switch (playerDice.dice[i])
				{
				case 1:
					turnScore += 100;
					onesCounter++;
					if (onesCounter == 3)
					{
						turnScore = (turnScore - 300) + 1000;
						onesCounter = 0;
						nonScore -= 3;
					}
					break;

				case 2:
					twosCounter++;
					if (twosCounter == 3)
					{
						turnScore += 200;
						twosCounter = 0;
						nonScore -= 3;
					}
					break;

				case 3:
					threesCounter++;
					if (threesCounter == 3)
					{
						turnScore += 300;
						threesCounter = 0;
						nonScore -= 3;
					}
					break;

				case 4:
					foursCounter++;
					if (foursCounter == 3)
					{
						turnScore += 400;
						foursCounter = 0;
						nonScore -= 3;
					}
					break;

				case 5:
					turnScore += 50;
					fivesCounter++;
					if (fivesCounter == 3)
					{
						turnScore = (turnScore - 150) + 500;
						fivesCounter = 0;
						nonScore -= 3;
					}
					break;

				case 6:
					sixesCounter++;
					if (sixesCounter == 3)
					{
						turnScore += 600;
						sixesCounter = 0;
						nonScore -= 3;
					}
					break;

				}
			}

			else
			{
				switch (playerDice.dice[i])
				{
				case 1:
					turnScore += 100;
					onesCounter++;
					if (onesCounter == 3)
					{
						turnScore = (turnScore - 300) + 1000;
						onesCounter = 0;
						nonScore -= 3;
					}
					break;

				case 2:
					twosCounter++;
					nonScore++;
					if (twosCounter == 3)
					{
						turnScore += 200;
						twosCounter = 0;
						nonScore -= 3;
					}
					break;

				case 3:
					threesCounter++;
					nonScore++;
					if (threesCounter == 3)
					{
						turnScore += 300;
						threesCounter = 0;
						nonScore -= 3;
					}
					break;

				case 4:
					foursCounter++;
					nonScore++;
					if (foursCounter == 3)
					{
						turnScore += 400;
						foursCounter = 0;
						nonScore -= 3;
					}
					break;

				case 5:
					fivesCounter++;
					turnScore += 50;
					if (fivesCounter == 3)
					{
						turnScore = (turnScore - 150) + 500;
						fivesCounter = 0;
						nonScore -= 3;
					}
					break;

				case 6:
					sixesCounter++;
					nonScore++;
					if (sixesCounter == 3)
					{
						turnScore += 600;
						sixesCounter = 0;
						nonScore -= 3;
					}
					break;
				}
			}
		}
		cout << "Current turn score: " << turnScore << endl;
		if (nonScore == 6 && twosCounter < 3 && threesCounter < 3 && foursCounter < 3 && sixesCounter < 3)
		{
			cout << "FARKLE!! No scoring dice, turn over" << endl;
			turnScore = 0;
			goto FARKLE_END;
		}
		onesCounter = 0, twosCounter = 0, threesCounter = 0, foursCounter = 0, fivesCounter = 0, sixesCounter = 0, nonScore = 0;
	}
	FARKLE_END:
	turnScore += allDiceScore;
	return turnScore;
}

void GameLoop::FinalTurn() // runs final round for everyone that didn't go out to get last chance to win
{
	cout << "==================================================" << endl;
	cout << "This is the FINAL round!!! Highest score will win!" << endl;
	cout << "==================================================" << endl;
	cout << endl << endl;
	
	
	for (int i = 0; i < numPlayers; i++)
	{
		if (wentOut[i] == 0)
		{
			scoreHolder = 0;
			cout << "=============================================" << endl;
			cout << players[i] << "'s Turn" << endl;
			cout << "Get ready to roll!!" << endl;
			system("PAUSE");
			playerDice.DiceRoll();
			cout << endl;
			playerDice.DiceResults();
			cout << endl;
			system("PAUSE");
			scoreHolder = Score();
			playerScore[i] += scoreHolder;
			cout << players[i] << "'s Overall Score: " << playerScore[i] << endl << endl;
			system("PAUSE");
		}
			 
	}

	for (int j = 0; j < numPlayers; j++) // this determines who has the highest score
	{
		if (playerScore[j] > maxScore)
		{
			maxScore = playerScore[j];
		}
	}

	for (int k = 0; k < numPlayers; k++) // marks winning player as true for game end method
	{
		if (playerScore[k] == maxScore)
		{
			winningPlayer[k] = true;
		}
	}

}

void GameLoop::ShowScores() // cycles through players and displays everyone's score
{
	cout << endl << endl << endl;
	cout << "==========================================" << endl;
	cout << "Current Scores: " << endl;
	cout << "==========================================" << endl;
	for (int j = 0; j < numPlayers; j++)
	{
		cout << players[j] << ":   " << playerScore[j] << endl;
	}
	system("PAUSE");
	cout << endl << endl;
}

void GameLoop::GameEnd() // shows final scores and presents winner
{
	for (int i = 0; i < numPlayers; i++)
	{
		if (winningPlayer[i] == 1)
		{
			ShowScores();
			cout << endl << endl;
			cout << "Congratulations to the winner, " << players[i] << ", with a final score of: " << playerScore[i] << endl << endl;
		}
	}
}
