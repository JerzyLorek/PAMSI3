
CXXFLAGS=-g -Iinc -Wall -pedantic -std=c++0x

a.out:main.o szachownica.o wezel.o menu.o
	g++ -Wall -pedantic -o a.out  main.o szachownica.o wezel.o menu.o


main.o: main.cpp 
	g++ -c ${CXXFLAGS} -o main.o main.cpp

szachownica.o: szachownica.cpp 
	g++ -c ${CXXFLAGS} -o szachownica.o szachownica.cpp

wezel.o: wezel.cpp 
	g++ -c ${CXXFLAGS} -o wezel.o wezel.cpp

menu.o: menu.cpp 
	g++ -c ${CXXFLAGS} -o menu.o menu.cpp

clean:	

	rm *~ -f
