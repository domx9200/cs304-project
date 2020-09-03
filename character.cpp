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