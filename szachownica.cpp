#include "szachownica.hh"


Szachownica::Szachownica()
{

	int p = 0;
	int q = 0;
  
	for(int i=0;i<RZEDY;i++)
	{
		vector <char> tmp;
		pole.push_back(tmp);
	}

	for(int i=RZEDY-1;i>=0;i--)
	{
		for(int j=0;j<KOLUMNY;j++)
		{
			//WYPE£NIANIE SZACHOWNICY ZNAKAMI '+' CO OZNACZA CZARNE POLE
			if(j%2 != 0 && i%2 != 0) pole[i].push_back('+');
			else if(j%2 == 0 && i%2 == 0) pole[i].push_back('+');
			else  
			{
				pole[i].push_back(' ');

			    //WYPE£NIANIE SZACHOWNICY PIONKAMI
				//OSTATNIE 3 RZEDY 'X' - PIONKI CZARNE
				if(i >= 5 && i <= 7)
				{
					biale.push_back(Pionek('X'));
					biale[p].ustaw_x(j);
					biale[p].ustaw_y(7-i);
					p++;
				}

				//PIERWSZE 3 RZEDY 'O' - PIONKI BIALE
				else if(i >= 0 && i <= 2)
				{
					czarne.push_back(Pionek('O'));
					czarne[q].ustaw_x(j);
					czarne[q].ustaw_y(7-i);
					q++;
				}
			}
		}
	}
}


//WYSWIETLANIE AKTUALNEGO STANU GRY W TERMINALU
void Szachownica::pokaz_szachownice()
{

	int c = 8;
	char indexes [KOLUMNY] = {'A','B','C','D','E','F','G','H'};


	for(int i=RZEDY-1;i>=0;i--)
	{
		cout << "   " ;
		for(int j=0;j<KOLUMNY;j++)
		{
			cout << "----";

		}
		cout << endl;
		cout << c ;
		c--;
		for(int j=0;j<KOLUMNY;j++)
		{

	  		//AKTUALIZUJE WYPE£NIENIE SZACHOWNICY CZARNYMI POLAMI (PO RUCHU JAKIEGOŒ PIONA)
			if(j%2 != 0 && i%2 != 0) pole[i][j] = '+';
			else if(j%2 == 0 && i%2 == 0) pole[i][j] = '+';
			else pole[i][j] = ' ';

			
			for(int p=0;p<int(biale.size());p++)
			{
				if(i == biale[p].wez_y() && j == biale[p].wez_x())
				{
					pole[i][j] = biale[p].wez_sym();	   
				}
			}

	  	
			for(int p=0;p<int(czarne.size());p++)
			{
				if(i == czarne[p].wez_y() && j == czarne[p].wez_x())
				{
					pole[i][j] = czarne[p].wez_sym();	   
				}
			}
			cout << " | " << pole[i][j];
		}
		cout << " |" << endl;

	}
	cout << "   " ;
	for(int j=0;j<KOLUMNY;j++)
	{
		cout << "----"; 
	}
	cout << endl;
	cout << " ";

  
	for(int j=0;j<KOLUMNY;j++)
	{
		cout << "   " << indexes[j];	  
	}
	cout << endl;


}

int max_glebokosc = 3;


//FUKCJA WYKONUJACA RUCHY PODCZAS GRY Z KOMPUTEREM
//WYKORZYSTANIE ALGORYTMU MINMAX - ALFABETA
void Szachownica::ruch_komp()
{

	DecisionTree drzewo;
	int ab=10000;
	int tmp;

	dodaj_wezel(drzewo,drzewo.wez_root(),"czarne",0);

	while(max_glebokosc > 0)
	{
		if(alfabeta(drzewo.wez_root(),0,-1000,1000,false) <= ab)
		{
			ab = alfabeta(drzewo.wez_root(),0,-1000,1000,false);
			tmp = max_glebokosc;
		}
		max_glebokosc--;
	}

	
	max_glebokosc = tmp;													//najlepsza opcja
	ab = alfabeta(drzewo.wez_root(),0,-1000,1000,false);

	
	for(int i=0;i<drzewo.wez_root()->wez_rozmiar_dzieci();i++)
	{
		
		if(ab == drzewo.wez_root()->wez_potomek(i)->wez_wartosc())
		{
			
			ruch_cz(drzewo.wez_root(),drzewo.wez_root()->wez_potomek(i));			//ruch komputera
			break;
		}
	}

	max_glebokosc = 3;
}





