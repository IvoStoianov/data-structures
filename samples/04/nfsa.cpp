#include <iostream>
#include <cstring>
#include <iomanip>
#include <assert.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

#define MAX_STATE 100

class NFSA
{
private:
	//int table[MAX_STATE][27];
	vector<int> table[MAX_STATE][27];

	int startState, maxUsedState;
	bool final[MAX_STATE];


public:

	NFSA operator + (const NFSA& a)
	{

		//*this, a

		NFSA result;

		//копираме таблицата на this

		int state; char symbol;
		for (state = 0; state < MAX_STATE; state++)
			for (symbol = 0; symbol < 27; symbol ++)
				result.table[state][symbol] = table[state][symbol];

		//копираме таблицата на a
		//с ПРЕИНДЕКСИРАНЕ 

		int oldFromStateNumber, 
		    newFromStateNumber, 
		    oldToStateNumber, 
		    newToStateNumber,
		    count;

		vector<int> oldToStates;
		
		for (oldFromStateNumber = 0; 
			 oldFromStateNumber < MAX_STATE; 
			 oldFromStateNumber++)
		{
			newFromStateNumber = oldFromStateNumber + this->maxUsedState+1;

			for (symbol = 0; symbol < 27; symbol ++)
			{
				oldToStates = a.table[oldFromStateNumber][symbol];

				for (count = 0; count < oldToStates.size(); count++)
				{

					oldToStateNumber = oldToStates[count];

					newToStateNumber = oldToStateNumber + this->maxUsedState+1;

					result.table[newFromStateNumber][symbol].push_back(newToStateNumber);

				}
			}
		}

		result.startState = this->maxUsedState + a.maxUsedState + 2;
		result.maxUsedState = result.startState;

		//копираме преходите от старите начални
		//състояния към новото начално състояние

		for (symbol = 0; symbol < 27; symbol++)
		{

			result.table[result.startState][symbol] =
				this->table[this->startState][symbol];

		}


		for (symbol = 0; symbol < 27; symbol++)
		{

			oldToStates = a.table[a.startState][symbol];

			for (count = 0; count < oldToStates.size(); count++)
			{
				newToStateNumber = oldToStates[count] + this->maxUsedState+1;
				result.table[result.startState][symbol].push_back (newToStateNumber);
			}

		}

		//прехвърляне на финалните състояния

		for (state = 0; state < MAX_STATE; state++)
		{
			if (this->final[state])
				result.final[state] = true;

			if (a.final[state])
				result.final[state+this->maxUsedState+1] = true;

		}

		if (this->final[this->startState] || a.final[a.startState])
			result.final[result.startState] = true;


		return result;


	}

	NFSA ()
	{
		
		startState = 0;
		maxUsedState = 0;
		for (int i = 0; i < MAX_STATE; i++)
			final[i] = false;
	}

	void setStartState (int state)
	{
		startState = state;
	}

	void setFinal (int state, bool isFinal=true)
	{
		final[state]=isFinal;
	}

	NFSA& addTrans (int fromState, int toState, char symbol)
	{
		table[fromState][symbol-'a'].push_back(toState);

		maxUsedState = max (maxUsedState, max (toState, fromState));

		return *this;
	}	

	friend ostream& operator << (ostream &, const NFSA&);

	bool recognize (int currentState, char *word)
	{

		if (word[0] == 0)
			return final[currentState] == true;

		char head = word[0] - 'a';
		char *tail = word+1;

		vector<int> nextStates = table[currentState][head];

		for (int count = 0; count < nextStates.size(); count++)
		{
			if (recognize (nextStates[count],tail))
				return true;
		}

		return false;


	}


	void printAllWordsFrom (int currentState, string currentString)
	{
		

		if (final[currentState])
		{
			cout << currentString << endl;

		}

		char symbol;
		vector<int> nextStates;
		
		for (symbol = 'a'; symbol <= 'z'; symbol++)
		{
			nextStates = table[currentState][symbol-'a'];

			for (int count = 0; count < nextStates.size(); count++)
			{
			
				printAllWordsFrom (nextStates[count],currentString+symbol);
			}

		}
	}





};


ostream& operator << (ostream &out, const NFSA& a)
{

	char symbol;

	int state, toState, i;


	out << "digraph {" << endl;

	//отпечатваме списък с всички преходи

	for (state = 0; state < MAX_STATE; state++)
	{
		for (symbol = 'a'; symbol <= 'z'; symbol++)
		{
			vector<int> targetStates = a.table[state][symbol-'a'];

			for (i = 0; i < targetStates.size(); i++)
				out << state << " -> " << targetStates[i] << " [label=\"" << symbol << "\"];" << endl;

		}
	}

	//отпечатваме списък с всички финални с-я

	for (state = 0; state < MAX_STATE; state++)
		if (a.final[state])
			out << state << "[style=fixlled]";

	out << "}" << endl;


	//dot -Tpdf test.dot -o test.pdf

	return out;
}


int main ()
{


/*	NFSA a;

	a.addTrans(0,1,'a').addTrans(1,3,'a').
	  addTrans(1,3,'c').addTrans(0,2,'c').
	  addTrans(1,6,'b').addTrans(2,4,'b').
	  addTrans(3,5,'b').addTrans(6,5,'c').
	  addTrans(4,5,'a').addTrans (0,5,'z').
	  addTrans(0,5,'s').addTrans (0,2,'a');

	 a.setFinal (5);
	 a.setFinal (2);
	 a.setFinal (6);

	cout << a;
*/




	//cout << "abz=" << a.recognize (0,"abz") << endl;

//	a.printAllWordsFrom (0,"");

	NFSA a,b;

	a.addTrans (0,1,'a');
	a.addTrans (1,2,'z');
	a.setFinal (2);
	
	b.addTrans (0,1,'b');
	b.addTrans (1,2,'w');
	b.setFinal (2);

	cout << a+b;

}
