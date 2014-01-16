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
vector< pair< vector<double>, vector<double> > > t;
double E_need;
    
double nu = 0.1;
double c = 0.1;
int isize = 30;
double k_plus = 1.00001;
double k_minus = 0.8;
#define MAX_ITERATIONS 1500000

void setup_consts()
{
	cout<<"Speed of teaching? (def 0.1)\n";
	cin>>nu;
}

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
//    return (exp(x)-exp(-x))/(exp(x)+exp(-x));
//    return 1.0/(1+exp(-x*c));
	return x/(c+abs(x));
}
double f_p(double x)
{
//    return 4/(sqr(exp(x)+exp(-x)));
//    return -x*(1-x);
	return c/sqr(abs(x)+c);
}
double f_p_out(double x)
{
	return f_p(x);
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
// f for the lastest layer
    //for(int j=0;j<val.back().size();j++)
    //{
//	val.back()[j] = f(val.back()[j]);
    //}
}

void set_in()
{
    cout<<"Input IN vector ("<<val[0].size()<<")\n";
    for(int i=0;i<val[0].size();i++)
    {
	cin>>val[0][i];
    }
}

void set_in_image(string path)
{
    ifstream in((path+".bmp").c_str(), ios::in|ios::binary);
    if(!in)
    {
	cout<<"Can not open image file\n";
	return;
    }
    if(val[0].size()!=sqr(isize))
    {
	cout<<"This neuron net is not to able to load images this size ("<<isize<<")\n";
	return;
    }
    in.seekg(0,ios::end);
    int size = (int)in.tellg();
    in.seekg(0);
    cout<<"Image file size: "<<size<<"\n";
    
    in.ignore(size-sqr(isize)*3);
    int i=0;
    for(;in.good();i++)
    {
	if(i>sqr(isize))
	{
	    cout<<"Too many bytes for input\n";
	    return;
	}
	unsigned char x;
	in>>x;
	in>>x;
	in>>x;
	val[0][i]=(int)x;
    }
    in.close();
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
	    out<<"\n";
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
	q.back()[i] = f_p_out(oi)*(out[i]-oi);
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
void set_educate(bool kbd, string file)
{
    int n;
    ifstream in;

    if(!kbd)
    	in.open((file+".edu").c_str());

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
	return;
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
}
void teach()
{
    int it = 0;
	double E_prev = 100;
	int order = 1;
    double E = E_need * 2;
    for(;E_need<E;it++)
    {
	if(it>MAX_ITERATIONS*order)
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
				order+=100;
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
	if(E>E_prev)
	{
		nu*=k_minus;
	}
	else
	{
		nu*=k_plus;
	}
	E_prev = E;
	if(it%1500==0)
	{
		cout<<"Iteration: "<<it<<" E="<<E<<"\n";
	}
    }
    cout<<"Iterations: "<<it<<"\n"
	<<"E: "<<E<<"\n";
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
void print_help()
{
    cout<<"h - help\n";
    cout<<"q - exit\n";
    cout<<"l - load network form file\n";
    cout<<"c - calc(vector)\n";
    cout<<"r - reconize image (calc for image)\n";
    cout<<"m - make a new network\n";
    cout<<"t - teach the network\n";
    cout<<"e - educate from file\n";
	cout<<"x - set up consts\n";
}

int main()
{
    char c;
    cout<<"Hi!\n";
    cout<<"I am a neuron network. To get more information, type 'h'.\n";
    while(cout<<'$', cin>>c)
    {
	if(c=='q')
	{
	    break;
	}
	if(c=='h')
	{
	    print_help();
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
	if(c=='r')
	{
	    string p;
	    cout<<"Image?:\n";
	    cin>>p;
	    set_in_image(p);
	    calc();
	    print_out();
	}
	if(c=='t')
	{
	    set_educate(true,"");
	    teach();
	}
	if(c=='e')
	{
	    string file;
	    cout<<"Education file:\n";
	    cin>>file;
	    set_educate(false,file);
	    teach();
	}
	if(c=='m')
	{
	    make();
	}
	if(c=='x')
	{
		setup_consts();
	}
    }
    cout<<"Bye!\n";
}