//ALGORYTM ALFABETA
int Szachownica::alfabeta(PTR_NODE ptr,int glebokosc,int alfa,int beta,bool maxGracz)
{

	int tmp=0;
	int wartosc=0;


	if(glebokosc == max_glebokosc)											//sprawdzenie czy koniec sciezki drzewa
	{
		
		if(maxGracz == true) return daj_wart("biale",ptr);
		else return daj_wart("czarne",ptr);
	} 

	
	if(maxGracz == true)                                    //dla ruchu bia³ego
	{
		wartosc = -100000;
		glebokosc++;

		for(int i=0;i<ptr->wez_rozmiar_dzieci();i++)
		{
			tmp = alfabeta(ptr->wez_potomek(i),glebokosc,alfa,beta,false);
			wartosc = max(wartosc,tmp);
			ptr->wez_potomek(i)->ustaw_wartosc(tmp);

			alfa = max(alfa,wartosc);
			if(alfa>=beta) break;
		}

		return wartosc;
	}


	else												
	{
		wartosc = 100000;
		glebokosc++;

	
		for(int i=0;i<ptr->wez_rozmiar_dzieci();i++)
		{
			
			tmp = alfabeta(ptr->wez_potomek(i), glebokosc, alfa, beta, false);
			wartosc = min(wartosc,tmp);
			ptr->wez_potomek(i)->ustaw_wartosc(tmp);

			alfa = min(beta,wartosc);
			if(alfa>=beta) break;
		}

		return wartosc;
	}
}


void Szachownica::ruch_cz(PTR_NODE root,PTR_NODE ptr)
{

	vector <Figura> b = root->wez_wszys_biale();
	vector <Figura> c = root->wez_wszys_czarne();
	Polozenie ruch_skad;
	Polozenie ruch_dokad;


	for(int i=0;i<int(c.size());i++)
	{
		if((c[i].wez_x() != ptr->wez_wszys_czarne()[i].wez_x()) && (c[i].wez_y() != ptr->wez_wszys_czarne()[i].wez_y()))
		{
			ruch_skad.ustaw_pos(c[i].wez_x(),c[i].wez_y());
			ruch_dokad.ustaw_pos(ptr->wez_wszys_czarne()[i].wez_x(),ptr->wez_wszys_czarne()[i].wez_y());
			if(ruch("czarne",true,ruch_skad,ruch_dokad) == true) 
			{
			  cout << "RUCH" << endl;
				return;
			}
		}
	}

}


//METODA ZWRACA MOZLIWE RUCHY DLA AKTUALNEJ POZYCJI PIONKA
vector <Info> Szachownica::mozliwe_ruchy(string kolor,vector <Figura> b,vector <Figura> c,vector <vector <char>> znaki)
{
	vector <Polozenie> tmp;
	Info tmp2;
	vector <Info> wektor;


	if(kolor == "biale")
	{
		if(czy_bicie("biale",b,c,znaki) == true)
		{
			for(int i=0;i<int(b.size());i++)
			{
				tmp = mozliw_bicia("biale",b[i].wez_pos(),znaki);
				for(int j = 0;j < int(tmp.size());j++)
				{
					tmp2.ruch_skad.ustaw_pos(b[i].wez_x(),b[i].wez_y());
					tmp2.ruch_dokad.ustaw_pos(tmp[j].wez_x(),tmp[j].wez_y());
					wektor.push_back(tmp2);
				}
			}
		}
	
		else
		{
			for(int i=0;i<int(b.size());i++)
			{
				tmp = mozliw_ruchy("biale",b[i].wez_pos(),znaki);
				for(int j = 0;j < int(tmp.size());j++)
				{
					tmp2.ruch_skad.ustaw_pos(b[i].wez_x(),b[i].wez_y());
					tmp2.ruch_dokad.ustaw_pos(tmp[j].wez_x(),tmp[j].wez_y());
					wektor.push_back(tmp2);
				}
			}
		}
	}
	else
	{
		if(czy_bicie("czarne",b,c,znaki) == true)
		{
			for(int i=0;i<int(c.size());i++)
			{
				tmp = mozliw_bicia("czarne",c[i].wez_pos(),znaki);
				for(int j = 0;j < int(tmp.size());j++)
				{
					tmp2.ruch_skad.ustaw_pos(c[i].wez_x(),c[i].wez_y());
					tmp2.ruch_dokad.ustaw_pos(tmp[j].wez_x(),tmp[j].wez_y());
					wektor.push_back(tmp2);
				}
			}
		}
		else
		{
		
			for(int i=0;i<int(c.size());i++)
			{
				tmp = mozliw_ruchy("czarne",c[i].wez_pos(),znaki);
				for(int j = 0;j < int(tmp.size());j++)
				{
					tmp2.ruch_skad.ustaw_pos(c[i].wez_x(),c[i].wez_y());
					tmp2.ruch_dokad.ustaw_pos(tmp[j].wez_x(),tmp[j].wez_y());
					wektor.push_back(tmp2);
				}
			}
		}
	}

	return wektor;
}


