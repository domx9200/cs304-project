#include "character.hpp"
#include "alphabet.hpp"
#include "str.hpp"
#include <list>
#include <iostream>
#include <vector>

bool DFANoAccept(str input)
{
    //since no string can be accepted the bool will be false always.
    return false;
}

bool DFAEmptyOnly(str input)
{
    //all that needs to be done is to check if the input string is 0.
    if(input.getSize() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

int main()
{
    alphabet alpha;
    Character temp("0");
    Character temp2("1");
    Character temp3("2");
    alpha.addCharToAlphabet(temp);
    alpha.addCharToAlphabet(temp2);
    alpha.addCharToAlphabet(temp3);
    
    alpha.printAlphabet();
    std::cout << "\n";

    str string;
    string.addCharToStr(temp);
    string.addCharToStr(temp2);
    std::cout << "string prior to find lexo: ";
    string.printStr();
    std::cout << "\n";

    std::cout << "string after find lexo: ";
    str lexo = alpha.findNLexo(9);
    lexo.printStr();
    std::cout << "\n";
    str emptyString;

    std::cout << "output of DFANoAccept using variable string: " << DFANoAccept(string) << "\n";
    std::cout << "output of DFANoAccept using variable lexo: " << DFANoAccept(lexo) << "\n";
    std::cout << "output of DFAEmptyOnly using variable string: " << DFAEmptyOnly(string) << "\n";
    std::cout << "output of DFAEmptyOnly using variable emptystring: " << DFAEmptyOnly(emptyString) << "\n";
}