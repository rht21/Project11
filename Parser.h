#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Scanner.h"
#include "Token.h"
#include "symbolTable.h"
#include "writer.h"
using namespace std;
class Parser
{
	Scanner scanner;
	writer codeWriter;
	string className;
	void parseClass();
	void classVarDec();
	void subroutineDec();
	string parseType();
	void parameterList();
	void subroutineBody(string name, string type);
	void varDec();
	void statements();
	void letStatement();
	void ifStatement();
	void whileStatement();
	void returnStatement();
	void doStatement();
	void subroutineCall();
	void expression();
	int expressionList();
	void term();
	symbolTable symbols;
	int arguments = 0;
	int locals = 0;
	int statics = 0;
	int fields = 0;
	int whileCounter = 0;
	int ifCounter = 0;
	bool returnFlag = false;
	public:
		Parser(string inputName, string outputName);
		void run();
};

