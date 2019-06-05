#include <iostream>
#include "menu.hh"

int main()
{
	char tmp = ' ';

	//PETLA WYBORU OPCJI: GRA 1VS 1 LUB GRACZ VS KOMPUTER
	while(tmp != 'q')
	{
		system("clear");

		pokaz_menu();
		cout << "\nTWOJ WYBOR: ";
		cin >> tmp;
		
		switch(tmp)
		{
			case 'g':
			{
				tmp = gra_1_vs_1();
			}break;

			case 'c':
			{
				tmp = gra_vs_komputer();
			}break;


		}
	}

	return 0;
}



