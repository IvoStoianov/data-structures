#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;

//ВНИМАНИЕ: C++ 11!
//g++ hash_simple.cpp -std=c++11
template <class KeyType>
using HashFunction = int (*)(const KeyType&);
//HasFunction е тип указател към ф-я с един аргумент (ключ) и резутат int

template <class KeyType, class ValType>
class HashMap
{

private:
	vector<vector<pair<KeyType,ValType > > > entries;
	//entries[i] :-> вектор от двойки key-value
	//entries[i][j] :-> двойка key-value
	//entries[i][j].first -> ключ
	//entries[i][j].second -> стойност

	//entries.size () - размер на ХТ

	HashFunction<KeyType> hash;

	int subIndexOf (const KeyType& key, int hashCode)
	{
		for (int count = 0; 
			 count < entries[hashCode].size(); 
			 count++)
		{
			if (entries[hashCode][count].first == key)
				return count;
		}
		return -1;
	}

public:

	HashMap (int size, HashFunction<KeyType> f)
	{
		hash = f;

		//entries.resize (size)?;		
		vector<pair<KeyType,ValType > > empty;
		for (int count = 0; count < size; count++)
		{
			entries.push_back (empty);
		}
		//осигуряваме точно size на брой елемента в entries
		//entries[i], i=0,..,size-1
	} 

	void setPair (KeyType key, ValType value)
	{
		int hashCode = hash (key) % entries.size();
		int subIndex = subIndexOf (key,hashCode);

		if (subIndex == -1)
		{
			//entries[hashCode]
			//НЕ Е ЗАДЪЛЖИТЕЛНО ПРАЗЕН,
			//може да съдържа други key-value с ключове,
			//с които имаме колизия
			pair<KeyType,ValType> keyvalue;
			keyvalue.first = key;
			keyvalue.second = value;
			entries[hashCode].push_back (keyvalue);

		} else {
			//"сменам телефонния номер на Сандра"
			entries[hashCode][subIndex].second = value;
		}
	}

	ValType getValue (KeyType key)
	{
		int hashCode = hash (key) % entries.size();
		int subIndex = subIndexOf (key,hashCode);

		assert (subIndex != -1);

		return entries[hashCode][subIndex].second;
	}
/*


	ValType& operator [] (const KeyType& key)
	{		
	}

	void printAll (ostream &out)
	{
	}

	bool hasKey (KeyType key)
	{
	}
	*/
};


int hashStringSimple (const string &s)
{
	int sum = 0;
	for (int i = 0; i < s.size(); i++)
		sum += s[i];

	return sum;
}

int main ()
{
	HashMap<string,string> hash (50,hashStringSimple);

	hash.setPair ("dog","Usually smells badly");

	cout << hash.getValue ("dog") << endl;

/*	dict["cat"] = "Smaller than a dog";
	cout << dict["cat"] << endl;
	dict["cat"] = "Cuter than a dog";
	cout << dict["cat"] << endl;

	dict.printAll (cout);

	if (dict.hasKey ("dog"))
		cout << "Yes dog\n";
	else
		cout << "No dog\n";
*/
}