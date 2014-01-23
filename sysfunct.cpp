#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
void errorf(string msg)
{
    cout<<msg<<"\n";
    exit(0);
}

char getcom()
{
	char c;
	do
	{
		cout<<'$';
		c = getchar();
	}
	while(c=='\n');
	return c;
}
