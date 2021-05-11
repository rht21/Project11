#pragma once
#include <string>
#include <algorithm>
#include <fstream>
#include "TokenEnum.h"
using namespace std;
class Token
{
	type token_type;
	string value;
	public:
		Token(string val);
		type getTokenType();
		string getVal();
		string to_string();
};

