#ifndef TOKENIZER__H_
#define TOKENIZER__H_ 

#include <iostream>
#include <string>
#include <map>

using namespace std;

class Tokenizer
{
private:

	static map<string,int> keywords;
	
	istream &input;

	void initKeywords ();

	string cachedToken;
	bool tokenPeeked;

public:

	Tokenizer (istream& in);

	int peekToken ();
	int readToken ();

	static const int TokenNumber = 0;
	static const int TokenPlus = 1;
	static const int TokenMult = 2;
	static const int TokenIf = 3;
	static const int TokenThen = 4;
	static const int TokenElse = 5;
	static const int TokenSetVar = 6;
	static const int TokenReadVar = 7;
	static const int TokenBegin = 8;
	static const int TokenEnd = 9;
	static const int TokenPrint = 10;
	static const int TokenFor = 11;
	static const int TokenFrom = 12;
	static const int TokenTo = 13;
	static const int TokenStep = 14;
	static const int TokenDo = 15;
	static const int TokenFunction = 16;
	static const int TokenCall = 17;

	int lastTokenType ();
	string lastTokenString();

};

#endif