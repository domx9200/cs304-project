#ifndef ALPHABET_HPP
#define ALPHABET_HPP
#include <vector>
#include "character.hpp"
#include "str.hpp"

class alphabet
{
   std::vector<Character> alpha;
   str lexoFindHelper(int n, int count, str temp);
public:
    void addCharToAlphabet(Character value);
    void printAlphabet();
    Character getCharacter(int index);
    str findNLexo(int n);
};
#endif