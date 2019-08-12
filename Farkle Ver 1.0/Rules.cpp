#include "stdafx.h"
#include "Rules.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

string fileName;
string fileContent;
ifstream readFile;

Rules::Rules()
{
}


Rules::~Rules()
{
}

void Rules::FarkleRules()
{
	fileName = "FarkleRules.txt"; 
	readFile.open(fileName); // opens FarkleRules.txt
	
	while (getline(readFile, fileContent)) // reads file and prints out each line of text to user
	{
		cout << fileContent << endl;
	}
}