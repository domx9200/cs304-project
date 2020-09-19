#include "DFA.hpp"
#include <iostream>
#include <map>

DFA::DFA() {}

DFA::DFA(std::vector<state> Q, state q0, std::vector<state> F, alphabet sigma)
{
    this->Q = Q;
    this->q0 = q0;
    this->F = F;
    this->sigma = sigma;
}

str DFA::getTrace()
{
    return trace;
}

std::vector<state> DFA::getStates()
{
    return Q;
}

std::vector<state> DFA::getAccepting()
{
    return F;
}

state DFA::getCurrent()
{
    return qi;
}

state DFA::getStart()
{
    return q0;
}

alphabet DFA::getAlpha()
{
    return sigma;
}

bool DFA::runDFA(str input)
{
    trace.clearStr();
    qi = q0;
    trace.addCharToStr(qi.getName());
    while(input.getSize() != 0)
    {
        Character temp = input.popValue();
        qi = Q.at(qi.getTransition(temp));
        trace.addCharToStr(qi.getName());
    }
    for(int i = 0; i < (int)F.size(); i++)
    {
        if(!F.at(i).getName().compare(qi.getName()))
        {
            return true;
        }
    }
    return false;
}

//task 7
DFA DFA::createCharDFA(Character toCheck)
{
    std::vector<state> states; std::vector<state> accepting; alphabet ofChar;
    ofChar.addCharToAlphabet(toCheck);
    state state1; state state2; state state3;
    std::map<std::string, int> state1T; std::map<std::string, int> state2T; std::map<std::string, int> state3T;

    //creation of state 0, with transitions to 1 if correct and 0 if incorrect
    state1T[toCheck.toString()] = 1;
    state1T["==="] = 2;
    state1.setupState(0 , "0", state1T);
    states.push_back(state1);

    //creation of state 1, which always transitions to 2 if a value is detected
    state2T["==="] = 2;
    state2.setupState(1, "1", state2T);
    states.push_back(state2);
    accepting.push_back(state2);

    //creation of state 2, which has no transitions and will always stay within itself
    state3T["==="] = 2;
    state3.setupState(2,"2", state3T);
    states.push_back(state3);

    return DFA(states, states.at(0), accepting, ofChar);
}

void DFA::createExam1_2()
{
    std::vector<state> states; std::vector<state> accepting;
    state closed; state open;
    std::map<std::string, int> stateClosedT; std::map<std::string, int> stateOpenT;
    alphabet alpha;
    alpha.addCharToAlphabet(Character("FRONT")); alpha.addCharToAlphabet(Character("BOTH"));
    alpha.addCharToAlphabet(Character("REAR")); alpha.addCharToAlphabet(Character("NEITHER"));
    stateClosedT["FRONT"] = 1; stateOpenT["NEITHER"] = 0;
    closed.setupState(0, "CLOSED", stateClosedT); open.setupState(1, "OPEN", stateOpenT);
    states.push_back(closed); states.push_back(open);
    accepting.push_back(open);
    Q = states; F = accepting; q0 = states.at(0); sigma = alpha;

}

void DFA::createExam1_4()
{
    std::vector<state> states; std::vector<state> accepting;
    state q1; state q2; state q3;
    std::map<std::string, int> q1T; std::map<std::string, int> q2T; std::map<std::string, int> q3T;
    alphabet alpha;
    alpha.addCharToAlphabet(Character("0")); alpha.addCharToAlphabet(Character("1"));
    q1T["1"] = 1; q2T["0"] = 2; q3T["==="] = 1;
    q1.setupState(0,"q1",q1T); q2.setupState(1,"q2",q2T); q3.setupState(2,"q3",q3T);
    states.push_back(q1); states.push_back(q2); states.push_back(q3);
    accepting.push_back(q2);
    Q = states; F = accepting; q0 = q1; sigma = alpha;
}

void DFA::createExam1_8()
{
    std::vector<state> states; std::vector<state> accepting;
    state q1; state q2;
    std::map<std::string, int> q1T; std::map<std::string, int> q2T;
    alphabet alpha;
    alpha.addCharToAlphabet(Character("0")); alpha.addCharToAlphabet(Character("1"));
    q1T["1"] = 1; q2T["0"] = 0;
    q1.setupState(0,"q1",q1T); q2.setupState(1,"q2",q2T);
    states.push_back(q1); states.push_back(q2);
    accepting.push_back(q2);
    Q = states; F = accepting; q0 = q1; sigma = alpha;
}

