#ifndef SZACHOWNICA_HH
#define SZACHOWNICA_HH

#include <vector>
#include <algorithm> 
#include <cstdlib> 
#include "pionek.hh"
#include "drzewo.hh"



//USTAWIENIA PIONKOW I WARTOSCI RUCHOW
#define ILOSC_PIONOW 12
#define RZEDY 8
#define KOLUMNY 8
#define WARTOSC_WYGRANA_B 1000
#define WARTOSC_WYGRANA_CZ -1000
#define WARTOSC_BICIE_B 30
#define WARTOSC_BICIE_CZ -30
#define WARTOSC_RUCH_B 5
#define WARTOSC_RUCH_CZ -5

using namespace std;

enum Ind {A,B,C,D,E,F,G,H};


//KLASA SZACHOWNICA ODPOWIEDZIALNA ZA WSZYSTKIE RUCHY, ZACHOWANIA BICIA
class Szachownica
{

  //INICJACJA FIGUR I STANOW NA SZACHOWNICY
  vector <vector <char>> pole;
  vector <Figura> biale;
  vector <Figura> czarne;
public:

 
  Szachownica();
  void pokaz_szachownice();

//SPRAWDZANIE CZY MOZNA WYKONAC RUCH
  bool ruch(string kolor,bool komputer,Polozenie ruch_skad,Polozenie ruch_dokad);
  bool koniec();

 //RUCH KOMPUTERA (WYKORZYSTANIE MINMAX)
  void ruch_komp();

private:

 
  bool biale_wygrana(vector <Figura> b);
  bool czarne_wygrana(vector <Figura> c);

  
  //METODA ZWRACAJACA WSZYSTKIE MOZLIWE RUCHY DLA DANEJ POZYCJI
  vector <Info> mozliwe_ruchy(string kolor,vector <Figura> b,vector <Figura> c,vector <vector <char>> znaki);

  
  //METODA SPRAWDZAJACA CZY MOZNA POPRAWNIE WYKONAC RUCH
  bool czy_mozliwy_ruszek(string kolor,Polozenie ruch_skad,Polozenie ruch_dokad,vector <vector <char>> znaki);

 
  //METODA SPRAWDZAJACA CZY MOZNA WYKONAC ZBICIE
  bool czy_wykonac_bicie(string kolor,Polozenie ruch_skad,vector <vector <char>> znaki);

  
  //METODA USUWAJACA PIONEK Z SZACHOWNICY JESLI WSZYSTKIE WARUNKI SA SPELNIONE
  void usun_przeciwnika(string kolor,Polozenie ruch_skad, Polozenie ruch_dokad);

 
  //SPRAWDZANIE CZY WYKONANY RUCH MOZNA ZALICZYC JAKO BICIE
  bool zaliczone_bicie(string kolor,Polozenie ruch_skad,Polozenie ruch_dokad,vector <vector <char>> znaki);

  //SPRAWDZANIE CZY KTORYS Z PIONKOW MOZE WYKONAC BICIE
  bool czy_bicie(string kolor,vector <Figura> b,vector <Figura> c,vector <vector <char>> znaki);

  //FUNKCJA ZWRACAJACA MOZLIWE RUCHY
  vector <Polozenie> mozliw_ruchy(string kolor,Polozenie ruch_skad,vector <vector <char>> znaki);

 //FUNKCJA ZWRACAJ¥CA MOZLIWE BICIA
  vector <Polozenie> mozliw_bicia(string kolor,Polozenie ruch_skad,vector <vector <char>> znaki);
//ALGORYTM STWORZONY NA PODSTAWIE MINMAX
  int alfabeta(PTR_NODE ptr,int glebokosc,int alfa,int beta,bool maxGracz);
 


  //AKTUALIZACJA ZNAKU NA POLU
  vector <vector <char>> aktualizuj_pole(vector <Figura> b,vector <Figura> c,vector <vector <char>> znaki);
  int daj_wart(string kolor,PTR_NODE ptr);

 
  
  void ruch_cz(PTR_NODE root,PTR_NODE ptr);
  void wyp_wezla(DecisionTree &tree,PTR_NODE ptr,string kolor,vector <Info> ruchy,vector <Figura> biale,vector <Figura> czarne,vector <vector <char>> znaki);  
  void dodaj_wezel(DecisionTree &tree,PTR_NODE ptr,string kolor,int glebokosc);
};

Ind zamiana(char c);


#endif
