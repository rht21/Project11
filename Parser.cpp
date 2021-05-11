#include "Parser.h"

Parser::Parser(string inputName, string outputName)
{
	Scanner s1(inputName);
	scanner = s1;
	writer codeWriter(outputName);
}

void Parser::run()
{
	parseClass();
	codeWriter.closeFile();
}

void Parser::parseClass()
{
	scanner.next(); // Class keyword
	className = scanner.next().getVal(); // className identifier
	scanner.next(); // { symbol
	vector<string> sub;
	// peek values and handle them until we hit a new token
	string nextType = scanner.peek().getVal();
	while (nextType != "}") {
		if (nextType == "static" || nextType == "field") {
			classVarDec();
		}
		else if (nextType == "constructor" || nextType == "method" || nextType == "function") {
			subroutineDec();
		}
		nextType = scanner.peek().getVal();
	}
}

void Parser::classVarDec()
{
	string segment = scanner.next().getVal();
	string type = parseType();
	vector<string> names;
	names.push_back(scanner.next().getVal());
	Token nextType = scanner.next();
	// loop printing tags until we hit a semicolon
	// these should all be identifiers and commas alternating
	while (nextType.getVal() != ";") {
		if(nextType.getTokenType() == type::IDENTIFIER){
			names.push_back(nextType.getVal());
		}
		nextType = scanner.next();
	}
	if(segment == "field"){
		segment = "this";
	}
	for(int i = 0; i < names.size(); i++){
		int num = 0;
		if(segment == "static"){
			num = statics;
			statics++;
		}
		else{
			num = fields;
			fields++;
		}
		symbols.put(names.at(i), type, segment, num);
	}
}

void Parser::subroutineDec()
{
	returnFlag = false;
	locals = 0;
	arguments = 0;
	ifCounter = 0;
	whileCounter = 0;
	string type = scanner.next().getVal();
	scanner.next().getVal(); //return type
	string name = scanner.next().getVal(); //name
	if (type == "method") {
		arguments++;
	}
	scanner.next(); // (
	parameterList();
	scanner.next(); // )
	subroutineBody(name, type);
	if (!returnFlag) {
		codeWriter.writePush("constant", 0);
		codeWriter.writeReturn();
	}
}

string Parser::parseType(){
	return scanner.next().getVal();
}

void Parser::parameterList()
{ 
	Token nextType = scanner.peek(); // Check if the next token is a )
	while (nextType.getVal() != ")") {
		if (nextType.getVal() == ",") {
			nextType = scanner.next();
		}
		string type = scanner.next().getVal();
		string name = scanner.next().getVal();
		symbols.put(name, type, "argument", arguments);
		arguments++;
		nextType = scanner.peek(); // Then we check if the next token is a )
	}
}

void Parser::subroutineBody(string name, string type)
{
	scanner.next(); // { symbol tag
	Token nextType = scanner.peek(); // Check if the next token is a statement
	while (nextType.getVal() != "let" && nextType.getVal() != "while" && nextType.getVal() != "if" && nextType.getVal() != "do" && nextType.getVal() != "return") {
		varDec();
		nextType = scanner.peek(); // Check if the next token is a statement
	}
	codeWriter.writeFunction(className + "." + name, locals);
	if (type == "method") {
		codeWriter.writePush("argument", 0);
		codeWriter.writePop("pointer", 0);
	}
	else if (type == "constructor") {
		codeWriter.writePush("constant", fields);
		codeWriter.writeCall("Memory.alloc", 1);
		codeWriter.writePop("pointer", 0);
	}
	statements();
	scanner.next(); // } symbol tag
}

void Parser::varDec()
{
	scanner.next();
	string type = scanner.next().getVal();
	Token nextType = scanner.next();
	vector<string> names;
	names.clear();
	// loop printing tags until we hit a semicolon
	// this should open up with a keyword and a type and the rest should all be identifiers and commas alternating
	while (nextType.getVal() != ";") {
		if(nextType.getTokenType() == type::IDENTIFIER){
			names.push_back(nextType.getVal());
		}
		nextType = scanner.next();
	}
	for(int  i = 0; i < names.size(); i++){
		symbols.put(names.at(i), type, "local", locals);
		locals++;
	}
}

void Parser::statements()
{
	Token nextType = scanner.peek(); // Check if the next token is a }
	while (nextType.getVal() != "}") {
		if (nextType.getVal() == "let") {
			letStatement();
		}
		else if (nextType.getVal() == "if") {
			ifStatement();
		}
		else if (nextType.getVal() == "while") {
			whileStatement();
		}
		else if (nextType.getVal() == "do") {
			doStatement();
		}
		else if (nextType.getVal() == "return") {
			returnFlag = true;
			returnStatement();
		}
		nextType = scanner.peek(); // Check if the next token is a statement
	}
	
}

