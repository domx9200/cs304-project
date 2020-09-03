#ifndef ALPHABET_HPP
#define ALPHABET_HPP
#include <vector>
#include "character.hpp"

class alphabet
{
   std::vector<Character> alpha;
public:
    void addCharToAlphabet(Character value);
    void printAlphabet();
};
#endif