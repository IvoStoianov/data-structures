#include <iostream>
#include <cstring>
#include <iomanip>
#include <assert.h>
#include <string.h>
#include <string>


//да приемем: 10 състояния
//да приемем: азбука: малки латински букв

//ДЕТЕРМИНИРАН FSA
//26 букви + колона за епс. преходи

//int table[10][27] = {2,2,-1,-1,-1,-1};


using namespace std;

#define MAX_STATE 20

class DFSA
{
private:
	int table[MAX_STATE][27];
	int startState;
	bool final[MAX_STATE];

public:
	DFSA ()
	{
		/*
			Инициализираме матрицата на преходите
			с -1 ("няма преход")
		*/
		for (int state = 0; state < MAX_STATE; state++)
		{
			for (char symbol = 'a'; symbol <= 'z'; symbol++)
				table[state][symbol-'a'] = -1;
			final[state] = false;
		}

		startState = 0;
	}

	void setStartState (int state)
	{
		startState = state;
	}

	void setFinal (int state, bool isFinal=true)
	{
		final[state]=isFinal;
	}

	DFSA& addTrans (int fromState, int toState, char symbol)
	{
		table[fromState][symbol-'a'] = toState;
		return *this;
	}

	friend ostream& operator << (ostream &, const DFSA&);

//	cout << "recognize(\"abc\")" << a.recognize ("abc") << endl;

	bool recognize (char *word)
	{

		int wordLength = strlen (word);

		int currentState = startState;

		for (int wordIndex = 0; 
			     wordIndex < wordLength; 
			     wordIndex++)
		{
			assert (word[wordIndex] >= 'a' && word[wordIndex] <= 'z');


			currentState = table[currentState][word[wordIndex]-'a'];

			if (currentState == -1)
				return false;

		}

		return final[currentState];

	}


	//рекурсивен вариант

	//можем ли да разпознаем думата word,
	//стартирайки от currentState (което не е задължително startState)
	bool recognize (int currentState, char *word)
	{

/*		return currentState != -1 &&
			   (
		        (word[0] == 0 && final[currentState]) ||
		        (word[0] != 0 && 
		        	recognize (nextState (currentState,word),word+1)));

*/
		if (currentState == -1)
			return false;

		if (word[0]==0)
			return final[currentState];


		 return recognize (nextState (currentState,word),
			              word+1);

	}

//int countAllWordsFrom (int currentState,.....)
	void printAllWordsFrom (int currentState, string currentString)
	{//int count = 0; count = 1; count += ...;return count;
		

		if (final[currentState])
		{
			cout << currentString << endl;

		}

		char symbol;
		int nextState;
		
		for (symbol = 'a'; symbol <= 'z'; symbol++)
		{
			nextState = table[currentState][symbol-'a'];
			if (nextState != -1)
			{
			
				printAllWordsFrom (nextState,currentString+symbol);
			}

		}
	}

private:
	int nextState (int currentState, char *word)
	{
		return table[currentState][word[0]-'a'];
	}

};



ostream& operator << (ostream &out, const DFSA& a)
{
//GRAPHVIS	

	char symbol;

	int state;


	//отпечатваме матрицата на преходите

/*
	for (symbol = 'a'; symbol <= 'z'; symbol++)
		out << setw(2) << symbol;

	out << endl;

	for (state = 0; state < MAX_STATE; state++)
	{
		for (symbol = 'a'; symbol <= 'z'; symbol++)
			if (a.table[state][symbol-'a'] != -1)
				out << setw(2) << a.table[state][symbol-'a'];
			else
				out << "__";
		out << endl;	
	}
*/

	out << "digraph {" << endl;

	//отпечатваме списък с всички преходи

	for (state = 0; state < MAX_STATE; state++)
	{
		for (symbol = 'a'; symbol <= 'z'; symbol++)
			if (a.table[state][symbol-'a'] != -1)
				out << state << " -> " << a.table[state][symbol-'a'] << " [label=\"" << symbol << "\"];" << endl;
	}

	//отпечатваме списък с всички финални с-я

	for (state = 0; state < MAX_STATE; state++)
		if (a.final[state])
			out << state << "[style=filled]";

	out << "}" << endl;


	//dot -Tpdf test.dot -o test.pdf

	return out;
}


int main ()
{

	//cout << table[0]['z'-'a'];

	DFSA a;

	a.addTrans(0,1,'a').addTrans(1,3,'a').
	  addTrans(1,3,'c').addTrans(0,2,'c').
	  addTrans(1,6,'b').addTrans(2,4,'b').
	  addTrans(3,5,'b').addTrans(6,5,'c').
	  addTrans(4,5,'a').addTrans (0,5,'z');

	 a.setFinal (5);

	cout << a;

	/*cout << "recognize(\"abc\")=" << a.recognize ("abc") << endl;
	cout << "recognize(\"abc\")=" << a.recognize (0,"zabcazzaza") << endl;
*/
	//a.printAllWordsFrom (0,"");

/*	a.printLanguage ();

*/

}