#include "state.hpp"
#include <iostream>

state::state(){}

int state::getTransition(Character input)
{
    auto it = transitions.find(input.toString());
    if(it != transitions.end())
    {
        return it->second;
    }
    input = Character("===");
    it = transitions.find(input.toString());
    if(it != transitions.end())
    {
        return it->second;
    }
    return indexLocal;
}

void state::addTransition(std::string in, int put)
{
    transitions.insert({in, put});
}

std::string state::getName()
{
    return stateName;
}

void state::addName(std::string na)
{
    stateName = na;
}

void state::addIndex(int ind)
{
    indexLocal = ind;
}

bool state::setupState(int index, std::string name, std::map<std::string, int> trans)
{
    indexLocal = index;
    stateName = name;
    transitions = trans;
    return true;
}