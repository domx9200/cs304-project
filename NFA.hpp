#ifndef NFA_HPP
#define NFA_HPP
#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>
#include "alphabet.hpp"


template <class T> class NFA{
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
    std::function<bool(T)> getQ() {return Q;};
    alphabet getSigma() {return Sigma;};
};

//task 28
template <class T> class traceTree {
    T state;
    Character transChar;
    bool isAccepted = NULL;
    bool isEps = NULL;
    std::vector<traceTree<T>> children{};
    void printTreeHelp(traceTree<T> toPrint, std::string toSpace) {
        if((int) toPrint.getChildren().size() == 0) {
            std::cout << toSpace << "[" << toPrint.getState() << ", " << (toPrint.getTransChar().toString() == "" ? "ε": toPrint.getTransChar().toString()) << ", " << (isAccepted ? "T": "F") << "]\n";
        } else {
            std::cout << toSpace << "[" << toPrint.getState() << ", " << (toPrint.getTransChar().toString() == "" ? "ε": toPrint.getTransChar().toString()) << ", " << (isAccepted ? "T": "F") << " -->\n";
            std::string newSpace = toSpace;
            newSpace.append("    ");
            for(int i = 0; i < (int) toPrint.getChildren().size(); i++) {
                printTreeHelp(toPrint.getChildren().at(i), newSpace);
            }
            std::cout << toSpace << "]\n";
        }
    };
public:
    traceTree(T state, Character transChar, bool isAccepted, std::vector<traceTree<T>> children)
        :state(state), transChar(transChar), isAccepted(isAccepted), children(children){};
    traceTree(T state, Character transChar, bool isAccepted)
        :state(state), transChar(transChar), isAccepted(isAccepted){};
    traceTree(T state, Character transChar, bool isAccepted, std::vector<traceTree<T>> children, bool isEps)
        :state(state), transChar(transChar), isAccepted(isAccepted), isEps(isEps), children(children){};
    Character getTransChar(){return transChar;};
    std::vector<traceTree<T>> getChildren(){return children;};
    T getState(){return state;};
    bool getAccepting() {return isAccepted;};
    bool getIsEps() {return isEps;};
    void printTree() {
        if((int) children.size() == 0) {
            std::cout << "[" << state << ", " << (transChar.toString() == "" ? "ε": transChar.toString()) << ", " << (isAccepted ? "T": "F") << "]\n";
        } else {
            std::cout << "[" << state << ", " << (transChar.toString() == "" ? "ε": transChar.toString()) << ", " << (isAccepted ? "T": "F") << " -->\n";
            for(int i = 0; i < (int) children.size(); i++) {
                printTreeHelp(children.at(i), "    ");
            }
            std::cout << "]\n";
        }
    };
};
#endif