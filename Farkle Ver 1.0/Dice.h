#pragma once
class Dice
{
private:

	

public:
	int dice[6];
	bool rollAgain[6] = { false };
	Dice();

	~Dice();

	void DiceRoll();
	void DiceResults();
	void DiceReroll();
};



