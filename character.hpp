#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <list>
#include <string>

class Character
{
    std::string value;
public:
    Character(std::string input);
    std::string toString();
    bool equals(Character compare);
    Character();
};
#endif