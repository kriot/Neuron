#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;
extern vector< vector< vector<double> > >w; 
extern vector< vector< double > > val;
 
void make()
{
    int l;
    cout<<"Levels: ";
    cin>>l;
    w.resize(l);
    int ll;
    cout<<"0 level: ";
    cin>>ll;
    w[0].resize(ll);
    for(int i=1;i<l;i++)
    {
	cout<<i<<" level: ";
	int ln;
	cin>>ln;
	w[i].resize(ln);
	for(int j=0;j<ln;j++)
	{
	    w[i][j].resize(ll);
	    for(int k=0;k<ll;k++)
	    {
		w[i][j][k] = (double)(rand()%200-100)/200;
	    }
	}
	ll=ln;
    }
    cout<<"You can't use this net! It need to save and load again!\n";
}

