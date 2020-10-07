#ifndef DFA_HPP
#define DFA_HPP
#include <functional>
#include <vector>
#include <iostream>
#include "alphabet.hpp"


template <class T> class DFA
{
    std::function<bool(T)> Q;
    alphabet Sigma; 
    T q0; 
    std::function<T(T, Character)> Delta;
    std::function<bool(T)> F;
    std::vector<T> trace;
    std::string name;
public:
    DFA(std::function<bool(T)> Q, alphabet Sigma, T q0, std::function<T(T, Character)> Delta, std::function<bool(T)> F)
        : Q(Q), Sigma(Sigma), q0(q0), Delta(Delta), F(F){};

    bool runDFA(str input);
    alphabet getSigma(){return Sigma;};
    void printTrace();
    std::function<T(T, Character)> getDelta(){return Delta;};
    void setName(std::string n){name = n;};
    std::string getName(){return name;};
    std::function<bool(T)> getQ(){return Q;};
    std::function<bool(T)> getF(){return F;};
    T getStart(){return q0;};
    void setF(std::function<bool(T)> newF){F = newF;};
};

template <class T>
bool DFA<T>::runDFA(str input)
{
    trace.clear();
    T qi = q0;
    trace.push_back(qi);
    while(input.getSize() > 0)
    {
        Character temp = input.popValue();
        if(Q(qi)) { 
            qi = Delta(qi, temp); 
        } else { 
            std::cout << "asked for a state that doesn't exist\n";
            return false;
        }
        trace.push_back(qi);
    }
    return F(qi);
}

template <class T>
void DFA<T>::printTrace()
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
#endif