void DFA::createExam1_10()
{
    std::vector<state> states; std::vector<state> accepting;
    state q1; state q2;
    std::map<std::string, int> q1T; std::map<std::string, int> q2T;
    alphabet alpha;
    alpha.addCharToAlphabet(Character("0")); alpha.addCharToAlphabet(Character("1"));
    q1T["1"] = 1; q2T["0"] = 0;
    q1.setupState(0,"q1",q1T); q2.setupState(1,"q2",q2T);
    states.push_back(q1); states.push_back(q2);
    accepting.push_back(q1);
    Q = states; F = accepting; q0 = q1; sigma = alpha;
}

void DFA::createExam1_12()
{
    std::vector<state> states; std::vector<state> accepting;
    state S; state q1; state q2; state r1; state r2;
    std::map<std::string, int> sT; std::map<std::string, int> q1T;
    std::map<std::string, int> q2T; std::map<std::string, int> r1T;
    std::map<std::string, int> r2T;
    alphabet alpha;
    alpha.addCharToAlphabet(Character("a")); alpha.addCharToAlphabet(Character("b"));
    sT["a"] = 1; sT["b"] = 3;
    q1T["b"] = 2; q2T["a"] = 1;
    r1T["a"] = 4; r2T["b"] = 3;

    S.setupState(0,"s", sT); states.push_back(S);
    q1.setupState(1,"q1", q1T); states.push_back(q1); accepting.push_back(q1);
    q2.setupState(2,"q2", q2T); states.push_back(q2);
    r1.setupState(3,"r1", r1T); states.push_back(r1); accepting.push_back(r1);
    r2.setupState(4,"r2", r2T); states.push_back(r2);
    Q = states; F = accepting; q0 = states.at(0); sigma = alpha;
}

void DFA::createExam1_14()
{
    std::vector<state> states; std::vector<state> accepting;
    state q0; state q1; state q2;
    std::map<std::string, int> q0T; std::map<std::string, int> q1T;
    std::map<std::string, int> q2T;
    alphabet alpha;
    alpha.addCharToAlphabet(Character("RESET")); alpha.addCharToAlphabet(Character("0"));
    alpha.addCharToAlphabet(Character("1")); alpha.addCharToAlphabet(Character("2"));
    q0T["1"] = 1; q0T["2"] = 2;
    q1T["1"] = 2; q1T["0"] = 1; q1T["==="] = 0;
    q2T["2"] = 1; q2T["0"] = 2; q2T["==="] = 0;
    q0.setupState(0, "q0", q0T); states.push_back(q0); accepting.push_back(q0);
    q1.setupState(1, "q1", q1T); states.push_back(q1);
    q2.setupState(2, "q2", q2T); states.push_back(q2);
    Q = states; F = accepting; this->q0 = states.at(0); sigma = alpha;
}

void DFA::createMade1()
{
    std::vector<state> states; std::vector<state> accepting;
    state start; state h; state e; state y;
    std::map<std::string, int> hT; std::map<std::string, int> eT;
    std::map<std::string, int> yT; std::map <std::string, int> stT;
    alphabet alpha;
    alpha.addCharToAlphabet(Character("h")); alpha.addCharToAlphabet(Character("e"));
    alpha.addCharToAlphabet(Character("y"));
    stT["h"] = 1;
    hT["h"] = 1; hT["==="] = 0; hT["e"] = 2;
    eT["h"] = 1; eT["y"] = 3; eT["==="] = 0;
    start.setupState(0, "0", stT); states.push_back(start);
    h.setupState(1, "h", hT); states.push_back(h);
    e.setupState(2, "e", eT); states.push_back(e);
    y.setupState(3, "y", yT); states.push_back(y); accepting.push_back(y);
    Q = states; F = accepting; q0 = states.at(0); sigma = alpha;
}

void DFA::createMade2()
{
    std::vector<state> states; std::vector<state> accepting;
    state ZZ; state OO; state ZO; state OZ;
    std::map<std::string, int> ZZT; std::map<std::string, int> OOT;
    std::map<std::string, int> ZOT; std::map <std::string, int> OZT;
    alphabet alpha;
    alpha.addCharToAlphabet(Character("0")); alpha.addCharToAlphabet(Character("1"));
    ZZT["0"] = 2; ZZT["1"] = 1;
    OOT["0"] = 3; OOT["1"] = 0;
    ZOT["0"] = 0; ZOT["1"] = 3;
    OZT["0"] = 1; OZT["1"] = 2;
    ZZ.setupState(0, "00", ZZT); states.push_back(ZZ);
    OO.setupState(1, "11", OOT); states.push_back(OO);
    ZO.setupState(2, "01", ZOT); states.push_back(ZO);
    OZ.setupState(3, "10", OZT); states.push_back(OZ); accepting.push_back(OZ);
    Q = states; F = accepting; q0 = states.at(0); sigma = alpha;
}