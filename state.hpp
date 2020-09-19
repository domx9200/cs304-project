#ifndef STATE_HPP
#define STATE_HPP
#include <vector>
#include <utility>
#include <map>
#include "character.hpp"
#include "str.hpp"

class state
{
    std::string stateName;
    std::map<std::string, int> transitions;
    int indexLocal;
public:
    state();
    state(std::map<std::string, int>, std::string name);
    int getTransition(Character);
    void addTransition(std::string, int);
    std::string getName();
    void addName(std::string);
    void addIndex(int);
    bool setupState(int index, std::string name, std::map<std::string, int>);
};
#endif