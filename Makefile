all: char.o main.o alpha.o
	g++ -o out char.o main.o alpha.o

char.o: Character.cpp Character.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++14

main.o: main.cpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++14

alpha.o: alphabet.cpp alphabet.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++14

clean:
	rm *.o out