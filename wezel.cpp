#include "wezel.hh"


void Node::wstaw(int i,vector <Figura> b,vector <Figura> c,vector <vector <char>> f)
{
	dzieci[i]->ustaw_biale(b);
	dzieci[i]->ustaw_czarne(c);
	dzieci[i]->ustaw_pole(f);
}

void Node::ustaw_pionki(vector <Figura> b,vector <Figura> c)
{
	for(int i=0;i<int(b.size());i++)
	{
		biale.push_back(b[i]);
	}
	for(int i=0;i<int(c.size());i++)
	{
		czarne.push_back(c[i]);
	}
}

void Node::tworz_dzieci(int rozmiar)
{
	for(int i=0;i<rozmiar;i++)
		{
			dzieci.push_back(make_shared <Node>());
		}

}
