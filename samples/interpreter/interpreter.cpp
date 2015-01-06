#include <iostream>
#include <fstream>
#include <assert.h>


#include "treeNodes.h"
#include "tokenizer.h"

using namespace std;


Expression* parseTree (Tokenizer&);


Expression* parseNumber (Tokenizer& input)
{
	input.readToken();
	return new Number (atoi(input.lastTokenString().c_str()));
}

Expression* parseSum (Tokenizer& input)
{
//В този момент знаем, че на входа
//чака израз от вида
//+<expr><expr>


	input.readToken();
	//В този момент знаем, че на входа
	//чака израз от вида
	//<expr><expr>

	Expression *left = parseTree(input);
	//В този момент знаем, че на входа
	//чака израз от вида
	//<expr>

	Expression *right = parseTree (input);

	return new Sum (left,right);

}

Expression* parseMul (Tokenizer& input)
{
//В този момент знаем, че на входа
//чака израз от вида
//* <expr> <expr>


	input.readToken();
	//В този момент знаем, че на входа
	//чака израз от вида
	//<expr><expr>

	Expression *left = parseTree(input);
	//В този момент знаем, че на входа
	//чака израз от вида
	//<expr>

	Expression *right = parseTree (input);

	return new Mul (left,right);

}

Expression* parseIf (Tokenizer& input)
{
//В този момент знаем, че на входа
//чака израз от вида
//<ifExpr> ::= if <expr> then <expr> else <expr>

	input.readToken();

	//<expr> then <expr> else <expr>
	Expression *condition = parseTree (input);

	//then <expr> else <expr>
	assert (input.readToken()==Tokenizer::TokenThen);

	//<expr> else <expr>
	Expression *thenCase = parseTree (input);

	//else <expr>
	assert (input.readToken()==Tokenizer::TokenElse);

	//<expr>
	Expression *elseCase = parseTree (input);

	return new If (condition,thenCase,elseCase);


}


Expression *parseSetVar (Tokenizer& input)
{
	//<var_set_expr> ::= set <var_name> <expr>

	input.readToken();

	//<var_name> <expr>

	input.readToken();
	//НЕ роверихме, че не е ключова дума
	string name = input.lastTokenString ();

	//<expr>
	Expression *val = parseTree (input);

	return new SetVar (name,val);
}

Expression *parseReadVar (Tokenizer& input)
{
	//<var_read_expr> ::=  $<var_name>   

	input.readToken ();

	string token = input.lastTokenString();

	return new ReadVar (token.substr(1,token.size()-1));
}


Expression *parsePrint (Tokenizer& input)
{
	//<print_expr> ::= print <expr>

	input.readToken ();

	//expr
	return new Print (parseTree (input));
}

Expression *parseBlock (Tokenizer& input)
{//<block_expr> ::= begin <expr> .... <expr> end


	input.readToken();

	Block *result = new Block ();

	while (input.peekToken () != Tokenizer::TokenEnd)
	{
		result->addExpression (parseTree (input));
	}

	//end
	input.readToken();

	return result;

}

Expression* parseTree (Tokenizer& input)
{

	if (input.peekToken() == Tokenizer::TokenNumber)
		return parseNumber (input);

	if (input.peekToken() == Tokenizer::TokenPlus)
		return parseSum(input);

	if (input.peekToken() == Tokenizer::TokenMult)
		return parseMul(input);

	if (input.peekToken() == Tokenizer::TokenIf)
		return parseIf(input);

	if (input.peekToken() == Tokenizer::TokenSetVar)
		return parseSetVar (input);

	if (input.peekToken() == Tokenizer::TokenReadVar)
		return parseReadVar (input);

	if (input.peekToken() == Tokenizer::TokenBegin)
		return parseBlock (input);

	if (input.peekToken() == Tokenizer::TokenPrint)
		return parsePrint (input);


	cerr << "Uknown operator:" << input.lastTokenString() << endl;
	assert (false);
	return NULL;
}


void printToStream (ostream &out, Expression *expr)
{

	out << "digraph{" << endl;
	expr->print (out);
	out << "}" << endl;

}

int main ()
{



/*	//1
	Expression *myExpr1 = new Number (1);

	cout << "val=" << myExpr1->value()<< endl;

	//+ 1 2

	Expression *myExpr2 = new Sum (
									new Number (1),
									new Number (2));

	cout << "val=" << myExpr2->value()<< endl;

	//_* + 1 2 7
	Expression *myExpr3 = new Mul ( new Sum ( new Number (1),
											  new Number (2)),
									new Number (7));

	cout << "val=" << myExpr3->value()<< endl;

	myExpr3->print (cout);


	//_* + 1 2 7

	*/

	
	Tokenizer t(cin);

	Expression *myExpr = parseTree (t);

	myExpr->value();


	ofstream out ("expression.dot");

	printToStream (out,myExpr);


}









