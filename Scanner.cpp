#include "Scanner.h"
Scanner::Scanner(string filename) {
	try {
		ifstream stream(filename);
		string line;
		while (getline(stream, line)) {
			if (line.find("//") != string::npos) {
				line = line.substr(0, line.find("//"));
			}
			file += line;
			
		}
	}
	catch (exception e) {
		throw e;
	}
}
Scanner::Scanner() {
}

string Scanner::nextTok()
{
	string tok = "";
	while (true) {
		if (pos == file.size()) {
			break;
		}
		char curChar = file[pos];
		
		
		if (tok.compare("") != 0 && (curChar == '.' || curChar == '{' || curChar == '}' || curChar == '[' || curChar == ']' ||
			curChar == '(' || curChar == ')' || curChar == '+' || curChar == '-' || curChar == '*' || curChar == '=' ||
			curChar == '/' || curChar == '&' || curChar == '|' || curChar == ';' || curChar == '<' || curChar == '>' || curChar == '~' || curChar == ',') && !commentFlag && !stringFlag) break;
		pos++;
		if (tok.compare("") == 0 && (curChar == ' ' || curChar == '\t')) continue;
		if ((curChar == ' ' && !commentFlag && !stringFlag)) {
			break;
		}
		else if (commentFlag && curChar == '*' && file[pos] == '/') {
			pos++;
			commentFlag = false;
		}
		else if (commentFlag) {
		}
		else if (stringFlag && curChar == '"') {
			stringFlag = false;
			tok += curChar;
		}
		else if (curChar == '"') {
			stringFlag = true;
			tok += curChar;
		}
		else if (curChar == '/' && file[pos] == '*' && file[pos+1] == '*') {
			commentFlag = true;
		}
		else if (tok.compare("") == 0 && (curChar == '.' || curChar == '{' || curChar == '}' || curChar == '[' || curChar == ']' ||
			curChar == '(' || curChar == ')' || curChar == '+' || curChar == '-' || curChar == '*' || curChar == '=' ||
			curChar == '/' || curChar == '&' || curChar == '|' || curChar == ';' || curChar == '<' || curChar == '>' || curChar == '~' || curChar == ',')) {
			tok += curChar;
			break;
		}
		else {
			tok += curChar;
		}
	}
	return tok;
}

Token Scanner::peek()
{
	Token next(nextTok());
	pos = oldPos;
	return next;
}

Token Scanner::peek2()
{
	nextTok();
	Token next(nextTok());
	pos = oldPos;
	return next;
}


Token Scanner::next()
{
	Token next(nextTok());
	oldPos = pos;
	return next;
}