//ZWRACA WARTOSC PRZEWIDYWALNYCH RUCHOW I STANOW
int Szachownica::daj_wart(string kolor,PTR_NODE ptr)
{
	if(biale_wygrana(ptr->wez_wszys_biale()) == true) return WARTOSC_WYGRANA_B;
	else if(czarne_wygrana(ptr->wez_wszys_czarne()) == true) return WARTOSC_WYGRANA_CZ;
	else if(czy_bicie("biale",ptr->wez_wszys_biale(),ptr->wez_wszys_czarne(),ptr->wez_pole()) == true) return WARTOSC_BICIE_B;
	else if(czy_bicie("czarne",ptr->wez_wszys_biale(),ptr->wez_wszys_czarne(),ptr->wez_pole()) == true) return WARTOSC_BICIE_CZ;
	else if(kolor == "biale") return WARTOSC_RUCH_B;
	else if(kolor == "czarne") return WARTOSC_RUCH_CZ;
	
	return 0;

}


//TWORZY NOWE WEZLY W DRZEWIE I WYPELNIA JE POZYCJAMI NA SZACHOWNICY
void Szachownica::dodaj_wezel(DecisionTree &drzewo,PTR_NODE ptr,string kolor,int glebokosc)
{
	vector <Info> ruchy;

	
	if(glebokosc == max_glebokosc) return;


	if(drzewo.wez_root() == NULL)
	{
		ruchy = mozliwe_ruchy("czarne",biale,czarne,pole);

		drzewo.ustaw_root(make_shared <Node>());
		drzewo.wez_root()->ustaw_pole(pole);
		drzewo.wez_root()->ustaw_pionki(biale,czarne);

		
		ptr=drzewo.wez_root();

	
		wyp_wezla(drzewo,drzewo.wez_root(),"czarne",ruchy,biale,czarne,pole);
	}

	else
	{
		if(kolor == "biale")
		{
		
			ruchy = mozliwe_ruchy("biale",ptr->wez_wszys_biale(),ptr->wez_wszys_czarne(),ptr->wez_pole());
			wyp_wezla(drzewo,ptr,"biale",ruchy,ptr->wez_wszys_biale(),ptr->wez_wszys_czarne(),ptr->wez_pole());
		}
		else
		{
			ruchy = mozliwe_ruchy("czarne",ptr->wez_wszys_biale(),ptr->wez_wszys_czarne(),ptr->wez_pole());
			wyp_wezla(drzewo,ptr,"czarne",ruchy,ptr->wez_wszys_biale(),ptr->wez_wszys_czarne(),ptr->wez_pole());
		}
	}


	glebokosc++;

	if(kolor == "biale")
	{
		for(int i=0;i<int(ruchy.size());i++)
		{
			dodaj_wezel(drzewo,ptr->wez_potomek(i),"czarne",glebokosc);
		}
	}
	else
	{
		for(int i=0;i<int(ruchy.size());i++)
		{
			dodaj_wezel(drzewo,ptr->wez_potomek(i),"biale",glebokosc);
		}
	}
}



