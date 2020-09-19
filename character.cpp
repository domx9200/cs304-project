#include "character.hpp"
#include <iostream>
#include <list>

Character::Character(std::string input)
{
    value = input;
}

Character::Character(){}

std::string Character::toString()
{
    return value;
}

bool Character::equals(Character comp)
{
    if(this->toString().compare(comp.toString()) == 0)
    {
        return true;
    }
    return false;
}