#include <iostream>
#include <cstring>
#include <assert.h>


//да приемем: 10 състояния
//да приемем: азбука: малки латински букв

//ДЕТЕРМИНИРАН FSA
//26 букви + колона за епс. преходи

int table[10][27] = {2,2,-1,-1,-1,-1};


int main ()
{
	cout << table[0]['z'-'a'];
}