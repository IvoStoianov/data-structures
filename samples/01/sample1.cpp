#include <iostream>

using namespace std;


int main ()
{

	char s2[] = "xyz";
	char s1[] = "abc";

	cout << "input:";
	cin.getline (s1,3);

	char x[5];
	cin >> x;
	cout << "x="<<x<<endl;

	cout << "output:" 
	     << s2 << endl;


}