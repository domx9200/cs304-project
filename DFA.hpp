#ifndef DFA_HPP
#define DFA_HPP
#include <functional>
#include <vector>
#include "alphabet.hpp"

class DFA
{
    std::function<bool(int)> Q;
    alphabet Sigma; 
    int q0; 
    std::function<int(int, Character)> Delta;
    std::function<bool(int)> F;
    std::vector<int> trace;
    std::string name;
public:
    DFA(std::function<bool(int)> Q, alphabet Sigma, int q0, std::function<int(int, Character)> Delta, std::function<bool(int)> F)
        : Q(Q), Sigma(Sigma), q0(q0), Delta(Delta), F(F){};

    bool runDFA(str input);
    alphabet getSigma();
    void printTrace();
    std::function<int(int, Character)> getDelta();
    void setName(std::string n){name = n;};
    std::string getName(){return name;};
};
#endif