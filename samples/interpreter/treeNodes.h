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

#endif