//WYPE£NIA WEZLY PIONKAMI I POLAMI
void Szachownica::wyp_wezla(DecisionTree &drzewo,PTR_NODE ptr,string kolor,vector <Info> ruchy,vector <Figura> b,vector <Figura> c,vector <vector <char>> znaki)
{

	vector <Figura> tmp;
	if(kolor == "biale")
	{
	
		tmp = b;

		
		ptr->tworz_dzieci(int(ruchy.size()));

	
		for(int j=0;j<int(ruchy.size());j++)
		{
			for(int i=0;i<int(b.size());i++)
			{
				
				if((b[i].wez_x() == ruchy[j].ruch_skad.wez_x()) && (b[i].wez_y() == ruchy[j].ruch_skad.wez_y()))
				{
					
					tmp[i].ustaw_pos(ruchy[j].ruch_dokad.wez_x(),ruchy[j].ruch_dokad.wez_y());
					znaki = aktualizuj_pole(tmp,c,znaki);
					ptr->wstaw(j,tmp,c,znaki);
					tmp=b;
				}
			}

		}
	}
	else
	{
		
		tmp=c;

		
		ptr->tworz_dzieci(int(ruchy.size()));

	
		for(int j=0;j<int(ruchy.size());j++)
		{
			for(int i=0;i<int(c.size());i++)
			{
				
				if((c[i].wez_x() == ruchy[j].ruch_skad.wez_x()) && (c[i].wez_y() == ruchy[j].ruch_skad.wez_y()))
				{
					
					tmp[i].ustaw_pos(ruchy[j].ruch_dokad.wez_x(),ruchy[j].ruch_dokad.wez_y());
					znaki = aktualizuj_pole(b,tmp,znaki);
					ptr->wstaw(j,b,tmp,znaki);
					tmp=c;
				}
			}

		}
	}
}



vector <vector <char>> Szachownica::aktualizuj_pole(vector <Figura> b,vector <Figura> c,vector <vector <char>> znaki)
{
	for(int i=0;i<=RZEDY-1;i++)
	{
		for(int j=0;j<KOLUMNY;j++)
		{
			if(j%2 != 0 && i%2 != 0) znaki[i][j] = '+';
			else if(j%2 == 0 && i%2 == 0) znaki[i][j] = '+';
			else znaki[i][j] = ' ';

		
			for(int p=0;p<int(b.size());p++)
			{
				if(i == b[p].wez_y() && j == b[p].wez_x())
				{
					znaki[i][j] = b[p].wez_sym();	   
				}
			}

	       
			for(int p=0;p<int(c.size());p++)
			{
				if(i == c[p].wez_y() && j == c[p].wez_x())
				{
					znaki[i][j] = c[p].wez_sym();	   
				}
			}
		}
	}

	return znaki;
}


//METODA ODPOWIEDZIALNA ZA WYKONANIE RUCHU I OPERACJE Z NIM ZWI¥ZANE
//SPRAWDZA CZY S¥ MO¯LIWE RUCHY
bool Szachownica::ruch(string kolor,bool komputer,Polozenie ruch_skad, Polozenie ruch_dokad)
{

	//KTORY GRACZ MA RUCH
	if(kolor == "biale")
	{	
		
		for(int p=0;p<int(biale.size());p++)
		{ 
			
			if(ruch_skad.wez_y() == biale[p].wez_y() && ruch_skad.wez_x() == biale[p].wez_x())
			{
				
				if(czy_wykonac_bicie("biale",ruch_skad,pole) == true)
				{
					if(zaliczone_bicie("biale",ruch_skad,ruch_dokad,pole) == false) return false;
				}
				else
				{
					if(czy_bicie("biale",biale,czarne,pole) == true) return false;
					else
					{
						if(czy_mozliwy_ruszek("biale",ruch_skad,ruch_dokad,pole) == false) return false;
					}
				}


				//JESLI DOBRZE WYKONANO RUCH TO ZMIANA WSPOLRZEDNYCH
				biale[p].ustaw_x(ruch_dokad.wez_x());
				biale[p].ustaw_y(ruch_dokad.wez_y());

				
				////GDY KTORYS Z PIONOW ZNAJDZIE SIE NA KONCU TABLICY WYBIERASZ GDZIE CHCESZ NIM RUSZYC
				if(biale[p].wez_y() == 7) 
				{
					int tmp1;
					int tmp2;
					Ind i = A;

					cout << "PION WRACA NA POCZATEK RZEDU\n";
					cout << "NA KTORE POLE W RZEDZIE 1 WYSLAC SWOJ PION: " ;
					cin >> tmp1 >> tmp2;

					i = zamiana(tmp1);
					biale[p].ustaw_x(i); biale[p].ustaw_y(tmp2);


				}

				return true;
			}
		}
	}
	else if(kolor == "czarne")
	{
	
		for(int p=0;p<int(czarne.size());p++)
		{
			if(ruch_skad.wez_y() == czarne[p].wez_y() && ruch_skad.wez_x() == czarne[p].wez_x())
			{
				if(czy_wykonac_bicie("czarne",ruch_skad,pole) == true)
				{
					if(zaliczone_bicie("czarne",ruch_skad,ruch_dokad,pole) == false) return false;
				}
				else
				{

					if(czy_bicie("czarne",biale,czarne,pole) == true) return false;
					else
					{
						if(czy_mozliwy_ruszek("czarne",ruch_skad,ruch_dokad,pole) == false) return false;
					}
				}

				//JESLI DOBRZE WYKONANO RUCH TO ZMIANA WSPOLRZEDNYCH
				czarne[p].ustaw_x(ruch_dokad.wez_x());
				czarne[p].ustaw_y(ruch_dokad.wez_y());



				if(czarne[p].wez_y() == 0) 
				{
					if(komputer == true)
					{
						
						while(true)
						{
							int wyjsc[4] = {1,3,5,7};
							int num = rand() % 4;
							int choice = wyjsc[num];
							cout << choice << endl;
							if(pole[choice][7] == '+')
							{
								czarne[p].ustaw_x(choice); czarne[p].ustaw_y(7);
								cout << czarne[p].wez_x() << " " << czarne[p].wez_y() << endl;
								break;
							}
						}
					}
					else
					{
						
						int tmp1;
						int tmp2;
						Ind i = A;

						cout << "PION WRACA NA POCZATEK RZEDU\n";
						cout << "NA KTORE POLE W RZEDZIE 8 WYSLAC SWOJ PION: ";
						cin >> tmp1 >> tmp2;

						i = zamiana(tmp1);
						czarne[p].ustaw_x(i); czarne[p].ustaw_y(tmp2-1);
					}
				}
				
				
				return true;
			}
		}
	}

	return false;
}



