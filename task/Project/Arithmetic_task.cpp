
#include "Arithmetic.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace math;

int main()
{
	string namefile;
	string line;
	cout << "Enter name file :";
	cin >> namefile;

	ifstream in(namefile);

	if (!in)
	{
		cout << "Error open file :" << namefile << endl;
		return cin.get(), 1;
	}

	Arithmetic ari;
	while (getline(in, line))
	{
		if (ari.transform(line) && ari.calculate())
		{
			cout << line << '=' << ari.answer() << endl;
		}
		else
		{
			cout << line << " error\n";
		}
	}
	
	in.close();
	return cin.get(), 0;
}

