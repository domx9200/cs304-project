#include "character.hpp"
#include "alphabet.hpp"
#include "str.hpp"
#include <list>
#include <iostream>

int main()
{
    alphabet alpha;
    Character temp("0");
    Character temp2("1");
    Character temp3("2");
    alpha.addCharToAlphabet(temp);
    alpha.addCharToAlphabet(temp2);
    alpha.addCharToAlphabet(temp3);
    alpha.printAlphabet();
    std::cout << "\n";


    str string;
    string.addCharToStr(temp);
    string.addCharToStr(temp2);
    string.printStr();
    std::cout << "\n";
}