#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#define sqr(x) ((x)*(x))
using namespace std;
extern vector<double> dec(vector<double>,vector<double>);
extern void calc();
extern double f(double);
extern double f_p(double);
extern vector< vector< vector<double> > >w; 
extern vector< vector< double > > val;
extern vector< pair< vector<double>, vector<double> > > t;
extern double E_need;
extern double nu;
extern double k_plus, k_minus;
extern int show_it;
extern int max_it;
double calc_dE(vector<double> out)
{
    double E=0;
    vector<double> d = dec(out, val.back());
    for(int i=0;i<d.size();i++)
    {
	E+=sqr(d[i]);
    }
    return E*0.5;
}

double teach_iteration(vector<double> in, vector<double> out)
{
    val[0] = in;
    calc();
    double dE = calc_dE(out);
    //cout<<"dE="<<dE<<"\n";
    vector< vector<double> >q;
    q.resize(w.size());
    for(int i=0;i<q.size();i++)
    {
	q[i].resize(w[i].size(),0);
    }
	//output layer
    for(int i=0;i<q.back().size();i++)
    {
	double oi = val.back()[i];
	q.back()[i] = f_p(oi)*(out[i]-oi);
    }


    for(int i=q.size()-2;i>0;i--)
    {
	for(int j=0;j<q[i].size();j++)
	{
	    double sum=0;
	    for(int k=0;k<q[i+1].size();k++)
	    {
		sum += q[i+1][k]*w[i+1][k][j];
	    }
	    q[i][j]= f_p(val[i][j])*sum;
	}
    }

    for(int i=1;i<w.size();i++)
    {
	for(int j=0;j<w[i].size();j++)
	{
	    for(int k=0;k<w[i][j].size();k++)
	    {
		w[i][j][k] += nu*val[i-1][k]*q[i][j];
	    }
	}
    }
    return dE;
}
void teach()
{
    long long it = 0;
	double E_prev = 100;
	int order = 1;
    double E = E_need * 2;
    for(;E_need<E;it++)
    {
	if(it>(long long)(max_it*order))
	{
	    cout<<"Two many iterations. It seems net is not able to be educated by this educational kit. Try to make E bit more or grow your network.\n";
	    cout<<"Would you like continue? (y/N)\n";
		char cont;
		cin>>cont;
		if(cont!='y')
			break;
		else
		{
			order++;
			cout<<"Do you want not to stop ever?\n";
			char notstop;
			cin>>notstop;
			if(notstop=='y')
			{
				order+=10000;
			}
		}
	}
        E = 0;
	for(int j=0;j<t.size();j++)
	{
	    double E_now = teach_iteration(t[j].first,t[j].second);
		E_now /= val.back().size();
		E_now = sqrt(E_now);
		E = max(E, E_now);
	}
	if(it%show_it==0)
	{
		if(E>E_prev)
		{
			nu*=k_minus;
		}
		else
		{
			nu*=k_plus;
		}
		E_prev = E;
	
		cout<<"Iteration: "<<it<<" E="<<E<<" nu="<<nu<<"\n";
	}
    }
    cout<<"Iterations: "<<it<<"\n"
	<<"E: "<<E<<"\n";
}

