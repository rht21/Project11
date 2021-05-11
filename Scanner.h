#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Token.h"
class Scanner
{
	string file = "";
	bool commentFlag = false;
	bool stringFlag = false;
	string nextTok();
	int pos = 0;
	int oldPos = 0;
	public:
		Scanner();
		Scanner(string filename);
		Token peek();
		Token peek2();
		Token next();
};

