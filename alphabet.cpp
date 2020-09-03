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
    std::cout << "no more";
}

Character alphabet::getCharacter(int index)
{
    if(index < (int) alpha.size() && index >= 0)
    {
        return alpha.at(index);
    }
    else
    {
        std::cout << "index doesn't exist. returning with NULL character\n"; 
        return Character("NULL");
    }
}