all: Character.o main.o alpha.o str.o DFA.o
	g++ -o out Character.o main.o alpha.o str.o DFA.o

Character.o: Character.cpp Character.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++17

main.o: main.cpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++17

alpha.o: alphabet.cpp alphabet.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++17

str.o: str.cpp str.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++17

DFA.o: DFA.cpp DFA.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++17
clean:
	rm *.o out