#include <iostream>
#include <cstring>
#include <assert.h>

using namespace std;

class String
{
private:
	char *s;
	char tmp;

public:
	String ()
	{
		s = new char [1];
		s[0] = 0;
	}


	//String s5  = s2;
	String (const String &str)
	{

		//s = str.s | системен copy sonctructor
		//s -> неинициализиран
		s = new char [strlen (str.s)+1];
		strcpy (s, str.s);
	}

	//s5  = s2;
	String& operator = (const String &str)
	{

		//s = str.s | системен o-r  =
		//s -> Е инициализиран

		if (this != &str)
		{
			delete s;
			s = new char [strlen (str.s)+1];
			strcpy (s, str.s);
		}

		return *this;
	}

	//s1 = "Hello world!";
	String& operator = (const char* str)
	{

		//s -> Е инициализиран

		delete s;
		s = new char [strlen (str)+1];
		strcpy (s, str);

		return *this;
	}

	//s1 += s2
	String& operator += (const String &str)
	{
		//str.s ----> s


		char *newBuf = new char [strlen(s)+strlen(str.s)+1];
		strcpy (newBuf,s);
		strcat (newBuf,str.s);

		delete s;
		s = newBuf;

		return *this;
	}


	//s1 + s2
	//cout << "s2+s2=" << s1 + s2 <<endl;
	// cout << "..." << s1.operator + (s2) << endl;

	String operator + (const String &str)
	{
		String result = s; //constuctor (char*)
		result += str;

		return result;
	}

	//String s2 = "abc";
	String (const char *str)
	{
		//s -> неинициализиран
		s = new char [strlen (str)+1];
		strcpy (s, str);

	}

	~String ()
	{
		delete []s;
	}

	//if (s1 < s2 || s2 <= s3 || s3 == s4)
	//s1.operator < (s2)

	bool operator < (const String& s)
	{
		return strcmp (this->s,s.s) < 0;
	}

	bool operator == (const String& s)
	{
		return strcmp (this->s,s.s) == 0;
	}

	bool operator <= (const String& s)
	{
		return *this < s || *this == s;
		//return this->operator < (s) ||
		//		 this->operator == (s);
		//return strcmp (this->s,s) <= 0;

	}

	//cout << s1.lenght();
	int length ()
	{
		return strlen (s); //this->s
	}

	//cout << s[2];
	//s2[2] = 'a';
	//*s2 = 'a'


	char& operator [] (int index)
	{
		//assert (index >= 0 && index < length());

		if (index >= 0 && index < length())
			return s[index];

		//char tmp = 'X'; | НЕ - ЛОКАЛНА Е
		return tmp;

	}

	friend ostream& operator << (ostream &, const String&);
	friend istream& operator >> (istream &, String&);

};


ostream& operator << (ostream &out, const String& s)
{
	out << s.s;	//operator << (ostream&,CHAR*)
	return out;
}

//	cin >> s1;

istream& operator >> (istream &in, String& s)
{
	char *chunk = new char [10];

	s = "";

	do 
	{
		
		in.clear();
		in.getline (chunk,10);
		s += chunk;

	} while (!in);


	return in;
}


void f (String inner)
{
	cout << inner;
}

int main ()
{

	String s1; //=""
	String s2 = "abc";

	//f(s2);
	String s5  = s2;

	s5 = s2;

	cout << "s2 = ";
	cout << s2;
	cout << endl;

//	String s4 = 100; //"100"
//	String s5 = 'a';// "a"

//	cin >> s1; //!


	s1 = "Hello world!";
	cout << "s1=" << s1 << endl;

	s1 += s2;
	cout << "s1=" << s1 << endl;

	cout << "s2+s2=" << s1 + s2 <<endl;




/*	s3 -= 'a'; //remove prefix
	s3 -= "aaa";

	if (s1 || s2) //substring
		cout << "yes";

*/

	if (s1 < s2 || s2 <= s1 || s2 == s1)
		cout << "yes" << endl;	//>, >=.

	cout << s1.length() << endl;

	cout << "s1[2] = " << s1[2] << endl;
	s1[2] = 'a';

	//s2.operaotor(2) = 'a';
	//f(x) = 2

	cout << "s1=" << s1 << endl;
	//s2[2] = "abc";


	s1[500] = 'z';
	cout << "s1[500] = " << s1[500] << endl;

	
	s2 = "TEST1";
	s1 = "TTT";
	s5 = "TEST2";

	cout << "Input:";
	cin >> s1;

	cout << "s1 =" << s1 << endl;


/*
	cout << s2["hello"]; //substring index

*/
}