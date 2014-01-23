#include <fstream>
#include <string>
#include <vector>
using namespace std;
extern vector< vector< vector<double> > >w; 
extern vector< vector< double > > val;
 
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


