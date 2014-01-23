#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
using namespace std;
stringstream getbuff;
void errorf(string msg)
{
    cout<<msg<<"\n";
    exit(0);
}

void msg(string m)
{
	cout<<m<<"\n";
}
void error(string msg)
{
	cout<<msg<<"\n";
}

char getcom()
{
	char c;
	if(getbuff>>c)
		return c;
	do
	{
		cout<<'$';
		c = getchar();
	}
	while(c=='\n');
	return c;
}


template<class T>
T ask(string msg )
{
	T x;
	if(getbuff>>x)
	{
		return x;
	}
	else
	{
		cout<<msg<<" ";
		cin>>x;
		return x;
	}
}
template<class T>
void pushbuff(T s)
{
	getbuff<<" "<<s;
}
void __m()
{
	ask<int>("");
	ask<string>("");
	pushbuff<const char*>("");
	pushbuff<string>("");
	char c;
	pushbuff<char*>(&c);
}
