all: ED.o main.o
	g++ -o ED ED.o main.o -lsfml-system

ED.o: ED.cpp ED.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++14

main.o: main.cpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++14

clean:
	rm *.o ED