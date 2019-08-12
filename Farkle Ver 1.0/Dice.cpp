#include "stdafx.h"
#include "Dice.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int dice[6];
string rerollAnswer;
bool rollAgain[6] = { false };

Dice::Dice()
{
}


Dice::~Dice()
{
}

void Dice::DiceRoll() // initial dice roll for each turn, uses random to roll between 1 and 6
					  // and populates an array	
{
	srand((unsigned)time(0));

	for (int i = 0; i < 6; i++)
	{
		dice[i] = (rand() % 6 + 1);
	}
}

void Dice::DiceResults() // prints out array of rolled dice to user
{
	for (int i = 0; i < 6; i++)
	{
		{
			cout << dice[i] << "   ";
		}
	}
}

void Dice::DiceReroll() // requests to user which dice to keep by position and rerolls the rest
{
	for (int j = 0; j < 6; j++)
	{
		ROLL_ERROR:
		cout << "Position " << j + 1 << ": ";
		cin >> rerollAnswer;

		if (rerollAnswer == "y" || rerollAnswer == "Y")
		{
			rollAgain[j] = true;
		}

		else if (rerollAnswer == "n" || rerollAnswer == "N")
		{
			rollAgain[j] = false;
		}

		else
		{
			cout << "Please enter Y or N, y or n is OK too" << endl << endl;
			goto ROLL_ERROR;
		}

	}

	for (int i = 0; i < 6; i++)
	{
		if (rollAgain[i] == 1)
		{
			dice[i] = (rand() % 6 + 1);
		}
	}
}