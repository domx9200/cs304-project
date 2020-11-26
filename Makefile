all: Character.o main.o alpha.o str.o DFA.hpp NFA.hpp reg.o
	g++ -o out Character.o main.o alpha.o str.o DFA.hpp NFA.hpp reg.o

Character.o: Character.cpp Character.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++2a

main.o: main.cpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++2a

alpha.o: alphabet.cpp alphabet.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++2a

str.o: str.cpp str.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++2a

reg.o: reg.cpp reg.hpp
	g++ -g -c -Og -Werror -ansi -Wall -pedantic -o $@ $< -std=c++2a

clean:
	rm *.o out