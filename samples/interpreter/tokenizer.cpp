#include <string>
#include "tokenizer.h"


map<string,int> Tokenizer::keywords;

void Tokenizer::initKeywords ()
{
	static bool keywordsInitialized = false;

	if (!keywordsInitialized)
	{
		keywords["+"] = Tokenizer::TokenPlus;
		keywords["*"] = Tokenizer::TokenMult; 
		keywords["if"] = Tokenizer::TokenIf;
		keywords["then"] = Tokenizer::TokenThen;
		keywords["else"] = Tokenizer::TokenElse;
		keywords["set"] = Tokenizer::TokenSetVar;
		keywords["begin"] = Tokenizer::TokenBegin;
		keywords["end"] = Tokenizer::TokenEnd;
		keywords["print"] = Tokenizer::TokenPrint;
		keywordsInitialized = true;
	}
}


bool isDigit (char c)
{
	return c >= '0' && c <= '9';
}

void eatWhiteSpaces (istream &input)
{
	while (input.peek() == 32 || 
		   input.peek () == 10 ||
		   input.peek () == 13)
		input.get();
}

Tokenizer::Tokenizer (istream& in):input (in)
{
	tokenPeeked = false;
	initKeywords();
}


int Tokenizer::readToken ()
{
	
	if (!tokenPeeked)
	{

		eatWhiteSpaces (input);

		input >> cachedToken;

	}

	tokenPeeked = false;

	return lastTokenType ();
}

int Tokenizer::peekToken ()
{
		
	if (!tokenPeeked)
	{
		readToken ();
		tokenPeeked = true;	
	}

	return lastTokenType();
}

string Tokenizer::lastTokenString ()
{
	return cachedToken;
}


int Tokenizer::lastTokenType ()
{

	if (isDigit(cachedToken[0]))
	{
		return Tokenizer::TokenNumber;
	}

	if (cachedToken[0] == '$')
	{
		return Tokenizer::TokenReadVar;
	}

	if(keywords.find(cachedToken) == keywords.end())
		//ключ word не съществава в таблицата с
		//ключови думи 
	{
		return -1;
	}

	return keywords[cachedToken];
}


