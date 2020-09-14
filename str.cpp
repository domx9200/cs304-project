#include "str.hpp"
#include <iostream>
#include <vector>
#include <string>

void str::addCharToStr(std::string value)
{
    string.push_back(Character(value));
}

void str::printStr()
{
    for(unsigned int i = 0; i < string.size(); i++)
    {
        std::cout << string.at(i).toString();
    }
}
//note that the character that gets returned does not share the same address as the original character
std::string str::getCharacter(int index)
{
    if(index < (int) string.size() && index >= 0)
    {
        return string.at(index).getString();
    }
    else
    {
        std::cout << "index doesn't exist. returning with NULL character\n"; 
        return "NULL";
    }
}

void str::addCharToStrFront(std::string value)
{
    string.insert(string.begin(), Character(value));
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

std::vector<Character> str::getString()
{
    return string;
}

void str::setString(std::vector<std::string> input)
{
    string.clear();
    for(unsigned int i = 0; i < input.size(); i++)
    {
        string.push_back(Character(input.at(i)));
    }
}

std::string str::printable()
{
    std::string temp;
    for(unsigned int i = 0; i < string.size(); i++)
    {
        temp.append(string.at(i).getString());
        if(i != string.size() - 1)
        {
            temp.append(",");
        }
    }
    return temp;
}