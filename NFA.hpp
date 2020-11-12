#ifndef NFA_HPP
#define NFA_HPP
#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>
#include "alphabet.hpp"


template <class T> class NFA
{
    std::function<bool(T)> Q;
    alphabet Sigma; 
    T q0; 
    std::function<std::vector<T>(T, Character)> Delta;
    std::function<bool(T)> F;
    std::string name;
public:
    NFA(std::function<bool(T)> Q, alphabet Sigma, T q0, std::function<std::vector<T>(T, Character)> Delta, std::function<bool(T)> F)
        : Q(Q), Sigma(Sigma), q0(q0), Delta(Delta), F(F){};
    void setName(std::string name) { this->name = name;};
    std::string getName(){return name;};
    std::function<std::vector<T>(T, Character)> getDelta() {return Delta;};
    std::function<bool(T)> getF() {return F;};
    T getStart(){return q0;};
};
#endif