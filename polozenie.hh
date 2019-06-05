#ifndef POLOZENIE_HH
#define POLOZENIE_HH

#include <iostream>

using namespace std;

//KLASA MIEJSCA PO£O¯ENIA FIGURY
class Polozenie
{
  int x;
  int y;
public:
  Polozenie() {;};

 
  int wez_x() {return x;};
  int wez_y() {return y;};
  void ustaw_x(int xx) {x=xx;};
  void ustaw_y(int yy) {y=yy;};
  void ustaw_pos(int xx,int yy) {x=xx;y=yy;};
};

//struktura zawieraj¹ca informacje o dwóch koordynatach. Miejsce po³o¿enia i miejsce docelowe
struct Info
{
  Polozenie ruch_skad;
  Polozenie ruch_dokad;
};

#endif

 
