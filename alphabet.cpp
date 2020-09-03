#include "alphabet.hpp"
#include <iostream>

void alphabet::addCharToAlphabet(Character value)
{
    alpha.push_back(value);
}

void alphabet::printAlphabet()
{
    for(unsigned int i = 0; i < alpha.size(); i++)
    {
        std::cout << alpha.at(i).toString() << " and ";
    }
    std::cout << "no more\n";
}