//CZY WYKONANY RUCH JEST POPRAWNY
bool Szachownica::czy_mozliwy_ruszek(string kolor,Polozenie ruch_skad,Polozenie ruch_dokad,vector <vector <char>> znaki)
{
	//KTORY GRACZ MA RUCH
	if(kolor == "biale")
	{
	
		if(((ruch_dokad.wez_x() == ruch_skad.wez_x() + 1 && ruch_dokad.wez_y() == ruch_skad.wez_y() + 1)
			|| (ruch_dokad.wez_x() == ruch_skad.wez_x() - 1 && ruch_dokad.wez_y() == ruch_skad.wez_y() + 1))
			&& (znaki[ruch_dokad.wez_y()][ruch_dokad.wez_x()] == '+')) 
		{
			return true;
		}
	}
	else
	{
		
		if((((ruch_dokad.wez_x() == ruch_skad.wez_x() + 1) && (ruch_dokad.wez_y() == ruch_skad.wez_y() - 1))
			|| ((ruch_dokad.wez_x() == ruch_skad.wez_x() - 1) && (ruch_dokad.wez_y() == ruch_skad.wez_y() - 1)))
			&& (znaki[ruch_dokad.wez_y()][ruch_dokad.wez_x()] == '+')) 
		{
			return true;
		}
	}

	return false;
}