void Parser::letStatement()
{
	scanner.next(); // let
	Token ident = scanner.next(); // ident
	symbolRecord rec = symbols.get(ident.getVal());
	Token nextType = scanner.peek();
	if (nextType.getVal() == "[") {
		scanner.next(); // [
		expression();
		scanner.next(); // ]
		scanner.next(); // =
		codeWriter.writePush(rec.segment, rec.offset);
		codeWriter.writeArtihmetic("add");
		expression();
		codeWriter.writePop("temp", 0);
		codeWriter.writePop("pointer", 1);
		codeWriter.writePush("temp", 0);
		codeWriter.writePop("that", 0);
	}
	else {
		scanner.next(); // =
		expression();
		symbolRecord rec = symbols.get(ident.getVal());
		codeWriter.writePop(rec.segment, rec.offset);
	}
	scanner.next(); // ;
}

void Parser::ifStatement()
{
	int curIf = ifCounter;
	ifCounter++;
	scanner.next(); // if
	scanner.next(); // (
	expression();
	codeWriter.writeIfGoTo("IF_TRUE" + to_string(curIf));
	codeWriter.writeGoTo("IF_FALSE" + to_string(curIf));
	scanner.next(); // )
	scanner.next(); // {
	codeWriter.writeLabel("IF_TRUE" + to_string(curIf));
	statements();
	scanner.next(); // }
	Token nextType = scanner.peek();
	if (nextType.getVal() == "else") {
		codeWriter.writeGoTo("IF_END" + to_string(curIf));
	}
	codeWriter.writeLabel("IF_FALSE" + to_string(curIf));
	
	if (nextType.getVal() == "else") {
		scanner.next(); // else
		scanner.next(); // {
		statements();
		scanner.next(); // }
		codeWriter.writeLabel("IF_END" + to_string(curIf));
	}
}

void Parser::whileStatement()
{
	int curWhile = whileCounter;
	whileCounter++;
	scanner.next(); // while
	scanner.next(); // (
	codeWriter.writeLabel("WHILE_EXP" + to_string(curWhile));
	expression();
	codeWriter.writeArtihmetic("not");
	codeWriter.writeIfGoTo("WHILE_END" + to_string(curWhile));
	scanner.next(); // )
	scanner.next(); // {
	statements();
	codeWriter.writeGoTo("WHILE_EXP" + to_string(curWhile));
	codeWriter.writeLabel("WHILE_END" + to_string(curWhile));
	scanner.next(); // }
}

void Parser::returnStatement()
{
	scanner.next(); // return
	string nextVal = scanner.peek().getVal();
	if (nextVal != ";") {
		expression();
	}
	else {
		codeWriter.writePush("constant", 0);
	}
	codeWriter.writeReturn();
	scanner.next(); // ;
}

void Parser::doStatement()
{
	scanner.next(); // do
	subroutineCall();
	codeWriter.writePop("temp", 0);
	scanner.next(); // ;
}
void Parser::subroutineCall() {
	string subName;
	Token nextType = scanner.peek2();
	if (nextType.getVal() != ".") {
		nextType = scanner.next(); // subroutineName ident
		subName = nextType.getVal();
		codeWriter.writePush("pointer", 0);
		scanner.next(); // (
		int numExpr = expressionList();
		scanner.next(); // )
		codeWriter.writeCall(className + "." + subName, numExpr + 1);
	}
	else {
		nextType = scanner.next(); // className/varName ident
		if (!symbols.contains(nextType.getVal())) {
			string cName = nextType.getVal();
			scanner.next(); // .
			nextType = scanner.next(); // subroutineName ident
			subName = nextType.getVal();
			scanner.next(); // (
			int numExpr = expressionList();
			scanner.next(); // )
			codeWriter.writeCall(cName + "." + subName, numExpr);
		}
		else {
			symbolRecord rec = symbols.get(nextType.getVal());
			scanner.next(); // .
			codeWriter.writePush(rec.segment, rec.offset);
			subName = scanner.next().getVal();
			scanner.next(); // (
			int numExpr = expressionList();
			scanner.next(); // )
			codeWriter.writeCall(rec.type + "." + subName, numExpr + 1);
		}
	}
}

void Parser::expression()
{
	term();
	Token nextType = scanner.peek();
	if (nextType.getVal() == "+" || nextType.getVal() == "-" || nextType.getVal() == "*" || nextType.getVal() == "/" || nextType.getVal() == "&" || nextType.getVal() == "|" || nextType.getVal() == "<" || nextType.getVal() == ">" || nextType.getVal() == "=") {
		string bop = scanner.next().getVal(); // bop
		term();
		if (bop == "+") {
			codeWriter.writeArtihmetic("add");
		}
		if (bop == "-") {
			codeWriter.writeArtihmetic("sub");
		}
		if (bop == "*") {
			codeWriter.writeCall("Math.multiply", 2);
		}
		if (bop == "/") {
			codeWriter.writeCall("Math.divide", 2);
		}
		if (bop == "&") {
			codeWriter.writeArtihmetic("and");
		}
		if (bop == "|") {
			codeWriter.writeArtihmetic("or");
		}
		if (bop == "<") {
			codeWriter.writeArtihmetic("lt");
		}
		if (bop == ">") {
			codeWriter.writeArtihmetic("gt");
		}
		if (bop == "=") {
			codeWriter.writeArtihmetic("eq");
		}
	}
}

