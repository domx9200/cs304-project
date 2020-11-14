#ifndef STR_HPP
#define STR_HPP
#include <vector>
#include "character.hpp"

class str
{
   std::vector<Character> string;
public:
    void addCharToStr(std::string value);
    void printStr();
    std::string getCharacter(int index);
    void addCharToStrFront(std::string value);
    int getSize();
    Character popValue();
    std::vector<Character> getString();
    void setString(std::vector<std::string>);
    std::string printable();
    void clearStr();
    void removeFront();
    int compare(str toComp);
    str(std::vector<Character> input) {string = input;};
    str(){};
};
#endif