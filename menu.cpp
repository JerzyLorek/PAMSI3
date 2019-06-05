#include "menu.hh"

//GRA GRACZ vs GRACZ -  RUCH ROZPOCZYNA GRACZ BIALY
char gra_1_vs_1()
{

	
	Szachownica szachownica;	
	Ind i = A;
	char tmp1 = 'y';
	int tmp2;
	Polozenie ruch_skad;
	Polozenie ruch_dokad;

	
	while(true)
	{

		
		if(szachownica.koniec() == true) break;
		
		while(true)
		{
			system("clear");
			szachownica.pokaz_szachownice();
			
			cout << "GRACZ BIALY (X) RUCH Z POLA (przyklad A1):  ";
			cin >> tmp1 >> tmp2 ;
			
			if(tmp1 == 'q') break;
			
			i = zamiana(tmp1);
			ruch_skad.ustaw_x(i);
			ruch_skad.ustaw_y(tmp2 - 1);
			
			cout << "GRACZ BIALY (X) RUCH NA POLE (przyklad B2): " ;
			cin >> tmp1 >> tmp2 ;

			if(tmp1 == 'q') break;
			
			i = zamiana(tmp1);
			ruch_dokad.ustaw_x(i);
			ruch_dokad.ustaw_y(tmp2 - 1);
			
			
			if(szachownica.ruch("biale",false,ruch_skad,ruch_dokad) == true) break;
		}

	
		if(szachownica.koniec() == true) break;
		
		while(true)
		{
			system("clear");
			szachownica.pokaz_szachownice();
			
			cout << "GRACZ CZARNY (O) RUCH Z POLA (przyklad A1): ";
			cin >> tmp1 >> tmp2 ;
			
			if(tmp1 == 'q') break;
			
			i = zamiana(tmp1);
			ruch_skad.ustaw_x(i);
			ruch_skad.ustaw_y(tmp2 - 1);
			cout << "GRACZ CZARNY (O) RUCH NA POLE (przyklad B2): " ;
			cin >> tmp1 >> tmp2 ;

			if(tmp1 == 'q') break;
			
			i = zamiana(tmp1);
			ruch_dokad.ustaw_x(i);
			ruch_dokad.ustaw_y(tmp2 - 1);

			
			if(szachownica.ruch("czarne",false,ruch_skad,ruch_dokad) == true) break;
		}
		
		
	}
	return 'q';
}

//GRA KOMPUTER vs GRACZ - GRACZ PORUSZA SIE PIONAMI BIALYMI
char gra_vs_komputer()
{

	Szachownica szachownica;	
	Ind i = A;
	char tmp1 = 'y';
	int tmp2;
	Polozenie ruch_skad;
	Polozenie ruch_dokad;

	
	while(true)
	{

		if(szachownica.koniec() == true) break;

		while(true)
		{
			system("clear");
			szachownica.pokaz_szachownice();
			
			cout << "GRACZ BIALY (X) RUCH Z POLA (przyklad A1): ";
			cin >> tmp1 >> tmp2 ;
			
			if(tmp1 == 'q') break;
			
			i = zamiana(tmp1);
			ruch_skad.ustaw_x(i);
			ruch_skad.ustaw_y(tmp2 - 1);
			
			cout << "GRACZ BIALY (X) RUCH NA POLE (przyklad B2): " ;
			cin >> tmp1 >> tmp2 ;

			if(tmp1 == 'q') break;
			
			i = zamiana(tmp1);
			ruch_dokad.ustaw_x(i);
			ruch_dokad.ustaw_y(tmp2 - 1);
			
			
			usleep(200000);

			if(szachownica.ruch("biale",false,ruch_skad,ruch_dokad) == true) break;
		}

		
		if(szachownica.koniec() == true) break;
		system("clear");
		szachownica.pokaz_szachownice();
		usleep(200000);
		szachownica.ruch_komp();
		
		
	}
	return 'q';
}








void pokaz_menu()
{
    cout << "WYBIERZ OPCJE: \n" << endl;
	cout << "q - WYJSCIE" << endl;
	cout << "g - GRACZ vs GRACZ" << endl;
	cout << "c - GRACZ vs KOMPUTER" << endl;


}
