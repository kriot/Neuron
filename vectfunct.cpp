#include <vector>
#include <string>
using namespace std;
extern void errorf(string);

double scalar(vector<double> x,vector<double> y)
{
    double ret = 0;
    if(x.size()!=y.size())
    {
	errorf("Scalar: Vectors must be same size");
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
	errorf("Dec: Vectors must be same size");
    }
    for(int i=0;i<x.size();i++)
    {
	ret[i] = x[i]-y[i];
    }
    return ret;
}


