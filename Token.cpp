#include "Token.h"

Token::Token(string val) {
	value = val;
	if (count(val.begin(), val.end(), '"') != 0) {
		token_type = type::STRING_CONST;
		value = value.substr(1,value.size()-2);
	}
	else if (val == "class" || val == "constructor" || val == "function" || val == "method" || val == "field" || val == "static" || 
		val == "var" || val == "int" || val == "char" || val == "boolean" || val == "void" || val == "true" || val == "false" || 
		val == "null" || val == "this" || val == "let" || val == "do" || val == "if" || val == "else" || val == "while" || val == "return") {
		token_type = type::KEYWORD;
	}
	else if (val == "{" || val == "}" || val == "(" || val == ")" || val == "[" || val == "]" || val == "." || val == "," || val == ";" || 
		val == "+" || val == "-" || val == "*" || val == "/" || val == "&" || val == "|" || val == "<" || val == ">" || val == "=" || val == "~" ) {
		token_type = type::SYMBOL;
	}
	else {
		try {
			stoi(val);
			token_type = type::INT_CONST;
		}
		catch (exception e) {
			if (val.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_") == string::npos && val.substr(0, 1).find_first_of("0123456789") == string::npos) {
				token_type = type::IDENTIFIER;
			}
			else throw "invalid token";
		}
	}
	
}
type Token::getTokenType() {
	return token_type;
}
string Token::getVal() {
	return value;
}
string Token::to_string() {
	string returnString;
	switch (token_type) {
		case type::STRING_CONST:
			returnString =  "<stringConstant> " + value + " </stringConstant>";
			break;
		case type::INT_CONST:
			returnString = "<integerConstant> " + value + " </integerConstant>";
			break;
		case type::KEYWORD:
			returnString =  "<keyword> " + value + " </keyword>";
			break;
		case type::SYMBOL:
			if (value == "<") returnString = "<symbol> &lt; </symbol>";
			else if (value == ">") returnString = "<symbol> &gt; </symbol>";
			else if (value == "&") returnString = "<symbol> &amp; </symbol>";
			else returnString = "<symbol> " + value + " </symbol>";
			
			break;
		case type::IDENTIFIER:
			returnString = "<identifier> " + value + " </identifier>";
			break;
	}
	return returnString;
}