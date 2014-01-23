#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
extern vector< vector< vector<double> > >w; 
extern vector< vector< double > > val;
extern vector< pair< vector<double>, vector<double> > > t;
extern double E_need;
extern void set_in_image(string);
bool set_educate(bool kbd, string file)
{
    int n;
    ifstream in;

    if(!kbd)
	{
    	in.open((file+".edu").c_str());
	int ins, outs;
	in >> ins >> outs;
	if(ins!=val.front().size() || outs!=val.back().size())
	{
		cout<<"This edu file is other size than net.\nEdu size: "<<ins<<" "<<outs<<"\nNet size: "<<val.front().size()<<" "<<val.back().size()<<"\n";
		return false;
	}
	}

    if(kbd)
    {
	cout<<"How many educational pairs? ";
	cin>>n;
    }
    else
	in>>n;
    t.resize(n);
    char c;
    if(kbd)
    {
	cout<<"Type (v-vector, i-image): ";
	cin>>c;
    }
    else
    {
	in>>c;
    }
    if(c!='v' && c!='i')
    {
	cout<<"Type must be 'i' or 'v'.";
	return false;
    }
    for(int i=0;i<t.size();i++)
    {
	t[i].first.resize(val[0].size());
	if(c=='v')
	{
	    if(kbd)
	    {
		cout<<"Input IN vector ("<<val[0].size()<<")\n";
		for(int j=0;j<val[0].size();j++)
		{
		    cin>>t[i].first[j];
		}
	    }
	    else
	    {
		for(int j=0;j<val[0].size();j++)
		{
		    in>>t[i].first[j];
		}
	    }
	}
	if(c=='i')
	{
	    string img;
	    if(kbd)
	    {
		cout<<"Image: ";
		cin>>img;
	    }
	    else
	    {
		in>>img;
	    }
	    set_in_image(img);
	    t[i].first = vector<double>(val[0]);
	}
	//out
	t[i].second.resize(val.back().size());
	if(kbd)
	{
	    cout<<"Input OUT vector ("<<val.back().size()<<")\n";
	    for(int j=0;j<val.back().size();j++)
	    {
		cin>>t[i].second[j];
	    }
	}
	else
	{
	    for(int j=0;j<val.back().size();j++)
	    {
		in>>t[i].second[j];
	    }
	}
    }
    if(kbd)
    {
	cout<<"E: ";
	cin>>E_need;
    }
    else
    {
	in>>E_need;
    }
    if(!kbd)
	in.close();
	return true;
}