//METODA USUWAJACA PIONEK Z SZACHOWNICY
void Szachownica::usun_przeciwnika(string kolor,Polozenie ruch_skad,Polozenie ruch_dokad)
{
	Polozenie pol_przeciwnika;

	if((kolor == "biale") && (ruch_dokad.wez_x() > ruch_skad.wez_x()) && (ruch_dokad.wez_y() > ruch_skad.wez_y()))
	{
		pol_przeciwnika.ustaw_x(ruch_skad.wez_x() + 1);
		pol_przeciwnika.ustaw_y(ruch_skad.wez_y() + 1);
	}
	else if((kolor == "biale") && (ruch_dokad.wez_x() < ruch_skad.wez_x()) && (ruch_dokad.wez_y() > ruch_skad.wez_y()))
	{
		pol_przeciwnika.ustaw_x(ruch_skad.wez_x() - 1);
		pol_przeciwnika.ustaw_y(ruch_skad.wez_y() + 1);
	}
	if((kolor == "biale") && (ruch_dokad.wez_x() > ruch_skad.wez_x()) && (ruch_dokad.wez_y() < ruch_skad.wez_y()))
	{
		pol_przeciwnika.ustaw_x(ruch_skad.wez_x() + 1);
		pol_przeciwnika.ustaw_y(ruch_skad.wez_y() - 1);
	}
	else if((kolor == "biale") && (ruch_dokad.wez_x() < ruch_skad.wez_x()) && (ruch_dokad.wez_y() < ruch_skad.wez_y()))
	{
		pol_przeciwnika.ustaw_x(ruch_skad.wez_x() - 1);
		pol_przeciwnika.ustaw_y(ruch_skad.wez_y() - 1);
	}
	else if((kolor == "czarne") && (ruch_dokad.wez_x() > ruch_skad.wez_x()) && (ruch_dokad.wez_y() < ruch_skad.wez_y()))
	{
		pol_przeciwnika.ustaw_x(ruch_skad.wez_x() + 1);
		pol_przeciwnika.ustaw_y(ruch_skad.wez_y() - 1);
	}
	else if((kolor == "czarne") && (ruch_dokad.wez_x() < ruch_skad.wez_x()) && (ruch_dokad.wez_y() < ruch_skad.wez_y()))
	{
		pol_przeciwnika.ustaw_x(ruch_skad.wez_x() - 1);
		pol_przeciwnika.ustaw_y(ruch_skad.wez_y() - 1);
	}
	else if((kolor == "czarne") && (ruch_dokad.wez_x() > ruch_skad.wez_x()) && (ruch_dokad.wez_y() > ruch_skad.wez_y()))
	{
		pol_przeciwnika.ustaw_x(ruch_skad.wez_x() + 1);
		pol_przeciwnika.ustaw_y(ruch_skad.wez_y() + 1);
	}
	else if((kolor == "czarne") && (ruch_dokad.wez_x() < ruch_skad.wez_x()) && (ruch_dokad.wez_y() > ruch_skad.wez_y()))
	{
		pol_przeciwnika.ustaw_x(ruch_skad.wez_x() - 1);
		pol_przeciwnika.ustaw_y(ruch_skad.wez_y() + 1);
	}

	if(kolor == "biale")
	{
		for(int p=0;p<int(czarne.size());p++)
		{
			if(pol_przeciwnika.wez_y() == czarne[p].wez_y() && pol_przeciwnika.wez_x() == czarne[p].wez_x())
			{
				czarne.erase(czarne.begin() + p);
			}
		}
	}
	else
	{
		for(int p=0;p<int(biale.size());p++)
		{
			if(pol_przeciwnika.wez_y() == biale[p].wez_y() && pol_przeciwnika.wez_x() == biale[p].wez_x())
			{
				biale.erase(biale.begin() + p);
			}
		}
	}
}


bool Szachownica::biale_wygrana(vector <Figura> b)
{
	if(int(b.size()) == 0) return true;

	return false;
}


bool Szachownica::czarne_wygrana(vector <Figura> c)
{
	if(int(c.size()) == 0) return true;

	return false;
}


bool Szachownica::koniec()
{
	if (int(biale.size()) == 0)
	{
		cout << "\nWYGRALY CZARNE\n";
		return true;
	}
	else if (int(czarne.size()) == 0)
	{	
		cout << "\nWYGRALY BIALE\n";
		return true;
	}
	return false;
}


vector <Polozenie> Szachownica::mozliw_ruchy(string kolor,Polozenie ruch_skad,vector <vector <char>> znaki)
{
	vector <Polozenie> wektor_polozenia;
	Polozenie tmp;
	if(kolor == "biale")
	{
		if((ruch_skad.wez_x() >= 1) && (ruch_skad.wez_x() <= 6))
		{
			if(znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == '+') 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+1,ruch_skad.wez_y()+1);
				wektor_polozenia.push_back(tmp);
			}
			if(znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == '+') 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-1,ruch_skad.wez_y()+1);
				wektor_polozenia.push_back(tmp);
			}
		}
		else if(ruch_skad.wez_x() == 0)
		{
			if(znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == '+') 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+1,ruch_skad.wez_y()+1);
				wektor_polozenia.push_back(tmp);
			}
		}
		else if(ruch_skad.wez_x() == 7)
		{
			if(znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == '+') 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-1,ruch_skad.wez_y()+1);
				wektor_polozenia.push_back(tmp);
			}
		}
	}
	else
	{
		if((ruch_skad.wez_x() >= 1) && (ruch_skad.wez_x() <= 6))
		{
			if(znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == '+') 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+1,ruch_skad.wez_y()-1);
				wektor_polozenia.push_back(tmp);
			}
			if(znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == '+') 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-1,ruch_skad.wez_y()-1);
				wektor_polozenia.push_back(tmp);
			}
		}
		else if(ruch_skad.wez_x() == 0)
		{
			if(znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == '+') 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+1,ruch_skad.wez_y()-1);
				wektor_polozenia.push_back(tmp);
			}
		}
		else if(ruch_skad.wez_x() == 7)
		{
			if(znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == '+') 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-1,ruch_skad.wez_y()-1);
				wektor_polozenia.push_back(tmp);
			}
		}
	}

	return wektor_polozenia;
}


