#ifndef FIGURA_HH
#define FIGURA_HH

#include "polozenie.hh"

//KLASA FIGURA

class Figura
{
  
  char sym;						//znak odpowiedzialny za kolor figury

  
  Polozenie s;						//koordynaty figury
public:
  Figura() {;};
  ~Figura() {;};


  char wez_sym() {return sym;};
  void ustaw_sym(char s) {sym = s;};
  int wez_x() {return s.wez_x();};
  void ustaw_x(int xx) {s.ustaw_x(xx);};
  int wez_y() {return s.wez_y();};
  void ustaw_y(int yy) {s.ustaw_y(yy);};
  Polozenie wez_pos() {return s;};
  void ustaw_pos(int xx,int yy) {s.ustaw_y(yy); s.ustaw_x(xx);};
};

#endif
