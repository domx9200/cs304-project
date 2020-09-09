#include "str.hpp"
#include <iostream>
#include <vector>

void str::addCharToStr(Character value)
{
    string.push_back(value);
}

void str::printStr()
{
    for(unsigned int i = 0; i < string.size(); i++)
    {
        std::cout << string.at(i).toString();
    }
}
//note that the character that gets returned does not share the same address as the original character
Character str::getCharacter(int index)
{
    if(index < (int) string.size() && index >= 0)
    {
        return string.at(index);
    }
    else
    {
        std::cout << "index doesn't exist. returning with NULL character\n"; 
        return Character("NULL");
    }
}

void str::addCharToStrFront(Character value)
{
    string.insert(string.begin(), value);
}

int str::getSize()
{
    return (int) string.size();
}

Character str::popValue()
{
    Character temp = string.at(0);
    string.erase(string.begin());
    return temp;
}