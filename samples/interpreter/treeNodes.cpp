#include "treeNodes.h"
#include <assert.h>

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

map<string,double> variables;
map<string,pair<vector<string>,Expression*> > functions;

ReadVar::ReadVar (string name):varName(name){}
double ReadVar::value ()
{
	return variables[varName];
}

void ReadVar::print (ostream &out)
{
	out << getId() << "[label=\"" << varName << "\"];" << endl;

}

SetVar::SetVar (string name, Expression *ex):varName(name),expr (ex){}

double SetVar::value ()
{
	variables[varName] = expr->value();
	return variables[varName];
}

void SetVar::print (ostream &out)
{

	out << getId() << "[label=\"set: " << varName << "\"];" << endl;

}


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

	out << getId() << "[label=\"block\"];" << endl;
	for (int i = 0; i < expressions.size(); i++)
	{
		out << getId() << "->" << expressions[i]->getId() << ";" << endl;
		expressions[i]->print (out);
	}

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
	out << getId() << "[label=\"if\"];" << endl;
	out << getId() << "->" << condition->getId() << ";" << endl;
	out << getId() << "->" << thenCase->getId() << ";" << endl;
	out << getId() << "->" << elseCase->getId() << ";" << endl;
	condition->print (out);
	thenCase->print (out);
	elseCase->print (out);
}

Print::Print(Expression *toPrint):expr(toPrint){};

double Print::value ()
{
	double val = expr->value();
	cout << endl << val << endl;
	return val;
}

void Print::print (ostream &out)
{
	out << getId() << "[label=\"print\"];" << endl;
	out << getId() << "->" << expr->getId() << ";" << endl;
	expr->print (out);

}


For::For (string var, 
		  Expression *from,
		  Expression *to,
		  Expression *step,
		  Expression *body):varName (var),
 							fromExpr (from),
 							toExpr (to),
 							stepExpr(step),
 							bodyExpr (body){}

//<for_expr> ::= for <var_name> from <expr> to <expr> step <expr> do <expr>
//for x from 1 to 10 step 1 do print x

double For::value ()
{
	double from = fromExpr->value(),
	       to = toExpr->value(),
	       step = stepExpr->value(),
	       lastVal = -1;


	for (double controlVal = from; 
		 (step > 0?controlVal <= to:controlVal >= from);
		 controlVal += step)
	{
		variables[varName] = controlVal;
		lastVal = bodyExpr->value();
	}

	return lastVal; 

}

void For::print (ostream &out)
{
	out << getId() << "[label=\"for: " << varName << "\"];" << endl;
	out << getId() << "->" << fromExpr->getId() << ";" << endl;
	out << getId() << "->" << toExpr->getId() << ";" << endl;
	out << getId() << "->" << stepExpr->getId() << ";" << endl;
	out << getId() << "->" << bodyExpr->getId() << ";" << endl;
	fromExpr->print (out);
	toExpr->print (out);
	stepExpr->print (out);	
	bodyExpr->print (out);	
}
	


FunDef::FunDef (string name, 
	            vector<string> args, 
	            Expression *b):fnName (name),
 							   formArgs (args),
 							   body (b){}
double FunDef::value ()
{
	
	functions[fnName] = pair<vector<string>,Expression*>(formArgs,body);
	return -1;
}

void FunDef::print (ostream &out)
{
	out << getId() << "[label=\"fun: " << fnName << " ";

	for (int i = 0; i < formArgs.size(); i++)
		out << formArgs[i] << " ";

	out << "\"];" << endl;

	out << getId() << "->" << body->getId() << ";" << endl;
	body->print (out);
	
}


//<call_fn> ::= call <var_name>  <expr>* end 

CallFn::CallFn (string name, 
	            vector<Expression*>args):fnName(name),
									     factArgs(args){}
double CallFn::value ()
{
//	functions[fnName] = pair<vector<string>,Expression*>(formArgs,body);

	vector<string>formalArgs = functions[fnName].first;
	Expression* body = functions[fnName].second;

	assert (formalArgs.size() == factArgs.size());

	for (int i = 0; i < formalArgs.size(); i++)
		variables[formalArgs[i]] = factArgs[i]->value();

	return body->value();
}

void CallFn::print (ostream &out)
{
	out << getId() << "[label=\"call: " << fnName << "\"];" << endl;
	for (int i = 0; i < factArgs.size(); i++)
	{
		out << getId() << "->" << factArgs[i]->getId() << ";" << endl;
		factArgs[i]->print (out);
	}

}


