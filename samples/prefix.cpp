#include <iostream>
#include <fstream>
#include <assert.h>

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

bool isDigit (char c)
{
	return c >= '0' && c <= '9';
}

Expression* createTree (istream&);


Expression* createNumber (istream& input)
{
	double val;
	input >> val;
	return new Number (val);
}

Expression* createSum (istream& input)
{
//В този момент знаем, че на входа
//чака израз от вида
//+<expr><expr>


	char c;
	input >> c;
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

Expression* createMul (istream& input)
{
//В този момент знаем, че на входа
//чака израз от вида
//*<expr><expr>


	char c;
	input >> c;
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

void eatWhiteSpaces (istream &input)
{
	while (input.peek() == 32 || 
		   input.peek () == 10 ||
		   input.peek () == 13)
		input.get();
}

Expression* createTree (istream& input)
{

	eatWhiteSpaces (input);

	if (isDigit (input.peek()))
		return createNumber (input);

	if (input.peek() == '+')
		return createSum(input);

	if (input.peek() == '*')
		return createMul(input);

	cerr << "Uknown operator:" << input.peek() << endl;
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

	Expression *myExpr = createTree (cin);

	cout << "val=" << myExpr->value() << endl;


	ofstream out ("expression.dot");

	printToStream (out,myExpr);


}









