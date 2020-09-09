#ifndef STR_HPP
#define STR_HPP
#include <vector>
#include "character.hpp"

class str
{
   std::vector<Character> string;
public:
    void addCharToStr(Character value);
    void printStr();
    Character getCharacter(int index);
    std::vector<Character> getVector();
    void addCharToStrFront(Character value);
    int getSize();
    Character popValue();
};
#endif