all: Character.o main.o alpha.o str.o
	g++ -o out Character.o main.o alpha.o str.o

Character.o: Character.cpp Character.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++14

main.o: main.cpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++14

alpha.o: alphabet.cpp alphabet.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++14

str.o: str.cpp str.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++14

clean:
	rm *.o out