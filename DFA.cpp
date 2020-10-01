#include "DFA.hpp"
#include "str.hpp"
#include "alphabet.hpp"
#include <iostream>

bool DFA::runDFA(str input)
{
    trace.clear();
    int qi = q0;
    trace.push_back(qi);
    while(input.getSize() > 0)
    {
        Character temp = input.popValue();
        if(Q(qi)) { 
            qi = Delta(qi, temp); 
        } else { 
            std::cout << "asked for state " << qi << ", however state doesn't exist, exiting with false.\n";
            return false;
        }
        trace.push_back(qi);
    }
    return F(qi);
}

void DFA::printTrace()
{
    for(int i = 0; i < (int)trace.size(); i++)
    {
        if(i == (int)trace.size() - 1)
        {
            std::cout << trace.at(i) << "\n";
        }
        else
        {
            std::cout << trace.at(i) << ",";
        }
    }
}