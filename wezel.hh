#include <iostream>
#include <vector>
#include <memory>
#include "figura.hh"

using namespace std;


//KLASA WEZLA DRZEWA
//ZAWIERA INFORMACJE O PRZEWIDYWALNYM STANIE SZACHOWNICY
class Node
{
	int wartosc;

	//INFO O STANIE SZACHOWNICY
	vector <Figura> biale;
	vector <Figura> czarne;
	vector <vector <char>> pole;
	vector <shared_ptr <Node>> dzieci;
public:
	Node() {biale.reserve(12);czarne.reserve(12);};

	
	void tworz_dzieci(int rozmiar) ;
	void wstaw(int i,vector <Figura> b,vector <Figura> c,vector <vector <char>> f);
	void ustaw_pionki(vector <Figura> b,vector <Figura> c);
	int wez_wartosc() {return wartosc;};
	int wez_rozmiar_dzieci() {return int(dzieci.size());};
	void ustaw_wartosc(int v) {wartosc=v;};
	void ustaw_biale(vector <Figura> b) {biale=b;};
	vector <Figura> wez_wszys_biale() {return biale;};
	void ustaw_czarne(vector <Figura> c) {czarne=c;};
	vector <Figura> wez_wszys_czarne() {return czarne;};
	shared_ptr <Node> wez_potomek(int index) {return dzieci[index];};
	void ustaw_pole(vector <vector <char>> f) {pole=f;};
	vector <vector <char>> wez_pole() {return pole;};
};