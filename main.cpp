#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#define sqr(x) ((x)*(x))
using namespace std;

extern void errorf(string);
template<class T>
extern T ask(string);
template<class T>
extern void pushbuff(T);
extern stringstream getbuff;
extern void load_net(string);
extern void save_net(string);
extern void make();
extern double scalar(vector<double>,vector<double>);
extern vector<double> dec(vector<double>,vector<double>);
extern void set_in_image(string);
extern bool set_educate(bool,string);
extern void teach();
extern char getcom();
extern void msg(string);
vector< vector< vector<double> > >w; 
vector< vector< double > > val;
vector< pair< vector<double>, vector<double> > > t;
double E_need;
    
double nu = 0.05;
double c = 0.1;
int isize = 30;
int show_it = 10;
double k_plus = 1.02;
double k_minus = 0.9;
int max_it =  1500000;
bool wanttotalk = true;

void setup_consts()
{
	cout<<"Speed of teaching? (def 0.1)\n";
	cin>>nu;
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
void print_help()
{
	cout<<"Inner shell commands:\n";
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

int main(int argc, char* argv[])
{
	for(int i=1;i<argc;i++)
	{
		string arg = string(argv[i]);
		if(arg=="-h")
		{
			print_help();
			exit(0);
		}
		if(arg=="-c")
		{
			for(int j=i+1;j<argc;j++)
			{
				pushbuff(argv[j]);
			}
			break;
		}
		if(arg=="-m")
			wanttotalk = false;
	}	
	char c;
	if(wanttotalk)
	{
		msg("Hi!");
    		msg("I am a neuron network. To get more information, type 'h'.");
    	}
	while(c = getcom())
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
			string file = ask<string>("Neuron net file name?");
	   		load_net(file);
		}
		if(c=='s')
		{
	    		string file = ask<string>("File to save?");
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
			string p = ask<string>("Image?");
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
			string file = ask<string>("Education file?");
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
	if(wanttotalk)
	{
    		msg("Bye!");
	}
}
