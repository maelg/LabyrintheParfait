Labyrinthe : main.o
	g++ main.o -o Labyrinthe -lsfml-graphics -lsfml-window -lsfml-system -std=c++11
main.o : main.cpp 
	g++ -c main.cpp -std=c++11
