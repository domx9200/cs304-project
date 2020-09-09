#include "character.hpp"
#include "alphabet.hpp"
#include "str.hpp"
#include <list>
#include <iostream>
#include <vector>

bool DFANoAccept(str input)
{
    std::vector<Character> states {Character("0")};
    Character current = states.at(0);
    std::vector<Character> accepts;
    while(input.getSize() != 0)
    {
        input.popValue();
    }
    for(int i = 0; i < (int) accepts.size(); i++)
    {
        if(current.equals(accepts.at(i)))
        {
            return true;
        }
    }
    return false;
}

bool DFAEmptyOnly(str input)
{
    std::vector<Character> states {Character("0"), Character("1")};
    Character current = states.at(0);
    std::vector<Character> accepts {Character("0")};
    while(input.getSize() != 0)
    {
        input.popValue();
        current = states.at(1);
    }
    for(int i = 0; i < (int) accepts.size(); i++)
    {
        if(current.equals(accepts.at(i)))
        {
            return true;
        }
    }
    return false;
}

bool DFAZeroOnly(str input)
{
    std::vector<Character> states {Character("0"), Character("1"), Character("2")};
    Character current = states.at(0);
    int currentState = 0;
    std::vector<Character> accepts {Character("1")};
    while(input.getSize() != 0)
    {
        Character temp = input.popValue();
        switch(currentState) 
        {
            case 0:
                if(temp.equals(Character("0")))
                {
                    currentState = 1;
                    current = states.at(1);
                }
                else
                {
                    currentState = 2;
                    current = states.at(2);
                } 
                break;   
            case 1:
                currentState = 2;
                current = states.at(2);
                break;
        }
    }
    for(int i = 0; i < (int) accepts.size(); i++)
    {
        if(current.equals(accepts.at(i)))
        {
            return true;
        }
    }
    return false;
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
    temp = lexo.popValue();
    std::cout << "output from popValue: " << temp.toString() << "\n";
    std::cout << "output from lexo after popValue: ";
    lexo.printStr();
    std::cout << "\n";
    temp = Character("0");
    str onlyZero;
    onlyZero.addCharToStr(temp);
    std::cout << "output of DFAZeroOnly using lexo: " << DFAZeroOnly(lexo) << "\n";
    std::cout << "output of DFAZeroOnly using onlyZero: " << DFAZeroOnly(onlyZero) << "\n";
    std::cout << "output of DFAZeroOnly using string: " << DFAZeroOnly(string) << "\n";

}