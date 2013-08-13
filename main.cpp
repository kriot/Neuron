#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>

#define sqr(x) ((x)*(x))
using namespace std;

void error(string msg)
{
    cout<<msg<<"\n";
    exit(0);
}

vector< vector< vector<double> > >w; 
vector< vector< double > > val;

double nu = 0.1;
double c = 0.01;

double scalar(vector<double> x,vector<double> y)
{
    double ret = 0;
    if(x.size()!=y.size())
    {
	error("Scalar: Vectors must be same size");
    }
    for(int i=0;i<x.size();i++)
    {
	ret += x[i]*y[i];
    }
    return ret;
}
vector<double> dec(vector<double> x,vector<double> y)
{
    vector<double> ret(x.size());
    if(x.size()!=y.size())
    {
	error("Dec: Vectors must be same size");
    }
    for(int i=0;i<x.size();i++)
    {
	ret[i] = x[i]-y[i];
    }
    return ret;
}


double f(double x)
{
    return (exp(x)-exp(-x))/(exp(x)+exp(-x));
    return 1.0/(1+exp(-x*c));
}
double f_p(double x)
{
    return 4/(sqr(exp(x)+exp(-x)));
    return -x*(1-x);
}
void calc()
{
    for(int i=1;i<val.size();i++)
    {
	for(int j=0;j<val[i-1].size();j++)
	{
	    val[i-1][j] = f(val[i-1][j]);
	}
	for(int j=0;j<val[i].size();j++)
	{
	    val[i][j] = scalar(w[i][j], val[i-1]); 
	}
    }
    for(int j=0;j<val.back().size();j++)
    {
	val.back()[j] = f(val.back()[j]);
    }
}

void set_in()
{
    cout<<"Input IN vector ("<<val[0].size()<<")\n";
    for(int i=0;i<val[0].size();i++)
    {
	cin>>val[0][i];
    }
}

void print_out()
{
    cout<<"OUT vector ("<<val.back().size()<<")\n";
    for(int i=0;i<val.back().size();i++)
    {
	cout<<val.back()[i]<<"\n";
    }
    cout<<"That's all\n";
}

void load_net(string file)
{
    ifstream in(file.c_str());
    int levels, lastneurons;
    in>>levels;
    in>>lastneurons;
    w.resize(levels);
    w[0].resize(lastneurons);
    val.resize(levels);
    val[0].resize(lastneurons);
    for(int i=1;i<levels;i++)
    {
	int neurons;
	in>>neurons;
	w[i].resize(neurons);
	val[i].resize(neurons);
	for(int j=0;j<neurons;j++)
	{
	    w[i][j].resize(lastneurons);
	    for(int k=0;k<lastneurons;k++)
	    {
		in>>w[i][j][k];
	    }
	}
	lastneurons = neurons;
    }
    in.close();
}
void save_net(string file)
{
    ofstream out(file.c_str());
    out<<w.size()<<"\n";
    out<<w[0].size()<<"\n";
    for(int i=1;i<w.size();i++)
    {
	out<<w[i].size()<<"\n";
	for(int j=0;j<w[i].size();j++)
	{
	    for(int k=0;k<w[i][j].size();k++)
	    {
		out<<w[i][j][k]<<" ";
	    }
	}
    }
    out.close();
}

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

void teach_iteration(vector<double> in, vector<double> out)
{
    val[0] = in;
    calc();
    double dE = calc_dE(out);
    cout<<"dE="<<dE<<"\n";
    if(dE > 1)
    {
	cout<<"OLO";
    }
    vector< vector<double> >q;
    q.resize(w.size());
    for(int i=0;i<q.size();i++)
    {
	q[i].resize(w[i].size(),0);
    }
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
}

void teach()
{
    cout<<"How many educational pairs? ";
    int n;
    cin>>n;
    vector< pair< vector<double>, vector<double> > > t(n);
    for(int i=0;i<n;i++)
    {
	cout<<"Input IN vector ("<<val[0].size()<<")\n";
	t[i].first.resize(val[0].size());
	for(int j=0;j<val[0].size();j++)
	{
	    cin>>t[i].first[j];
	}
	cout<<"Input OUT vector ("<<val.back().size()<<")\n";
	t[i].second.resize(val.back().size());
	for(int j=0;j<val.back().size();j++)
	{
	    cin>>t[i].second[j];
	}
    }
    cout<<"Iterations: ";
    int it;
    cin>>it;
    for(int i=0;i<it;i++)
    {
	for(int j=0;j<n;j++)
	{
	    teach_iteration(t[j].first,t[j].second);
	}
    }
}
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
int main()
{
    char c;
    while(cout<<'$', cin>>c)
    {
	if(c=='q')
	{
	    break;
	}
	if(c=='l')
	{
	    string file;
	    cout<<"Neuron net file name?\n";
	    cin>>file;
	    load_net(file);
	}
	if(c=='s')
	{
	    string file;
	    cout<<"File to save?\n";
	    cin>>file;
	    save_net(file);
	}
	if(c=='c')
	{
	    set_in();
	    calc();
	    print_out();
	}
	if(c=='t')
	{
	    teach();
	}
	if(c=='m')
	{
	    make();
	}
    }
    cout<<"Bye!\n";
}
