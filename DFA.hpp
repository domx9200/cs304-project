#ifndef DFA_HPP
#define DFA_HPP
#include <vector>
#include "character.hpp"
#include "str.hpp"
#include "state.hpp"
#include "alphabet.hpp"

class DFA
{
   std::vector<state> Q;
   state q0;
   std::vector<state> F;
   alphabet sigma;
   state qi;
   str trace;
public:
    DFA();
    DFA(std::vector<state>, state, std::vector<state>, alphabet);
    str getTrace();
    std::vector<state> getStates();
    std::vector<state> getAccepting();
    state getCurrent();
    state getStart();
    bool runDFA(str);
    DFA createCharDFA(Character);
    void createExam1_2();
    void createExam1_4();
    void createExam1_8();
    void createExam1_10();
    void createExam1_12();
    void createExam1_14();
    void createMade1();
    void createMade2();
    alphabet getAlpha();
};
#endif