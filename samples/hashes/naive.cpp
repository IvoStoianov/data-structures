#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;


template <class KeyType, class ValType>
class Dictionary
{

private:
	vector<KeyType> keys;
	vector<ValType> values;

	int keyIndex (KeyType key)
	//or returns -1
	{
		for (int count = 0; count < keys.size(); count++)
			if (keys[count] == key)
				return count;

		return -1;
	}
public:


	ValType getValue (KeyType key)
	{
		int index = keyIndex (key);

		assert (index != -1);

		return values[index];

	}

	void setPair (KeyType key, ValType value)
	{
		int index = keyIndex (key);

		if (index != -1)
		{
			values[index] = value;
		} else {
			keys.push_back(key);
			values.push_back (value);
		}

	}

	ValType& operator [] (KeyType key)
	{
		int index = keyIndex (key);

		if (index == -1)
		{
			ValType dummy;
			setPair (key,dummy);
			//index = keys.size()-1
			//лошо - разчитаме, че знаем как работи
			//setPair
			index = keyIndex (key);
		}

		assert (index != -1);
		
		return values[index];
	}


	void printAll (ostream &out)
	{
		for (int count = 0; count < keys.size(); count++)
		{
			out << keys[count] << ":" << values[count] << endl;
		}
	}

	bool hasKey (KeyType key)
	{
		return keyIndex (key) != -1;
	}

};


int main ()
{
	Dictionary<string,string> dict;

	dict.setPair ("dog","Usually smells badly");

	cout << dict.getValue ("dog") << endl;

	dict["cat"] = "Smaller than a dog";
	cout << dict["cat"] << endl;
	dict["cat"] = "Cuter than a dog";
	cout << dict["cat"] << endl;

	dict.printAll (cout);

	if (dict.hasKey ("dog"))
		cout << "Yes dog\n";
	else
		cout << "No dog\n";

}