//SPRWADZENIE CZY MOZNA BIC
bool Szachownica::czy_wykonac_bicie(string kolor,Polozenie ruch_skad,vector <vector <char>> znaki)
{
	if(kolor == "biale")
	{
		if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'O'))
				|| ((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'O'))
				|| ((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'O'))
				|| ((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'O')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() <= 1))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'O'))
				|| ((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'O')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() >= 6))
		{
			if(((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'O'))
				|| ((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'O')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'O'))
				|| ((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'O')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'O'))
				|| ((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'O')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() <= 1))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'O')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() >= 6))
		{
			if(((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'O')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() >= 6))
		{
			if(((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'O')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() <= 1))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'O')))
			{
				return true;
			}
		}
	}
	else
	{
		if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'X'))
				|| ((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'X'))
				|| ((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'X'))
				|| ((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'X')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() <= 1))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'X'))
				|| ((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'X')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() >= 6))
		{
			if(((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'X'))
				|| ((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'X')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if(((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'X'))
				|| ((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'X')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if(((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'X'))
				|| ((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'X')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() <= 1))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'X')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() >= 6))
		{
			if(((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'X')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() >= 6))
		{
			if(((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'X')))
			{
				return true;
			}
		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() <= 1))
		{
			if(((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'X')))
			{
				return true;
			}
		}
	}

	return false;
}


//METODA ZWRACAJACA MOZLIWE BICIA
vector <Polozenie> Szachownica::mozliw_bicia(string kolor,Polozenie ruch_skad,vector <vector <char>> znaki)
{
	vector <Polozenie> wektor_polozenia;
	Polozenie tmp;
	if(kolor == "biale")
	{
		if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'O'))
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'O'))
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
		}
		else if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() <= 1))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'O'))
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'O'))
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
			
		}
		else if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() >= 6))
		{
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
			
		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
			
		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
			
		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() <= 1))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}

		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() >= 6))
		{
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}

		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() >= 6))
		{
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}

		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() <= 1))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'O')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}

		}
	}
	else
	{
		if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
		}
		else if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() <= 1))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
		}
		else if((ruch_skad.wez_x() > 1) && (ruch_skad.wez_x() < 6) && (ruch_skad.wez_y() >= 6))
		{
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'X'))
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			} 
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'X'))
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() > 1) && (ruch_skad.wez_y() < 6))
		{
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}

		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() <= 1))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()+1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}

		}
		else if((ruch_skad.wez_x() <= 1) && (ruch_skad.wez_y() >= 6))
		{
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()+2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()+1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()+2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}

		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() >= 6))
		{
			if((znaki[ruch_skad.wez_y()-2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()-1][ruch_skad.wez_x()-1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()-2);
				wektor_polozenia.push_back(tmp);
			}

		}
		else if((ruch_skad.wez_x() >= 6) && (ruch_skad.wez_y() <= 1))
		{
			if((znaki[ruch_skad.wez_y()+2][ruch_skad.wez_x()-2] == '+') && (znaki[ruch_skad.wez_y()+1][ruch_skad.wez_x()-1] == 'X')) 
			{
				tmp.ustaw_pos(ruch_skad.wez_x()-2,ruch_skad.wez_y()+2);
				wektor_polozenia.push_back(tmp);
			}
		}
	}

	return wektor_polozenia;
}


