#include <iostream>
#include <fstream>
#include <assert.h>
#include <map>
#include <vector>


#include "tokenizer.h"

using namespace std;


int countAllNodes = 0;

class Expression
{
private:
	int myID;
public:
	virtual double value () = 0;
	virtual void print (ostream&) = 0;
public:
	virtual int getId ()
	{
		return myID;
	}
public:
	Expression ()
	{
		myID = countAllNodes;
		countAllNodes++;
	}

};

class Number : public Expression

{
private:
	double val;
public:
	double value ()
	{
		return val;
	}
	Number (double _val):val(_val){}
	void print (ostream &out)
	{
		out << getId() << "[label=\"" << val << "\"];" << endl;

	}
};

class Sum : public Expression
{

private:
	
Expression *left, *right;

public:

	double value ()
	{
		return left->value() + right->value();
	}

	Sum (Expression *l, Expression *r):left(l),right(r){}
	void print (ostream &out)
	{
		out << getId() << "[label=\"+\"];" << endl;
		out << getId() << "->" << left->getId() << ";" << endl;
		out << getId() << "->" << right->getId() << ";" << endl;
		left->print (out);
		right->print (out);

	}


};

//!!!
map<string,int> variables;

class ReadVar : public Expression
{
private:
	string varName;
public:
	ReadVar (string name):varName(name){}
	double value ()
	{
		return variables[varName];
	}

	void print (ostream &out)
	{}


};

class SetVar : public Expression
{

private:
	string varName;
	Expression *expr;	
public:

	SetVar (string name, Expression *ex):varName(name),expr (ex){}

	double value ()
	{
		variables[varName] = expr->value();
		return variables[varName];
	}

	void print (ostream &out)
	{}


};

class Mul : public Expression
{

private:
	
Expression *left, *right;

public:

	double value ()
	{
		return left->value() * right->value();
	}

	Mul (Expression *l, Expression *r):left(l),right(r){}
	void print (ostream &out)
	{
		out << getId() << "[label=\"*\"];" << endl;
		out << getId() << "->" << left->getId() << ";" << endl;
		out << getId() << "->" << right->getId() << ";" << endl;
		left->print (out);
		right->print (out);

	}

};

class Block : public Expression
{
private:
	vector <Expression*> expressions;
public:
	Block () {}
	void addExpression (Expression *e)
	{
		expressions.push_back (e);
	}
	double value ()
	{
		double lastVal;
		for (int i = 0; i < expressions.size(); i++)
		{
			lastVal=expressions[i]->value();
		}
		return lastVal;
	}
	void print (ostream &out)
	{

	}

};

//<while_expr> ::= while <expr> <expr>
//while x set x (- x 1)
class If : public Expression
{
private:
	Expression *condition, 
	           *thenCase, 
	           *elseCase;
public:
	double value ()
	{
		if (condition->value() != 0)
			return thenCase->value();
		return elseCase->value();
	}
	//!
	If (Expression *cond, 
		Expression *th, 
		Expression *el):condition (cond), 
	                    thenCase (th), 
	                    elseCase (el){}

	void print (ostream &out)
	{
			//???

	}

};

Expression* createTree (Tokenizer&);


Expression* createNumber (Tokenizer& input)
{
	input.readToken();
	return new Number (atoi(input.lastTokenString().c_str()));
}

Expression* createSum (Tokenizer& input)
{
//В този момент знаем, че на входа
//чака израз от вида
//+<expr><expr>


	input.readToken();
	//В този момент знаем, че на входа
	//чака израз от вида
	//<expr><expr>

	Expression *left = createTree(input);
	//В този момент знаем, че на входа
	//чака израз от вида
	//<expr>

	Expression *right = createTree (input);

	return new Sum (left,right);

}

Expression* createMul (Tokenizer& input)
{
//В този момент знаем, че на входа
//чака израз от вида
//* <expr> <expr>


	input.readToken();
	//В този момент знаем, че на входа
	//чака израз от вида
	//<expr><expr>

	Expression *left = createTree(input);
	//В този момент знаем, че на входа
	//чака израз от вида
	//<expr>

	Expression *right = createTree (input);

	return new Mul (left,right);

}

Expression* createIf (Tokenizer& input)
{
//В този момент знаем, че на входа
//чака израз от вида
//<ifExpr> ::= if <expr> then <expr> else <expr>

	input.readToken();

	//<expr> then <expr> else <expr>
	Expression *condition = createTree (input);

	//then <expr> else <expr>
	assert (input.readToken()==Tokenizer::TokenThen);

	//<expr> else <expr>
	Expression *thenCase = createTree (input);

	//else <expr>
	assert (input.readToken()==Tokenizer::TokenElse);

	//<expr>
	Expression *elseCase = createTree (input);

	return new If (condition,thenCase,elseCase);


}


Expression *createSetVar (Tokenizer& input)
{
	//<var_set_expr> ::= set <var_name> <expr>

	input.readToken();

	//<var_name> <expr>

	input.readToken();
	//НЕ роверихме, че не е ключова дума
	string name = input.lastTokenString ();

	//<expr>
	Expression *val = createTree (input);

	return new SetVar (name,val);
}

Expression *createReadVar (Tokenizer& input)
{
	//<var_read_expr> ::=  $<var_name>   

	input.readToken ();

	string token = input.lastTokenString();

	return new ReadVar (token.substr(1,token.size()-1));
}

Expression *createBlock (Tokenizer& input)
{//<block_expr> ::= begin <expr> .... <expr> end


	input.readToken();

	Block *result = new Block ();

	while (input.peekToken () != Tokenizer::TokenEnd)
	{
		result->addExpression (createTree (input));
	}

	//end
	input.readToken();

	return result;

}

Expression* createTree (Tokenizer& input)
{

	if (input.peekToken() == Tokenizer::TokenNumber)
		return createNumber (input);

	if (input.peekToken() == Tokenizer::TokenPlus)
		return createSum(input);

	if (input.peekToken() == Tokenizer::TokenMult)
		return createMul(input);

	if (input.peekToken() == Tokenizer::TokenIf)
		return createIf(input);

	if (input.peekToken() == Tokenizer::TokenSetVar)
		return createSetVar (input);

	if (input.peekToken() == Tokenizer::TokenReadVar)
		return createReadVar (input);

	if (input.peekToken() == Tokenizer::TokenBegin)
		return createBlock (input);


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

	Expression *myExpr = createTree (t);

	cout << "val=" << myExpr->value() << endl;


	ofstream out ("expression.dot");

	printToStream (out,myExpr);


}









