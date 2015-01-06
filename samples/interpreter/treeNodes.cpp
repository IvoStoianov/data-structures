#include "treeNodes.h"

int countAllNodes = 0;

Expression::Expression ()
{
	myID = countAllNodes;
	countAllNodes++;
}

int Expression::getId ()
{
	return myID;
}


double Number::value ()
{
	return val;
}

Number::Number (double _val):val(_val){}

void Number::print (ostream &out)
{
	out << getId() << "[label=\"" << val << "\"];" << endl;

}

double Sum::value ()
{
	return left->value() + right->value();
}

Sum::Sum (Expression *l, Expression *r):left(l),right(r){}

void Sum::print (ostream &out)
{
	out << getId() << "[label=\"+\"];" << endl;
	out << getId() << "->" << left->getId() << ";" << endl;
	out << getId() << "->" << right->getId() << ";" << endl;
	left->print (out);
	right->print (out);

}


map<string,int> variables;

ReadVar::ReadVar (string name):varName(name){}
double ReadVar::value ()
{
	return variables[varName];
}

void ReadVar::print (ostream &out)
{}

SetVar::SetVar (string name, Expression *ex):varName(name),expr (ex){}

double SetVar::value ()
{
	variables[varName] = expr->value();
	return variables[varName];
}

void SetVar::print (ostream &out)
{}


double Mul::value ()
{
	return left->value() * right->value();
}

Mul::Mul (Expression *l, Expression *r):left(l),right(r){}
void Mul::print (ostream &out)
{
	out << getId() << "[label=\"*\"];" << endl;
	out << getId() << "->" << left->getId() << ";" << endl;
	out << getId() << "->" << right->getId() << ";" << endl;
	left->print (out);
	right->print (out);

}

Block::Block () {}

void Block::addExpression (Expression *e)
{
	expressions.push_back (e);
}

double Block::value ()
{
	double lastVal;
	for (int i = 0; i < expressions.size(); i++)
	{
		lastVal=expressions[i]->value();
	}
	return lastVal;
}

void Block::print (ostream &out)
{

}

//<while_expr> ::= while <expr> <expr>
//while x set x (- x 1)
double If::value ()
{
	if (condition->value() != 0)
		return thenCase->value();
	return elseCase->value();
}
//!
If::If (Expression *cond, 
		Expression *th, 
		Expression *el):condition (cond), 
        	            thenCase (th), 
            	        elseCase (el){}

void If::print (ostream &out)
{
		//???

}

Print::Print(Expression *toPrint):expr(toPrint){};

double Print::value ()
{
	int val = expr->value();
	cout << endl << val << endl;
	return val;
}

void Print::print (ostream &out){}