//SPRAWDZENIE CZY DANY RUCH MOZNA ZALICZYC JAKO BICIE
bool Szachownica::zaliczone_bicie(string kolor,Polozenie ruch_skad,Polozenie ruch_dokad,vector <vector <char>> znaki)
{

	if(((ruch_dokad.wez_x() == ruch_skad.wez_x() + 2) && (ruch_dokad.wez_y() == ruch_skad.wez_y() + 2))
		|| ((ruch_dokad.wez_x() == ruch_skad.wez_x() - 2) && (ruch_dokad.wez_y() == ruch_skad.wez_y() + 2))
		|| ((ruch_dokad.wez_x() == ruch_skad.wez_x() + 2) && (ruch_dokad.wez_y() == ruch_skad.wez_y() - 2))
		|| ((ruch_dokad.wez_x() == ruch_skad.wez_x() - 2) && (ruch_dokad.wez_y() == ruch_skad.wez_y() - 2)))
	{
		Polozenie pol_przeciwnika;

		if((kolor == "biale") && (ruch_dokad.wez_x() > ruch_skad.wez_x()) && (ruch_dokad.wez_y() > ruch_skad.wez_y()))
		{
			pol_przeciwnika.ustaw_x(ruch_skad.wez_x() + 1);
			pol_przeciwnika.ustaw_y(ruch_skad.wez_y() + 1);
		}
		else if((kolor == "biale") && (ruch_dokad.wez_x() < ruch_skad.wez_x()) && (ruch_dokad.wez_y() > ruch_skad.wez_y()))
		{
			pol_przeciwnika.ustaw_x(ruch_skad.wez_x() - 1);
			pol_przeciwnika.ustaw_y(ruch_skad.wez_y() + 1);
		}
		if((kolor == "biale") && (ruch_dokad.wez_x() > ruch_skad.wez_x()) && (ruch_dokad.wez_y() < ruch_skad.wez_y()))
		{
			pol_przeciwnika.ustaw_x(ruch_skad.wez_x() + 1);
			pol_przeciwnika.ustaw_y(ruch_skad.wez_y() - 1);
		}
		else if((kolor == "biale") && (ruch_dokad.wez_x() < ruch_skad.wez_x()) && (ruch_dokad.wez_y() < ruch_skad.wez_y()))
		{
			pol_przeciwnika.ustaw_x(ruch_skad.wez_x() - 1);
			pol_przeciwnika.ustaw_y(ruch_skad.wez_y() - 1);
		}
		else if((kolor == "czarne") && (ruch_dokad.wez_x() > ruch_skad.wez_x()) && (ruch_dokad.wez_y() < ruch_skad.wez_y()))
		{
			pol_przeciwnika.ustaw_x(ruch_skad.wez_x() + 1);
			pol_przeciwnika.ustaw_y(ruch_skad.wez_y() - 1);
		}
		else if((kolor == "czarne") && (ruch_dokad.wez_x() < ruch_skad.wez_x()) && (ruch_dokad.wez_y() < ruch_skad.wez_y()))
		{
			pol_przeciwnika.ustaw_x(ruch_skad.wez_x() - 1);
			pol_przeciwnika.ustaw_y(ruch_skad.wez_y() - 1);
		}
		else if((kolor == "czarne") && (ruch_dokad.wez_x() > ruch_skad.wez_x()) && (ruch_dokad.wez_y() > ruch_skad.wez_y()))
		{
			pol_przeciwnika.ustaw_x(ruch_skad.wez_x() + 1);
			pol_przeciwnika.ustaw_y(ruch_skad.wez_y() + 1);
		}
		else if((kolor == "czarne") && (ruch_dokad.wez_x() < ruch_skad.wez_x()) && (ruch_dokad.wez_y() > ruch_skad.wez_y()))
		{
			pol_przeciwnika.ustaw_x(ruch_skad.wez_x() - 1);
			pol_przeciwnika.ustaw_y(ruch_skad.wez_y() + 1);
		}


		if(znaki[ruch_dokad.wez_y()][ruch_dokad.wez_x()] == '+')
		{
			if(kolor == "biale")
			{
				usun_przeciwnika("biale",ruch_skad,ruch_dokad);
			}	
			else
			{
				usun_przeciwnika("czarne",ruch_skad,ruch_dokad);
			}	
			return true;
		}
	}
	return false;
}	


bool Szachownica::czy_bicie(string kolor,vector <Figura> b,vector <Figura> c,vector <vector <char>> znaki)
{
	if(kolor == "biale")
	{
		
		for(int p=0;p<int(b.size());p++)
		{
			if(czy_wykonac_bicie("biale",b[p].wez_pos(),znaki) == true)
			{
				return true;
			}
		}
	}
	else
	{

		for(int p=0;p<int(c.size());p++)
		{
			if(czy_wykonac_bicie("czarne",c[p].wez_pos(),znaki) == true)
			{
				return true;
			}
		}
	}

	return false;
}


Ind zamiana(char c)
{
  Ind rtn;

  switch (c)
  {
    case 'A': rtn = A;
    break;
    
    case 'B': rtn = B;
    break;
    
    case 'C': rtn = C;
    break;

    case 'D': rtn = D;
    break;

    case 'E': rtn = E;
    break;

    case 'F': rtn = F;
    break;

    case 'G': rtn = G;
    break;

    case 'H': rtn = H;
    break;
  }
  
  return rtn;

}
