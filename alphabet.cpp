#include "alphabet.hpp"
#include <iostream>
#include <cmath>

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

//NULL will be the result of n being 0
str alphabet::findNLexo(int n)
{
    str temp;
    if(n == 0)
    {
        temp.addCharToStr(Character("NULL"));
        return temp;
    }
    else if(n < 0)
    {
        std::cout << "input is lower than zero, returning empty string\n";
        return temp;
    }
    int count = 0;
    int tmp = 1;
    while(tmp < n)
    {
        n -= tmp;
        count++;
        tmp = pow((double) alpha.size(), (double) count); 
    }
    temp = lexoFindHelper(n,count, temp);
    return temp;
}

str alphabet::lexoFindHelper(int n, int count, str temp)
{
    for(int i = 0; i < count; i++)
    {
        temp.addCharToStrFront(alpha.at(n % alpha.size()));
        n = n/alpha.size();
    }
    return temp;
}