int Parser::expressionList()
{
	int exprCount = 0;
	Token nextType = scanner.peek();
	// loop printing tags until we hit a )
	// this should open up with a ( and a expression and the rest should all be expressions and commas alternating
	while (nextType.getVal() != ")") {
		exprCount++;
		expression();
		nextType = scanner.peek();
		if (nextType.getVal() == ",") {
			scanner.next(); // ,
		}
	}
	return exprCount;
}

/*vector<string> Parser::subroutineCall()
{
	vector<string> result;
	vector<string> sub;
	result.push_back("<subroutineCall>");
	Token nextType = scanner.peek2();
	if (nextType.getVal() == ".") {
		scanner.next(); // className/varName ident
		scanner.next(); // .
	}
	scanner.next(); // subroutineName ident
	scanner.next(); // (
	sub = expressionList();
	for (string s : sub) {
		result.push_back("  " + s); // expressionList
	}
	scanner.next(); // )
	result.push_back("</subroutineCall>");
	return result;
}*/

void Parser::term()
{
	Token nextType = scanner.peek();
	if (nextType.getTokenType() == type::INT_CONST) {
		scanner.next();
		codeWriter.writePush("constant", stoi(nextType.getVal()));
	}
	else if (nextType.getTokenType() == type::STRING_CONST) {
		scanner.next();
		codeWriter.writePush("constant", nextType.getVal().size());
		codeWriter.writeCall("String.new", 1);
		for (char c : nextType.getVal()) {
			codeWriter.writePush("constant", c);
			codeWriter.writeCall("String.appendChar", 2);
		}
	}
	else if (nextType.getVal() == "(") {
		scanner.next();
		expression();
		scanner.next(); // )
	}
	else if (nextType.getTokenType() == type::IDENTIFIER) {
		Token peeked = scanner.peek2();
		if (peeked.getVal() == "(" || peeked.getVal() == ".") {
			subroutineCall();
		}
		else if (peeked.getVal() == "[") { // varname [expression]
			scanner.next();
			scanner.next(); // [
			expression();
			scanner.next(); // ]
			symbolRecord rec = symbols.get(nextType.getVal());
			codeWriter.writePush(rec.segment, rec.offset);
			codeWriter.writeArtihmetic("add");
			codeWriter.writePop("pointer", 1);
			codeWriter.writePush("that", 0);
			//codeWriter.writePop("this", 0); // This offset may have to be from the variable
		}
		else { // varname
			scanner.next();
			symbolRecord rec = symbols.get(nextType.getVal());
			codeWriter.writePush(rec.segment, rec.offset);
		}
	}
	else if (nextType.getTokenType() == type::KEYWORD) {
		scanner.next();
		string val = nextType.getVal();
		if (val == "true") {
			codeWriter.writePush("constant", 0);
			codeWriter.writeArtihmetic("not");
		}
		else if (val == "false") {
			codeWriter.writePush("constant", 0);
		}
		else if (val == "this") {
			codeWriter.writePush("pointer", 0);
		}
		else if (val == "null") {
			codeWriter.writePush("constant", 0);
		}
		else {
			throw "unhandled keyword in term";
		}
	}
	else if (nextType.getVal() == "~" || nextType.getVal() == "-") {
		scanner.next();
		string uop = nextType.getVal();
		term();
		if (uop == "~") {
			codeWriter.writeArtihmetic("not");
		}
		if (uop == "-") {
			codeWriter.writeArtihmetic("neg");
		}
	}
	/*if (nextType.getTokenType() == type::IDENTIFIER) {
		nextType = scanner.peek2();
		if (nextType.getVal() == "(" || nextType.getVal() == ".") {
			Token nextType = scanner.peek2();
			if (nextType.getVal() == ".") {
				scanner.next(); // className/varName ident
				scanner.next(); // .
			}
			scanner.next(); // subroutineName ident
			scanner.next(); // (
			sub = expressionList();
			for (string s : sub) {
				result.push_back("  " + s); // expressionList
			}
			scanner.next(); // )
		}
		else if (nextType.getVal() == "[") {
			scanner.next(); // ident
			scanner.next(); // [
			sub = expression();
			for (string s : sub) {
				result.push_back("  " + s); // expression
			}
			scanner.next(); // ]
		}
		else {
			scanner.next(); // ident
		}
	}
	else if (nextType.getVal() == "("){
		scanner.next(); // (
		sub = expression();
		for (string s : sub) {
			result.push_back("  " + s); // expression
		}
		scanner.next(); // )
	}
	else if (nextType.getVal() == "~" || nextType.getVal() == "-") {
		scanner.next(); // uop ~/-
		sub = term();
		for (string s : sub) {
			result.push_back("  " + s); // term
		}
	}
	else {
		scanner.next(); // intConst or strConst or keyword
	}
	result.push_back("</term>");
	return result;*/
}

