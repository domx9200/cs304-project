#include "character.hpp"
#include <iostream>
#include <list>

Character::Character(std::string input)
{
    value = input;
}

std::string Character::toString()
{
    return value;
}

std::string Character::getString()
{
    return value;
}

bool Character::equals(Character comp)
{
    if(this->getString().compare(comp.getString()) == 0)
    {
        return true;
    }
    return false;
}