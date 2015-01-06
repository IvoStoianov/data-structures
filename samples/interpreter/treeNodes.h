#ifndef __TREENODES_H
#define __TREENODES_H

#include <map>
#include <vector>
#include <string>
#include <iostream>


using namespace std;

class Expression
{
private:
	int myID;
public:
	virtual double value () = 0;
	virtual void print (ostream&) = 0;
public:
	virtual int getId ();
public:
	Expression ();
};

class Number : public Expression

{
private:
	double val;
public:
	double value ();
	Number (double _val);
	void print (ostream &out);
};

class Sum : public Expression
{

private:
	
	Expression *left, *right;

public:

	double value ();
	Sum (Expression *l, Expression *r);
	void print (ostream &out);
};

class ReadVar : public Expression
{
private:
	string varName;
public:
	ReadVar (string name);
	double value ();
	void print (ostream &out);
};

class SetVar : public Expression
{

private:
	string varName;
	Expression *expr;	
public:

	SetVar (string name, Expression *ex);
	double value ();
	void print (ostream &out);

};

class Mul : public Expression
{

private:
	
	Expression *left, *right;

public:

	double value ();
	Mul (Expression *l, Expression *r);
	void print (ostream &out);

};

class Block : public Expression
{
private:
	vector <Expression*> expressions;
public:
	Block ();
	void addExpression (Expression *e);
	double value ();
	void print (ostream &out);
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
	double value ();
	//!
	If (Expression *cond, 
		Expression *th, 
		Expression *el);

	void print (ostream &out);
};

class Print : public Expression
{
private:
	Expression *expr;

public:

	Print(Expression *toPrint);
	double value ();
	void print (ostream &out);

};

//<for_expr> ::= for <var_name> from <expr> to <expr> step <expr>
class For : public Expression
{
private:
	string varName;
	Expression *fromExpr, *toExpr, *stepExpr, *bodyExpr;
public:
	For (string var, 
		Expression *from,
		Expression *to,
		Expression *step,
		Expression *body);
		double value ();
		void print (ostream &out);


};

//<def_fn> ::= function (<var_name>)+ do <expr>  
class FunDef : public Expression
{
private:
	string fnName;
	vector<string> formArgs;
	Expression *body;
public:
	FunDef (string name, vector<string> a, Expression *b);
	double value ();
	void print (ostream &out);
};

//<call_fn> ::= call <var_name>  <expr>* end 

class CallFn : public Expression
{
private:
	string fnName;
	vector<Expression*> factArgs;
public:
	CallFn (string name, vector<Expression*>args);
	double value ();
	void print (ostream &out);

};



#endif