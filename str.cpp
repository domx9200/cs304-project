#include "str.hpp"
#include <iostream>

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