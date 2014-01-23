#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define sqr(x) ((x)*(x))
using namespace std;
extern vector< vector< vector<double> > >w; 
extern vector< vector< double > > val;
extern int isize; 

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
	unsigned char r,g,b;
	in>>r;
	in>>g;
	in>>b;
	val[0][i]=((int)r+(int)g+(int)b)/3;
    }
    in.close();
}


