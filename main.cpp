#include "character.hpp"
#include "alphabet.hpp"
#include "str.hpp"
#include "DFA.hpp"
#include "NFA.hpp"
#include "reg.hpp"
#include <list>
#include <iostream>
#include <vector>
#include <optional>
#include <utility>
#include <algorithm>
#include <typeinfo>
#include <random>
#include <unordered_set>

template <typename T>
void testDFA(DFA<T> toTest)
{
    std::cout << "printing output of DFA " << toTest.getName() << "\n";
    for(int i = 0; i < 40; i++)
    {
        std::cout << "using string " << toTest.getSigma().findNLexo(i).printable() << ": " << toTest.runDFA(toTest.getSigma().findNLexo(i)) << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";
}

//task 7
DFA<int> genOneChar(Character toUse)
{
    alphabet temp;
    temp.addCharToAlphabet(toUse);
    temp.addCharToAlphabet(Character("---"));
    DFA<int> output([](int state){return state == 0 || state == 1 || state == 2;}, temp, 0, [toUse](int state, Character c){
        switch(state) {
            case(0):
                if(c.equals(toUse))
                {
                    return 1;
                }
                else
                {
                    return 2;
                }
                break;
            case(1):
                return 2;
                break;
            case(2):
                return 2;
                break;
        }
        return -1;
    }, [](int state){return state == 1;});
    return output;
}

//task 10
template <class T>
bool detDFA(DFA<T> toRun, str input)
{
    return toRun.runDFA(input);
}

//task 11
template <class T>
void getTrace(DFA<T> toRun, str input)
{
    toRun.runDFA(input);
    std::cout << "printing trace: ";
    toRun.printTrace();
}

//task 12
template <typename T>
str task12Helper(std::function<T(T, Character)> transitions, std::function<bool(T)> F, T qi, std::vector<Character> sigma, std::vector<T> *visited, Character c)
{
    str output;
    if(F(qi))
    {
        output.addCharToStrFront(c.toString());
        return output;
    }

    for(int i = 0; i < (int) sigma.size(); i++)
    {
        auto qNext = transitions(qi, sigma.at(i));
        auto it = std::find(visited->begin(), visited->end(), qNext);
        if(it == visited->end())
        {
            visited->push_back(qNext);
            output = task12Helper(transitions, F, qNext, sigma, visited, sigma.at(i));
            if(output.getSize() > 0)
            {
                output.addCharToStrFront(c.toString());
                return output;
            }
        }
    }
    return output;
}

template <typename T>
std::optional<str> task12(DFA<T> toFind)
{
    auto transitions = toFind.getDelta();
    auto F = toFind.getF();
    T q0 = toFind.getStart();
    std::vector<Character> sigma = toFind.getSigma().getVec();
    std::vector<T> visited {q0};
    str output;

    if(F(q0)){ return output; }

    for(int i = 0; i < (int) sigma.size(); i++) {
        T qi = transitions(q0, sigma.at(i));
        auto it = std::find(visited.begin(), visited.end(), qi);
        if(it == visited.end()) {
            visited.push_back(qi);
            output = task12Helper(transitions, F, qi, sigma, &visited, sigma.at(i));
            if(toFind.runDFA(output)) {
                return output;
            }
        }
    }
    return std::nullopt;
}

//task 13
template <typename T>
DFA<T> complementDFA(DFA<T> toComplement)
{
    auto origF = toComplement.getF();
    std::function<bool(T)> newF;
    newF = [origF](T state){ return !origF(state); };
    DFA output = toComplement;
    output.setF(newF);
    output.setName("complement of " + output.getName());
    return output;
}

//does both union and intersect as the difference between the two is the || and the && in the new F function
template <typename T, typename F>
DFA<std::pair<T,F>> unin(DFA<T> DFA1, DFA<F> DFA2, bool isunion = true){

    std::function<bool(T)> Q1 = DFA1.getQ();
    std::function<bool(F)> Q2 = DFA2.getQ();
    std::function<T(T, Character)> D1 = DFA1.getDelta();
    std::function<F(F, Character)> D2 = DFA2.getDelta();
    std::function<bool(T)> F1 = DFA1.getF();
    std::function<bool(F)> F2 = DFA2.getF();

    if(isunion){
        DFA<std::pair<T,F>> output([Q1, Q2](std::pair<T,F> state){ return Q1(state.first) && Q2(state.second); }, 
            DFA1.getSigma(), std::pair<T,F>{DFA1.getStart(), DFA2.getStart()}, 
            [D1, D2](std::pair<T,F> state, Character c){
                T q1 = D1(state.first, c);
                F q2 = D2(state.second, c);
                return std::pair<T,F>{q1,q2};
            }, [F1,F2](std::pair<T,F> state){return F1(state.first) || F2(state.second);});
        output.setName("(Union of " + DFA1.getName() + " and " + DFA2.getName() + ")");
    return output; 
    }
    DFA<std::pair<T,F>> output([Q1, Q2](std::pair<T,F> state){ return Q1(state.first) && Q2(state.second); }, 
        DFA1.getSigma(), std::pair<T,F>{DFA1.getStart(), DFA2.getStart()}, 
        [D1, D2](std::pair<T,F> state, Character c){
           T q1 = D1(state.first, c);
           F q2 = D2(state.second, c);
           return std::pair<T,F>{q1,q2};
        }, [F1,F2](std::pair<T,F> state){return F1(state.first) && F2(state.second);});
    output.setName("(Intersect of " + DFA1.getName() + " and " + DFA2.getName() + ")");
    return output;
}

//task 14
template <typename T, typename F>
DFA<std::pair<T,F>> unionDFA(DFA<T> DFA1, DFA<F> DFA2){

    return unin(DFA1, DFA2);
}

//task 16
template <typename T, typename F>
DFA<std::pair<T,F>> intersectDFA(DFA<T> DFA1, DFA<F> DFA2){

    return unin(DFA1, DFA2, false);
}

//task 18
template <typename T, typename F>
bool subset(DFA<T> X, DFA<F> Y) {
    return !task12(intersectDFA(complementDFA(Y), X)).has_value();
}

//task 20
template<typename T, typename F>
bool equals(DFA<T> X, DFA<F> Y) {
    return subset(X, Y) && subset(Y, X);
}

//task 24
template<typename T>
NFA<T> convertDFA(DFA<T> input) {
    auto dDelta = input.getDelta();
    auto newDelta = [dDelta](T state, Character c){
        std::vector<T> out;
        if(!c.equals(Character(""))) {
            out.push_back(dDelta(state, c));
        }
        return out;
    };
    NFA<T> output(input.getQ(), input.getSigma(), input.getStart(), newDelta, input.getF());
    output.setName("converted from DFA " + input.getName());
    return output;
}


//task 27
template<typename T>
bool oracle(NFA<T> input, str check, std::vector<std::pair<T, bool>> trace, bool isTrue, T currentState) {
    //this cannot happen in a correct string as the trace will always be
    //equal or larger than the amount of characters.
    if((int)trace.size() < check.getSize()) {
        return false;
    }

    if(trace.empty()) {
        if(check.getSize() == 0) {
            if(input.getF()(currentState) == isTrue)
                return true;
        }
    return false;
    }

    //if it is expecting an epsilon transition here then it will only run this,
    //if it doesn't find what it's looking for then something is wrong.
    if(std::get<1>(trace.at(0))) {
        //calls the delta function with epsilon
        std::vector<T> temp = input.getDelta()(currentState, Character(""));
        //runs through the vector the delta function returns, if empty it just skips this
        for(int i = 0; i < (int)temp.size(); i++) {
            if(std::get<0>(trace.at(0)) == temp.at(i)) {
                trace.erase(trace.begin());
                return oracle(input, check, trace, isTrue, temp.at(i));
            }
        }
    }

    //will only check this if it isn't expecting an epsilon transition
    if(!std::get<1>(trace.at(0))){
        std::vector<T> temp = input.getDelta()(currentState, check.getString().at(0));
        //again if empty it just doesn't run
        for(int i = 0; i < (int)temp.size(); i++) {
            if(std::get<0>(trace.at(0)) == temp.at(i)) {
                trace.erase(trace.begin());
                check.removeFront();
                return oracle(input, check, trace, isTrue, temp.at(i));
            }
        }
    }
    return false;
}

template<typename T>
traceTree<T> forkingHelp(NFA<T> input, str inStr, T qi, bool wasEps = false) {
    std::vector<traceTree<T>> children;
    std::vector<T> epsilonOutput = input.getDelta()(qi, Character(""));
    for(int i = 0; i < (int) epsilonOutput.size(); i++) {children.push_back(forkingHelp(input, inStr, epsilonOutput.at(i), true));}

    //end if it reaches epsilon/empty
    if(inStr.getSize() > 0) {
        Character currentChar(inStr.getCharacter(0));
        std::vector<T> charOutput = input.getDelta()(qi, currentChar);
        inStr.removeFront();
        for(int i = 0; i < (int) charOutput.size(); i++) {children.push_back(forkingHelp(input, inStr, charOutput.at(i), false));}
        return traceTree<T>(qi, currentChar, input.getF()(qi), children, wasEps);
    }
    return traceTree<T>(qi, Character(""), input.getF()(qi), children, wasEps);
}

//task 30
template<typename T>
traceTree<T> forking(NFA<T> input, str inStr) {
    return forkingHelp(input, inStr, input.getStart());
}

//task 32
template<typename T>
bool backTracking(NFA<T> input, str inStr) {
    std::vector<std::pair<T, str>> visited;
    std::vector<std::pair<T, str>> pending = {std::pair<T, str>{input.getStart(), inStr}};
    while(pending.size() != 0){
        std::pair<T, str> temp = pending.at(0);
        pending.erase(pending.begin());
        if(temp.second.getSize() == 0 && input.getF()(temp.first)){return true;}
        std::vector<T> epsOut = input.getDelta()(temp.first, Character(""));
        for(int i = 0; i < (int) epsOut.size(); i++) {
            bool isInV = false;
            for(int j = 0; j < (int) visited.size(); j++) {
                if(visited.at(j).first == epsOut.at(i) && visited.at(j).second.compare(temp.second) == 0) {
                    isInV = true;
                }
            }
            if(!isInV){
                std::pair<T, str> test(epsOut.at(i), temp.second);
                visited.push_back(test);
                bool isInP = false;
                for(int j = 0; j < (int) pending.size(); j++) {
                    if(pending.at(j).first == epsOut.at(i) && pending.at(j).second.compare(temp.second) == 0) {
                        isInP = true;
                    }
                }
                if(!isInP)
                    pending.push_back(test);
            }
        }
        if(temp.second.getSize() > 0) {
            std::vector<T> charOut = input.getDelta()(temp.first, Character(temp.second.getCharacter(0)));
            temp.second.removeFront();
            for(int i = 0; i < (int) charOut.size(); i++) {
                bool isInV = false;
                for(int j = 0; j < (int) visited.size(); j++) {
                    if(visited.at(j).first == charOut.at(i) && visited.at(j).second.compare(temp.second) == 0) {
                        isInV = true;
                    }
                }
                if(!isInV){
                    std::pair<T, str> test(charOut.at(i), temp.second);
                    visited.push_back(test);
                    bool isInP = false;
                    for(int j = 0; j < (int) pending.size(); j++) {
                        if(pending.at(j).first == charOut.at(i) && pending.at(j).second.compare(temp.second) == 0) {
                            isInP = true;
                        }
                    }
                    if(!isInP)
                        pending.push_back(test);
                }
            } 
        }
    }
    return false;
}

//task 33
template<typename T, typename F>
NFA<std::pair<int, std::pair<std::optional<T>, std::optional<F>>>> unionNFA(NFA<T> nfa1, NFA<F> nfa2) {
    std::pair<int, std::pair<std::optional<T>, std::optional<F>>> start = {0, {std::nullopt, std::nullopt}};
    auto Q1 = nfa1.getQ();
    auto Q2 = nfa2.getQ();
    //as detailed in video #6
    auto newQ = [=](auto state){
        if(state == start) {
            return true;
        }
        if(state.first == 1 && state.second.first.has_value()) {
            return Q1(state.second.first.value());
        }
        if(state.first == 2 && state.second.second.has_value()) {
            return Q2(state.second.second.value());
        }
        return false;
    };
    T Qs1 = nfa1.getStart();
    F Qs2 = nfa2.getStart();
    auto D1 = nfa1.getDelta();
    auto D2 = nfa2.getDelta();
    //as detailed in video #6
    auto newDelta = [=](auto state, Character c){
        std::vector<std::pair<int , std::pair<std::optional<T>, std::optional<F>>>> output;
        if(state == start && c.equals(Character(""))) {
            output.push_back({1, {Qs1, std::nullopt}});
            output.push_back({2, {std::nullopt, Qs2}});
        }
        if(state.first == 1 && state.second.first.has_value()) {
            std::vector<T> temp = D1(state.second.first.value(), c);
            for(int i = 0; i < (int) temp.size(); i++) {
                output.push_back({1, {temp.at(i), std::nullopt}});
            }
        }
        if(state.first == 2 && state.second.second.has_value()) {
            std::vector<F> temp = D2(state.second.second.value(), c);
            for(int i = 0; i < (int) temp.size(); i++) {
                output.push_back({2, {std::nullopt, temp.at(i)}});
            }
        }
        return output;
    };

    auto F1 = nfa1.getF();
    auto F2 = nfa2.getF();
    //slightly modified. because you only wanted one new state created
    //I opted to just return true if either of the inputs return true at a given point.
    auto newF = [=](auto state) {
        if(state.first == 1 && state.second.first.has_value()) {
            return F1(state.second.first.value());
        }
        if(state.first == 2 && state.second.second.has_value()) {
            return F2(state.second.second.value());
        }
        return false;
    };
    NFA<std::pair<int, std::pair<std::optional<T>, std::optional<F>>>> out(newQ, nfa1.getSigma(), 
                                                                              start, newDelta, newF);
    out.setName("(union of " + nfa1.getName() + " and " + nfa2.getName() + ")");
    return out;
}

//task 34
template<typename T, typename F>
NFA<std::pair<int, std::pair<std::optional<T>, std::optional<F>>>> concNFA(NFA<T> nfa1, NFA<F> nfa2){
    std::pair<int, std::pair<std::optional<T>, std::optional<F>>> start = {0, {nfa1.getStart(), std::nullopt}};
    auto Q1 = nfa1.getQ();
    auto Q2 = nfa2.getQ();

    auto newQ = [=](auto state){
        if(state.first == 0 && state.second.first.has_value()) {
            return Q1(state.second.first.value());
        }
        if(state.first == 1 && state.second.second.has_value()) {
            return Q2(state.second.second.value());
        }
        return false;
    };

    auto Qs2 = nfa2.getStart();
    auto D1 = nfa1.getDelta();
    auto D2 = nfa2.getDelta();
    auto F1 = nfa1.getF();
    auto newDelta = [=](auto state, Character c){
        std::vector<std::pair<int , std::pair<std::optional<T>, std::optional<F>>>> output;
        if(state.first == 0 && state.second.first.has_value()) {
            std::vector<T> temp = D1(state.second.first.value(), c);
            for(int i = 0; i < (int) temp.size(); i++) {
                output.push_back({0, {temp.at(i), std::nullopt}});
            }
            if(F1(state.second.first.value()) && c.equals(Character(""))) {
                output.push_back({1, {std::nullopt, Qs2}});
            }
        }
        if(state.first == 1 && state.second.second.has_value()) {
            std::vector<F> temp = D2(state.second.second.value(), c);
            for(int i = 0; i < (int) temp.size(); i++) {
                output.push_back({1, {std::nullopt, temp.at(i)}});
            }
        }
        return output;
    };

    auto F2 = nfa2.getF();
    auto newF = [=](auto state){
        if(state.first == 1 && state.second.second.has_value()) {
            return F2(state.second.second.value());
        }
        return false;
    };
    NFA<std::pair<int, std::pair<std::optional<T>, std::optional<F>>>> out(newQ, nfa1.getSigma(), 
                                                                              start, newDelta, newF);
    out.setName("(concatenation of " + nfa1.getName() + " and " + nfa2.getName() + ")");
    return out;
}

//task 36
template<typename T>
NFA<std::pair<int, std::optional<T>>> kleeneStar(NFA<T> input) {
    std::pair<int, std::optional<T>> newStart = {-1, std::nullopt};
    auto oldQ = input.getQ();
    auto newQ = [=](std::pair<int, std::optional<T>> state){
        if(state.first == -1)
            return true;
        else if(state.first == 0 && state.second.has_value())  
            return oldQ(state.second.value());
        return false;
    };

    auto oldDelta = input.getDelta();
    std::function<bool(T)> oldF = input.getF();
    T oldStart = input.getStart();
    auto newDelta = [=](std::pair<int, std::optional<T>> state, Character c){
        std::vector<std::pair<int, std::optional<T>>> output;
        if(state.first == -1 && c.equals(Character("")))
            output.push_back({0, oldStart});
        if(state.first == 0 && state.second.has_value()) {
            std::vector<T> temp = oldDelta(state.second.value(), c);
            for(int i = 0; i < (int) temp.size(); i++)
                output.push_back({0, temp.at(i)});
        }
        if(state.first == 0 && c.equals(Character("")) && state.second.has_value())
            if(oldF(state.second.value()))
                output.push_back(newStart);
        return output;
    };

    auto newF = [=](std::pair<int, std::optional<T>> state){return state.first == -1;};

    NFA<std::pair<int,std::optional<T>>> out(newQ, input.getSigma(), newStart, newDelta, newF);
    out.setName("(Kleene star of " + input.getName() + ")");
    return out;
}

template<typename T>
DFA<std::vector<T>> compile(NFA<T> input){
    auto Qn = input.getQ();
    auto Qd = [=](std::vector<T> state){
        std::vector<T> found;
        for(int i = 0; i < (int)state.size(); i++){
            if(Qn(state.at(i))){
                for(int j = 0; j < (int)found.size(); j++){
                    if(state.at(i) == found.at(j))
                        return false;
                }
                found.push_back(state.at(i));
            } else
                return false;
        }
        return true;
    };

    auto Dn = input.getDelta();
    auto E = [=](std::vector<T> input){
        bool changed = true;
        while(changed){
            changed = false;
            for(int i = 0; i < (int) input.size(); i++){
                std::vector<T> temp = Dn(input.at(i), Character(""));
                for(int j = 0; j < (int) temp.size(); j++)
                    if(std::find(input.begin(), input.end(), temp.at(j)) == input.end()){
                        changed = true;
                        input.push_back(temp.at(j));
                    }
            }
        }
        return input;
    };

    std::vector<T> startState = {input.getStart()};
    startState = E(startState);

    auto Fn = input.getF();
    auto Fd = [=](std::vector<T> state){
        for(int i = 0; i < (int) state.size(); i++) 
            if(Fn(state.at(i)))
                return true;
        return false;
    };

    auto Dd = [=](std::vector<T> state, Character c){
        std::vector<T> check;
        for(int i = 0; i < (int) state.size(); i++){
            std::vector<T> temp = Dn(state.at(i), c);
            check.insert(check.end(), temp.begin(), temp.end());
        }

        for(int i = 0; i < (int) check.size(); i++){
            for(int j = 0; j < (int) check.size(); j++){
                if(check.at(i) == check.at(j) && i != j){
                    check.erase(check.begin() + j);
                }
            }
        }
        return E(check);
    };
    DFA<std::vector<T>> newDFA(Qd, input.getSigma(), startState, Dd, Fd);
    newDFA.setName("DFA created from " + input.getName());
    return newDFA;
}

int main(){
    //Task 1-3
    alphabet alpha;
    alpha.addCharToAlphabet(Character("0"));
    alpha.addCharToAlphabet(Character("1"));
    
    alpha.printAlphabet();
    std::cout << "\n";

    str string;
    string.addCharToStr("0");
    string.addCharToStr("1");
    std::cout << "string prior to find lexo: " << string.printable() << "\n";
    std::cout << "string after find lexo: " << alpha.findNLexo(9).printable() << "\n-----------------------------------------------------------------------------------\n";
    
    //task 5
    DFA<int> noAccept([](int state) { return state == 0; }, alpha, 0, 
                 [](int state, Character c){ return 0; }, [](int state){ return state == 1; });
    noAccept.setName("noAccept");

    //task 6
    DFA<int> emptyOnly([](int state){return state == 0 || state == 1;}, alpha, 0, 
                  [](int state, Character c){ return 1; }, [](int state){return state == 0;});
    emptyOnly.setName("emptyOnly");

    //task 7
    DFA<int> CharGen = genOneChar(Character("D"));
    CharGen.setName("CharGen");

    //task 8 example 1_2
    //this task was modified slightly to allow for an entry point and an accepting state.
    alphabet ex1_2;
    ex1_2.addCharToAlphabet(Character("FRONT"));
    ex1_2.addCharToAlphabet(Character("REAR"));
    ex1_2.addCharToAlphabet(Character("BOTH"));
    ex1_2.addCharToAlphabet(Character("NEITHER"));
    DFA<int> example1_2([](int state){return state == 0 || state == 1;}, ex1_2, 0, [](int state, Character c){
        switch(state){
            case 0:
                if(c.equals(Character("FRONT"))){return 1;}
                return 0;
            case 1:
                if(c.equals(Character("NEITHER"))){return 0;}
                return 1;
        }
        return -1;
    }, [](int state){return state == 1;});
    example1_2.setName("example1_2");

    //task 8, example1_4
    DFA<int> example1_4([](int state){return state == 0 || state == 1 || state == 2;}, alpha, 0, [](int state, Character c){
        switch(state){
            case 0:
                if(c.equals(Character("1"))){ return 1;}
                return 0;
            case 1:
                if(c.equals(Character("0"))){return 2;}
                return 1;
            case 2:
                return 1;
        }
        return -1;
    }, [](int state){return state == 1;});
    example1_4.setName("example1_4");

    //task 8, example 1_8
    DFA<int> example1_8([](int state){return state == 0 || state == 1;}, alpha, 0, [](int state, Character c){
        switch(state){
            case 0:
                if(c.equals(Character("1"))){return 1;}
                return 0;
            case 1:
                if(c.equals(Character("0"))){return 0;}
                return 1;
        }
        return -1;
    }, [](int state){return state == 1;});
    example1_8.setName("example1_8");

    //task 8, example 1_10
    DFA<int> example1_10([](int state){return state == 0 || state == 1;}, alpha, 0, [](int state, Character c){
        switch(state){
            case 0:
                if(c.equals(Character("1"))){return 1;}
                return 0;
            case 1:
                if(c.equals(Character("0"))){return 0;}
                return 1;
        }
        return -1;
    }, [](int state){return state == 0;});
    example1_10.setName("example1_10");

    //task 8, example 1_12
    DFA<int> example1_12([](int state){return (state <= 4 && state > -1);}, alpha, 2, [](int state, Character c){
        switch(state){
            case 2:
                return c.equals(Character("0")) ? 1 : 3;
            case 1:
                return c.equals(Character("1")) ? 0 : 1;
            case 0:
                return c.equals(Character("0")) ? 1 : 0;
            case 3:
                return c.equals(Character("0")) ? 4 : 3;
            case 4:
                return c.equals(Character("1")) ? 3 : 4;
        }
        return -1;
    }, [](int state){return state == 1 || state == 3;});
    example1_12.setName("example1_12");
    
    //task 8, example 1_14
    alphabet ex1_14;
    ex1_14.addCharToAlphabet(Character("0"));
    ex1_14.addCharToAlphabet(Character("1"));
    ex1_14.addCharToAlphabet(Character("2"));
    ex1_14.addCharToAlphabet(Character("RESET"));
    DFA<int> example1_14([](int state){return state == 0 || state == 1 || state == 2;}, ex1_14, 0, [](int state, Character c){
        switch(state) {
            case 0:
                if(c.equals(Character("1"))) {return 1;}
                else if(c.equals(Character("2"))) {return 2;}
                return 0;
            case 1:
                if(c.equals(Character("1"))) {return 2;}
                else if(c.equals(Character("2")) || c.equals(Character("RESET"))) {return 0;}
                return 1;
            case 2:
                if(c.equals(Character("2"))) {return 1;}
                else if(c.equals(Character("1")) || c.equals(Character("RESET"))) {return 0;}
                return 2;
        }
        return -1;
    }, [](int state){return state == 0;});
    example1_14.setName("example1_14");

    //task 8, self made DFA
    //checks to see if the word hey exists in the given string
    alphabet heyBet;
    heyBet.addCharToAlphabet(Character("h"));
    heyBet.addCharToAlphabet(Character("e"));
    heyBet.addCharToAlphabet(Character("y"));
    DFA<int> heyCheck([](int state){return state <= 3 && state > -1;}, heyBet, 0, [](int state, Character c){
        switch(state) {
            case 0:
                if(c.equals(Character("h"))){return 1;}
                return 0;
            case 1:
                if(c.equals(Character("e"))){return 2;}
                else if(c.equals(Character("h"))){return 1;}
                return 0;
            case 2:
                if(c.equals(Character("y"))){return 3;}
                else if(c.equals(Character("h"))){return 1;}
                return 0;
            case 3:
                return 3;
        }
        return -1;
    }, [](int state){return state == 3;});
    heyCheck.setName("heyCheck");

    //task 8, self made DFA 2
    DFA<int> oddOnesEvenTotal([](int state){return state <= 3 && state > -1;}, alpha, 0, [](int state, Character c){
        switch(state) {
            case 0:
                if(c.equals(Character("0"))) {return 2;}
                return 1;
            case 1:
                if(c.equals(Character("0"))) {return 3;}
                return 0;
            case 2:
                if(c.equals(Character("0"))) {return 0;}
                return 3;
            case 3:
                if(c.equals(Character("0"))) {return 1;}
                return 2;
        }
        return -1;
    }, [](int state){return state == 3;});
    oddOnesEvenTotal.setName("oddOnesEvenTotal");

    //task 9, testing each DFA
    std::vector<DFA<int>> dVec {noAccept, emptyOnly, CharGen, example1_2, example1_4, example1_8, example1_10, example1_12, example1_14, heyCheck, oddOnesEvenTotal};

    for(int i = 0; i < (int) dVec.size(); i++){
        std::cout << "running DFA " << dVec.at(i).getName() << "\n";
        for(int j = 0; j < 20; j++){
            std::cout << "using string " << dVec.at(i).getSigma().findNLexo(j).printable() << ": " << dVec.at(i).runDFA(dVec.at(i).getSigma().findNLexo(j)) << "\n";
            std::cout << "printing trace: ";
            dVec.at(i).printTrace();
            std::cout << "\n";
        }
        std::cout << "-----------------------------------------------------------------------------------\n";
    }

    //showcase of tasks 10 and 11
    for(int i = 0; i < (int) dVec.size(); i++){
        std::cout << "running DFA " << dVec.at(i).getName() << " with detDFA and getTrace functions\n";
        for(int j = 0; j < 20; j++){
            std::cout << "using string " << dVec.at(i).getSigma().findNLexo(j).printable() << ": " << detDFA(dVec.at(i), dVec.at(i).getSigma().findNLexo(j)) << "\n";
            getTrace(dVec.at(i), dVec.at(i).getSigma().findNLexo(j));
            std::cout << "\n";
        }
        std::cout << "-----------------------------------------------------------------------------------\n";
    }

    //showcase of task 12
    for(int i = 0; i < (int) dVec.size(); i++){
        std::cout << "finding string using task12 and DFA " << dVec.at(i).getName() << "\n";
        auto output = task12(dVec.at(i));
        if(output.has_value() == false) {std::cout << "false, no string found.\n";}
        else {std::cout << "string found, string is: " << output->printable() << "\n";}
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    //showcase of task 13
    for(int i = 0; i < (int) dVec.size(); i++){
        std::cout << "complementing " << dVec.at(i).getName() << "\n";
        DFA comp = complementDFA(dVec.at(i));
        for(int j = 0; j < 20; j++){
            std::cout << "original output for " << dVec.at(i).getSigma().findNLexo(j).printable() << " is: " << detDFA(dVec.at(i), dVec.at(i).getSigma().findNLexo(j)) << "\n";

            std::cout << "output of comp  for " << dVec.at(i).getSigma().findNLexo(j).printable() << " is: " << detDFA(comp, dVec.at(i).getSigma().findNLexo(j)) << "\n\n";
        }
        std::cout << "-----------------------------------------------------------------------------------\n";
    }

    //task 15
    auto testunin = [](auto DFA){
        for(int i = 0; i < 20; i++) {
            std::cout << DFA.getName() << " string = " << DFA.getSigma().findNLexo(i).printable() << " output = ";
            if(DFA.runDFA(DFA.getSigma().findNLexo(i)))
                std::cout << "true\n";
            else
                std::cout << "false\n";
        }
        std::cout << "\n";
    };
    std::cout << "testing union\n";
    auto u = unionDFA(example1_12, oddOnesEvenTotal);
    auto u2 = unionDFA(example1_8, example1_4);
    auto u3 = unionDFA(example1_10, example1_12);
    auto u4 = unionDFA(noAccept, emptyOnly);
    testunin(u);
    testunin(u2);
    testunin(u3);
    testunin(u4);
    std::cout << "-----------------------------------------------------------------------------------\n";

    std::cout << "attempting to union a unioned DFA with a non-unioned DFA.\n";
    auto u5 = unionDFA(u, emptyOnly); 
    auto u6 = unionDFA(u2, example1_10); 
    auto u7 = unionDFA(u3, noAccept); 
    auto u8 = unionDFA(u4, oddOnesEvenTotal); 
    testunin(u5);
    testunin(u6);
    testunin(u7);
    testunin(u8);
    std::cout << "-----------------------------------------------------------------------------------\n";

    std::cout << "attempting to union two unioned DFA's.\n";
    auto u9 = unionDFA(u, u2);
    auto u10 = unionDFA(u3, u4);
    auto u11 = unionDFA(u, u3);
    auto u12 = unionDFA(u2, u4);
    testunin(u9);
    testunin(u10);
    testunin(u11);
    testunin(u12);
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 17
    auto in = intersectDFA(example1_12, oddOnesEvenTotal);
    auto in2 = intersectDFA(example1_8, example1_4);
    auto in3 = intersectDFA(example1_10, example1_12);
    auto in4 = intersectDFA(noAccept, emptyOnly);

    auto in5 = intersectDFA(example1_12, example1_8);
    auto in6 = intersectDFA(oddOnesEvenTotal, example1_4);
    auto in7 = intersectDFA(example1_10, noAccept);
    auto in8 = intersectDFA(emptyOnly, example1_12);

    auto in9 = intersectDFA(example1_12, emptyOnly);
    auto in10 = intersectDFA(oddOnesEvenTotal, noAccept);
    auto in11 = intersectDFA(example1_8, example1_12);
    auto in12 = intersectDFA(example1_4, example1_10);

    std::cout << "attempting intersect of standard DFA's.\n";
    testunin(in);
    testunin(in2);
    testunin(in3);
    testunin(in4);
    std::cout << "-----------------------------------------------------------------------------------\n";
    testunin(in5);
    testunin(in6);
    testunin(in7);
    testunin(in8);
    std::cout << "-----------------------------------------------------------------------------------\n";
    testunin(in9);
    testunin(in10);
    testunin(in11);
    testunin(in12);
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 19
    auto testTasks = [](auto x, auto y, bool test, bool isSub = true){
        std::cout << "testing if " << x.getName() << " is";
        if(isSub)
            std::cout << " a subset of ";
        else
            std::cout << " equal to ";
        std::cout << y.getName() << ": ";
        if(test)
            std::cout << "true\n";
        else
            std::cout << "false\n";
    };
    testTasks(example1_8, example1_4, subset(example1_8, example1_4));
    testTasks(example1_4, example1_8, subset(example1_4, example1_8));
    testTasks(example1_10, example1_10, subset(example1_10, example1_10));
    testTasks(u10, u11, subset(u10, u11));
    testTasks(u11, u10, subset(u11, u10));
    testTasks(u, oddOnesEvenTotal, subset(u, oddOnesEvenTotal));
    testTasks(oddOnesEvenTotal, u, subset(oddOnesEvenTotal, u));
    testTasks(in, example1_12, subset(in, example1_12));
    testTasks(example1_12, in, subset(example1_12, in));
    testTasks(example1_12, example1_12, subset(example1_12, example1_12));
    testTasks(u5, u, subset(u5, u));
    testTasks(u, u5, subset(u,u5));
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 21
    testTasks(example1_8, example1_4, equals(example1_8, example1_4), false);
    testTasks(example1_10, example1_10, equals(example1_10,example1_10), false);
    testTasks(u10, u11, equals(u10,u11), false);
    testTasks(u, oddOnesEvenTotal, equals(u, oddOnesEvenTotal), false);
    testTasks(u,u, equals(u,u), false);
    testTasks(in4, noAccept, equals(in4, noAccept), false);
    testTasks(in9, noAccept, equals(in9, noAccept), false);
    testTasks(in2, example1_4, equals(in2, example1_4), false);
    testTasks(in2, example1_8, equals(in2, example1_8), false);
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 22
    testTasks(in4, noAccept, equals(in4, noAccept), false); //intersect 4 should output noAccept
    testTasks(in9, noAccept, equals(in9, noAccept), false); //intersect 9 should output noAccept
    testTasks(u4, emptyOnly, equals(u4, emptyOnly), false); //union 4 should output emptyOnly 
    testTasks(u2, example1_4, equals(u2, example1_4), false); //union 2 should output example1_4

    //expressly defined so that it is the complement of noAccept
    DFA<int> allAccept([](int state) { return state == 0; }, alpha, 0, 
                 [](int state, Character c){ return 0; }, [](int state){ return state == 0; });
    allAccept.setName("allAccept");
    testTasks(complementDFA(noAccept), allAccept, equals(complementDFA(noAccept), allAccept), false); //the complement of a DFA that accepts nothing should accept everything

    //expressly defined so that it is the complement of emptyOnly
    DFA<int> notEmptyOnly([](int state){return state == 0 || state == 1;}, alpha, 0, 
                  [](int state, Character c){ return 1; }, [](int state){return state == 1;});
    notEmptyOnly.setName("notEmptyOnly");
    testTasks(complementDFA(emptyOnly), notEmptyOnly, equals(complementDFA(emptyOnly), notEmptyOnly), false); //the complement of this DFA should be true iff it isn't empty
    std::cout << "-----------------------------------------------------------------------------------\n";


    //defining characters 0 and 1 because they are all that I use in these situations
    Character zero("0");
    Character one("1");
    Character eps("");

    //task 25
    NFA<int> test1([](int state){return state <= 3 && state >= 0;}, alpha, 0, [](int state, Character c){
        std::vector<int> possTrans;
        if(c.equals(Character(""))) { return possTrans; }
        switch(state) {
            case 0:
                possTrans.push_back(0);
                if(c.equals(Character("1"))) {possTrans.push_back(1);}
                break;
            case 1:
                possTrans.push_back(2);
                break;
            case 2:
                possTrans.push_back(3);
                break;
            case 3:
                possTrans.push_back(3);
                break;
        }
        return possTrans;
    }, [](int state){ return state == 3;});
    test1.setName("test1");
    //task 26
    //each nfa will be getting 6 test traces
    //traces are setup as the state it is currently at, and the character that will bring it to the next state.
    //the final state will have a character with the value "end", this is so the trace knows it's reached the end.
    std::vector<std::vector<std::pair<int, bool>>> test1Traces;
    std::vector<str> test1Strings;

    // will fail
    test1Traces.push_back(std::vector<std::pair<int, bool>>{});
    test1Strings.push_back(str(std::vector<Character>{}));
    
    // will result in a true, as it ends at state 3
    test1Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(0,false), std::make_pair<int, bool>(1,false), 
                                                            std::make_pair<int, bool>(2,false), std::make_pair<int, bool>(3,false)});
    test1Strings.push_back(str(std::vector<Character>{zero, one, one, one}));

    //using the same set of inputs as above this will fail if it goes to 0 instead of 1
    test1Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(0,false), std::make_pair<int, bool>(0,false), 
                                                            std::make_pair<int, bool>(1,false), std::make_pair<int, bool>(2,false)});
    test1Strings.push_back(str(std::vector<Character>{zero, one, one, one}));
    
    // //again using the same inputs, this will also fail, in a different way, due to changing how many times 1 goes to 0
    test1Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(0,false), std::make_pair<int, bool>(0,false), 
                                                            std::make_pair<int, bool>(0,false), std::make_pair<int, bool>(1,false)});
    test1Strings.push_back(str(std::vector<Character>{zero, one, one, one}));
    
    // //test will fail due to not reaching accepting state
    test1Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(0,false)});
    test1Strings.push_back(str(std::vector<Character>{one}));

    // //same as above, however it does go to state 1 instead of staying at 0
    test1Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(1,false)});
    test1Strings.push_back(str(std::vector<Character>{one}));

    NFA<int> test2([](int state){return state <= 3 && state >= 0;}, alpha, 0, [](int state, Character c){
        std::vector<int> possTrans;
        if(c.equals(Character(""))) { 
            return possTrans; 
        }
        switch(state) {
            case 0:
                possTrans.push_back(0);
                if(c.equals(Character("0"))){possTrans.push_back(1);}
                else {possTrans.push_back(2);}
                break;
            case 1:
                if(c.equals(Character("0"))){possTrans.push_back(3);}
                break;
            case 2:
                possTrans.push_back(3);
                if(c.equals(Character("0"))){possTrans.push_back(2);};
                break;
            case 3:
                possTrans.push_back(3);
                break;
        }
        return possTrans;
    }, [](int state){ return state == 3;});
    test2.setName("test2");
    //task 26
    std::vector<std::vector<std::pair<int, bool>>> test2Traces;
    std::vector<str> test2Strings;

    //will fail
    test2Traces.push_back(std::vector<std::pair<int, bool>>{});
    test2Strings.push_back(str(std::vector<Character>{}));

    //will also fail
    test2Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(0,false)});
    test2Strings.push_back(str(std::vector<Character>{zero}));

    //will also fail, however the state brought to from the character 0 is 1, not 0
    test2Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(1,false)});
    test2Strings.push_back(str(std::vector<Character>{zero}));

    //this will pass
    test2Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(0,false), std::make_pair<int, bool>(2,false), 
                                                            std::make_pair<int, bool>(3,false)});
    test2Strings.push_back(str(std::vector<Character>{one,one,zero}));

    // //this will fail because at state 2, the input 0 had gone back to state 2
    test2Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(0,false), std::make_pair<int, bool>(2,false), 
                                                            std::make_pair<int, bool>(2,false)});
    test2Strings.push_back(str(std::vector<Character>{one,one,zero}));

    // //will pass
    test2Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(0,false), std::make_pair<int, bool>(2,false), 
                                                            std::make_pair<int, bool>(2,false), std::make_pair<int, bool>(3,false)});
    test2Strings.push_back(str(std::vector<Character>{one,one,zero,one}));

    //nfa from figure 1.27 on page 48
    NFA<int> test3([](int state){return state >= 1 && state <= 4;}, alpha, 1, [](int state, Character c){
        std::vector<int> possTrans;
        bool isEps = c.equals(Character(""));
        switch(state) {
            case 1:
                if(!isEps) {
                    possTrans.push_back(1);
                    if(c.equals(Character("1"))) {possTrans.push_back(2);}
                }
                break;
            case 2:
                if(isEps || c.equals(Character("0"))) {possTrans.push_back(3);}
                break;
            case 3:
                if(!isEps && c.equals(Character("1"))) {possTrans.push_back(4);}
                break;
            case 4:
                if(!isEps) {possTrans.push_back(4);}
                break;
        }
        return possTrans;
    }, [](int state){return state == 4;});
    test3.setName("test3");
    //task 26
    std::vector<std::vector<std::pair<int, bool>>> test3Traces;
    std::vector<str> test3Strings;

    //will fail
    test3Traces.push_back(std::vector<std::pair<int, bool>>{});
    test3Strings.push_back(str(std::vector<Character>{}));

    //will pass
    test3Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(2,false), std::make_pair<int, bool>(3,true), 
                                                            std::make_pair<int, bool>(4,false)});
    test3Strings.push_back(str(std::vector<Character>{one,one}));
    
    //will fail
    test3Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(1,false), std::make_pair<int, bool>(2,false)});
    test3Strings.push_back(str(std::vector<Character>{one,one}));

    //will fail
    test3Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(1,false), std::make_pair<int, bool>(2,false), 
                                                            std::make_pair<int, bool>(3,true)});
    test3Strings.push_back(str(std::vector<Character>{one,one}));

    //will fail
    test3Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(1,false), std::make_pair<int, bool>(2,false), 
                                                            std::make_pair<int, bool>(3,false)});
    test3Strings.push_back(str(std::vector<Character>{one,one,zero}));
    
    //will pass, added the last character 1 which brought us to state 4, an accepting state
    test3Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(1,false), std::make_pair<int, bool>(2,false), 
                                                            std::make_pair<int, bool>(3,false), std::make_pair<int, bool>(4,false)});
    test3Strings.push_back(str(std::vector<Character>{one,one,zero,one}));

    //from figure 1.33 on page 52 of the book
    alphabet singleCharacter;
    singleCharacter.addCharToAlphabet(Character("0"));
    NFA<int> test4([](int state){return state >= 0 && state <= 5;}, singleCharacter, 0, [](int state, Character c){
        std::vector<int> possTrans;
        bool isEps = c.equals(Character(""));
        switch(state) {
            case 0:
                if(isEps) {
                    possTrans.push_back(1);
                    possTrans.push_back(3);
                }
                break;
            case 1:
                if(!isEps) {possTrans.push_back(2);}
                break;
            case 2:
                if(!isEps) {possTrans.push_back(1);}
                break;
            case 3:
                if(!isEps) {possTrans.push_back(4);}
                break;
            case 4:
                if(!isEps) {possTrans.push_back(5);}
                break;
            case 5:
                if(!isEps) {possTrans.push_back(3);}
                break;
        }
        return possTrans;
    }, [](int state){return state == 1 || state == 3;});
    test4.setName("test4");
    //task 26
    std::vector<std::vector<std::pair<int, bool>>> test4Traces;
    std::vector<str> test4Strings;

    //passes
    test4Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(1,true)});
    test4Strings.push_back(str(std::vector<Character>{}));

    //passes
    test4Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(3,true)});
    test4Strings.push_back(str(std::vector<Character>{}));

    //fails
    test4Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(1,true), std::make_pair<int, bool>(2,false)});
    test4Strings.push_back(str(std::vector<Character>{zero}));

    //fails
    test4Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(3,true), std::make_pair<int, bool>(4,false)});
    test4Strings.push_back(str(std::vector<Character>{zero}));
    
    //fails
    test4Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(3,true), std::make_pair<int, bool>(4,false),
                                                            std::make_pair<int, bool>(5,false)});
    test4Strings.push_back(str(std::vector<Character>{zero,zero}));

    //passes
    test4Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(1,true), std::make_pair<int, bool>(2,false),
                                                            std::make_pair<int, bool>(1,false)});
    test4Strings.push_back(str(std::vector<Character>{zero,zero}));

    NFA<int> test5([](int state){return state >= 1 && state <= 3;}, alpha, 1, [](int state, Character c){
        std::vector<int> possTrans;
        bool isEps = c.equals(Character(""));
        switch(state) {
            case 1:
                if(isEps) {possTrans.push_back(3);}
                else if(c.equals(Character("1"))){possTrans.push_back(2);}
                break;
            case 2:
                if(!isEps) {
                    possTrans.push_back(3);
                    if(c.equals(Character("0"))) {possTrans.push_back(2);}
                }
                break;
            case 3:
                if(!isEps && c.equals(Character("0"))) {possTrans.push_back(1);}
                break;
        }
        if(!c.equals(Character("")) && possTrans.size() == 0){possTrans.push_back(state);}
        return possTrans;
    }, [](int state){ return state == 1;});
    test5.setName("test5");
    //task 26
    std::vector<std::vector<std::pair<int, bool>>> test5Traces;
    std::vector<str> test5Strings;

    //passes
    test5Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(3,true), std::make_pair<int, bool>(1,false)});
    test5Strings.push_back(str(std::vector<Character>{zero}));

    //fails
    test5Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(2,false)});
    test5Strings.push_back(str(std::vector<Character>{one}));

    //fails
    test5Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(2,false), std::make_pair<int, bool>(3,false)});
    test5Strings.push_back(str(std::vector<Character>{one,zero}));

    //passes
    test5Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(2,false), std::make_pair<int, bool>(2,false), 
                                                            std::make_pair<int, bool>(3,false), std::make_pair<int, bool>(1,false)});
    test5Strings.push_back(str(std::vector<Character>{one,zero,zero,zero}));

    //fails
    test5Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(2,false), std::make_pair<int, bool>(2,false), 
                                                            std::make_pair<int, bool>(3,false), std::make_pair<int, bool>(1,false),
                                                            std::make_pair<int, bool>(3,true)});
    test5Strings.push_back(str(std::vector<Character>{one,zero,zero,zero}));

    //passes
    test5Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(2,false), std::make_pair<int, bool>(2,false), 
                                                            std::make_pair<int, bool>(3,false), std::make_pair<int, bool>(1,false),
                                                            std::make_pair<int, bool>(3,true), std::make_pair<int, bool>(1,false)});
    test5Strings.push_back(str(std::vector<Character>{one,zero,zero,zero,zero}));

    NFA<int> test6([](int state){return state >= 1 && state <= 4;}, alpha, 1, [](int state, Character c){
        std::vector<int> possTrans;
        bool isEps = c.equals(Character(""));
        switch(state) {
            case 1:
                if(isEps) {possTrans.push_back(3);}
                else if(c.equals(Character("0"))) {possTrans.push_back(2);}
                break;
            case 2:
                if(!isEps) {
                    if(c.equals(Character("1"))) {
                        possTrans.push_back(2);
                        possTrans.push_back(4);
                    }
                }
                break;
            case 3:
                if(isEps) {possTrans.push_back(2);}
                else if(c.equals(Character("0"))) {possTrans.push_back(4);}
                break;
            case 4:
                if(!isEps && c.equals(Character("0"))) {possTrans.push_back(3);}
                break;
        }
        if(!c.equals(Character("")) && possTrans.size() == 0){possTrans.push_back(state);}
        return possTrans;
    }, [](int state){ return state == 3;});
    test6.setName("test6");
    //task 26
    std::vector<std::vector<std::pair<int, bool>>> test6Traces;
    std::vector<str> test6Strings;

    //fails
    test6Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(3,true), std::make_pair<int, bool>(4,false)});
    test6Strings.push_back(str(std::vector<Character>{zero}));

    //fails, should fail oracle as well
    test6Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(3,true), std::make_pair<int, bool>(1,false)});
    test6Strings.push_back(str(std::vector<Character>{one}));

    //fails
    test6Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(2,false), std::make_pair<int, bool>(4,false)});
    test6Strings.push_back(str(std::vector<Character>{zero,one}));

    //passes
    test6Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(2,false), std::make_pair<int, bool>(4,false),
                                                            std::make_pair<int, bool>(3,false)});
    test6Strings.push_back(str(std::vector<Character>{zero,one,zero}));

    //fails
    test6Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(3,true), std::make_pair<int, bool>(2,true)});
    test6Strings.push_back(str(std::vector<Character>{}));

    //fails, should fail oracle as well
    test6Traces.push_back(std::vector<std::pair<int, bool>>{std::make_pair<int, bool>(3,true), std::make_pair<int, bool>(4,false),
                                                            std::make_pair<int, bool>(1,false)});
    test6Strings.push_back(str(std::vector<Character>{zero,one}));
    std::cout << "-----------------------------------------------------------------------------------\n";
    auto oracleTest = [](NFA<int> test, auto traces, auto strings, auto trues){
        std::cout << "printing out the results of oracle using NFA " << test.getName() << "\n";
        for(int i = 0; i < (int)traces.size(); i++){
            std::cout << oracle(test, strings.at(i), traces.at(i), trues.at(i), test.getStart()) << " ";
        }
        std::cout << "\n";
    };
    oracleTest(test1, test1Traces, test1Strings, std::vector<bool>{false, true, false, false, false, false});
    oracleTest(test2, test2Traces, test2Strings, std::vector<bool>{false, false, false, true, false, true});  
    oracleTest(test3, test3Traces, test3Strings, std::vector<bool>{false, true, false, false, false, true});
    oracleTest(test4, test4Traces, test4Strings, std::vector<bool>{true, true, false, false, false, true});
    oracleTest(test5, test5Traces, test5Strings, std::vector<bool>{true, false, false, true, false, true});
    oracleTest(test6, test6Traces, test6Strings, std::vector<bool>{false, false, false, true, false, false});


    auto forkingTest = [](auto handMade, auto functionMade){
        std::cout << "testing to see if they are equal\n starting with the hand made trace tree\n";
        handMade.printTree();
        std::cout << "now printing the function made trace tree\n";
        functionMade.printTree();
    };
    
    //this sucks to make by hand, also task 29
    traceTree<int> test1TraceTree(0, one, false, std::vector<traceTree<int>>{
        traceTree<int>(0, Character(""), false, std::vector<traceTree<int>>{}),
        traceTree<int>(1, Character(""), false, std::vector<traceTree<int>>{})
    });

    traceTree<int> test2TraceTree(0, one, false, std::vector<traceTree<int>>{
        traceTree<int>(0, one, false, std::vector<traceTree<int>>{
            traceTree<int>(0, zero, false, std::vector<traceTree<int>>{
                traceTree<int>(0, eps, false, std::vector<traceTree<int>>{}),
                traceTree<int>(1, eps, false, std::vector<traceTree<int>>{})
            }),
            traceTree<int>(2, zero, false, std::vector<traceTree<int>>{
                traceTree<int>(3, eps, true, std::vector<traceTree<int>>{}),
                traceTree<int>(2, eps, false, std::vector<traceTree<int>>{})
            })
        }),
        traceTree<int>(2, one, false, std::vector<traceTree<int>>{
            traceTree<int>(3, zero, true, std::vector<traceTree<int>>{
                traceTree<int>(3, eps, true, std::vector<traceTree<int>>{})
            })
        })
    });

    traceTree<int> test3TraceTree(1, one, false, std::vector<traceTree<int>>{
        traceTree<int>(1, one, false, std::vector<traceTree<int>>{
            traceTree<int>(1, eps, false, std::vector<traceTree<int>>{}),
            traceTree<int>(2, eps, false, std::vector<traceTree<int>>{
                traceTree<int>(3, eps, false, std::vector<traceTree<int>>{})
            })
        }),
        traceTree<int>(2, one, false, std::vector<traceTree<int>>{
            traceTree<int>(3, one, false, std::vector<traceTree<int>>{
                traceTree<int>(4, eps, true, std::vector<traceTree<int>>{})
            })
        })
    });

    traceTree<int> test4TraceTree(0, zero, false, std::vector<traceTree<int>>{
        traceTree<int>(1, zero, true, std::vector<traceTree<int>>{
            traceTree<int>(2, eps, false, std::vector<traceTree<int>>{})
        }),
        traceTree<int>(3, zero, true, std::vector<traceTree<int>>{
            traceTree<int>(4, eps, false, std::vector<traceTree<int>>{})
        })
    });

    traceTree<int> test5TraceTree(1, one, true, std::vector<traceTree<int>>{
        traceTree<int>(3, one, false, std::vector<traceTree<int>>{
            traceTree<int>(3, zero, false, std::vector<traceTree<int>>{
                traceTree<int>(1, eps, true, std::vector<traceTree<int>>{
                    traceTree<int>(3, eps, false, std::vector<traceTree<int>>{})
                })
            })
        }),
        traceTree<int>(2, zero, false, std::vector<traceTree<int>>{
            traceTree<int>(3, eps, false, std::vector<traceTree<int>>{}),
            traceTree<int>(2, eps, false, std::vector<traceTree<int>>{})
        })
    });

    traceTree<int> test6TraceTree(1, zero, false, std::vector<traceTree<int>>{
        traceTree<int>(3, zero, true, std::vector<traceTree<int>>{
            traceTree<int>(2, zero, false, std::vector<traceTree<int>>{
                traceTree<int>(2, one, false, std::vector<traceTree<int>>{
                    traceTree<int>(2, eps, false),
                    traceTree<int>(4, eps, false)
                })
            }),
            traceTree<int>(4, one, false, std::vector<traceTree<int>>{
                traceTree<int>(4, eps, false)
            })
        }),
        traceTree<int>(2, one, false, std::vector<traceTree<int>>{
            traceTree<int>(2, eps, false),
            traceTree<int>(4, eps, false)
        })
    });

    //testing task 30 using above traceTree as reference
    forkingTest(test1TraceTree, forking(test1, test1Strings.at(5)));
    forkingTest(test2TraceTree, forking(test2, test2Strings.at(4)));
    forkingTest(test3TraceTree, forking(test3, test3Strings.at(1)));
    forkingTest(test4TraceTree, forking(test4, test4Strings.at(2)));
    forkingTest(test5TraceTree, forking(test5, test5Strings.at(2)));
    forkingTest(test6TraceTree, forking(test6, test6Strings.at(2)));
    std::cout << "-----------------------------------------------------------------------------------\n";
    auto backTest = [](auto nfa, str test){
        std::cout << "testing backTracing using " << nfa.getName() << " and string " << test.printable() << ": " << (backTracking(nfa, test) ? "true\n": "false\n");
    };
    backTest(test1, test1Strings.at(3));
    backTest(test2, test2Strings.at(0));
    backTest(test3, test3Strings.at(5));
    backTest(test4, test4Strings.at(2));
    backTest(test5, test5Strings.at(5));
    backTest(test6, test6Strings.at(2));
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 31 also doubles as a trace tree and oracle test
    auto genTests = []<typename T>(NFA<T> nfa, str toUse, int toGen = 10){
        std::cout << "-------------------------------------------------\n";
        int cap = 50;
        int accepting = 0;
        auto tt = forking(nfa, toUse);
        for(int i = 0; i < toGen; i++) {
            int j;
            std::vector<std::pair<T, bool>> traces;
            auto ttTest = tt;
            str output;
            int temp = toUse.getSize();
            str breakS = toUse;
            bool firstRun = true;
            traces.clear();
            for(j = 0; j < cap && temp; j++) {
                while(temp) {
                    if(!firstRun) {
                        traces.push_back({ttTest.getState(), ttTest.getIsEps()});
                        if(!ttTest.getIsEps()) {
                            temp--;
                            output.addCharToStr(breakS.getCharacter(0));
                            breakS.removeFront();
                        }
                    }
                    if(temp && ttTest.getChildren().size() != 0) {
                        std::random_device rd;
                        ttTest = ttTest.getChildren().at(rd() % (int) ttTest.getChildren().size());
                        firstRun = false;
                    } else if(temp && ttTest.getChildren().size() == 0) {
                        std::cout << "attempt " << i + 1 << " has reached the end before reaching full string, using string generated up to this point\n";
                        temp = 0;
                        if(oracle(nfa, output, traces, ttTest.getAccepting(), nfa.getStart()))
                            accepting++;
                    }
                }
                if(oracle(nfa, toUse, traces, ttTest.getAccepting(), nfa.getStart()))
                    accepting++;
            }
            if(j == 50) {
                std::cout << "number of steps reached hard cap of 50, terminating current run, will run oracle with what was generated\n";
                if(oracle(nfa, toUse, traces, ttTest.getAccepting(), nfa.getStart()))
                    accepting++;
            }
        }
        std::cout << "number of accepted traces: " << accepting << "\n";
    };
    genTests(test1, str(std::vector<Character> {one, one, one, zero, zero, one, one, zero, zero, one, one}));
    genTests(test2, str(std::vector<Character> {zero, one, one, zero}));
    genTests(test3, str(std::vector<Character> {one, zero, zero, one, one, zero}));
    genTests(test4, str(std::vector<Character> {zero, zero, zero, zero, zero}));
    genTests(test5, str(std::vector<Character> {one, one, one, zero, zero, zero}));
    genTests(test6, str(std::vector<Character> {zero, one, one, one, zero, zero, one, zero}));
    std::cout << "-----------------------------------------------------------------------------------\n";
    auto unionTest = [](auto nfa, str test){
        std::cout << "testing union using " << nfa.getName() << " with string " << test.printable() << ": " << (backTracking(nfa, test) ? "true\n": "false\n"); 
    };
    auto unTest = unionNFA(test1, test2);
    auto unTest2 = unionNFA(test1, test5);
    auto unTest3 = unionNFA(test2, test3);
    auto unTest4 = unionNFA(test6, test2);
    auto unTest5 = unionNFA(unTest, unTest2);
    auto unTest6 = unionNFA(unTest3, unTest4);

    //three tests each, one for each situation
    //failure state for both, pass for one, and pass for the other
    //if it outputs false, true, true, then it is correct.
    //genTests are extra tests each is a total of 10 
    genTests(unTest, str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    unionTest(unTest, test1Strings.at(5));
    unionTest(unTest, test1Strings.at(1));
    unionTest(unTest, test2Strings.at(3));
    std::cout << "-----------------------------------------------------------------------------------\n";
    genTests(unTest2, str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    unionTest(unTest2, test1Strings.at(5));
    unionTest(unTest2, test1Strings.at(1));
    unionTest(unTest2, test5Strings.at(0));
    std::cout << "-----------------------------------------------------------------------------------\n";
    genTests(unTest3, str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    unionTest(unTest3, test1Strings.at(0));
    unionTest(unTest3, test2Strings.at(3));
    unionTest(unTest3, test3Strings.at(1));
    std::cout << "-----------------------------------------------------------------------------------\n";
    genTests(unTest4, str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    unionTest(unTest4, test6Strings.at(0));
    unionTest(unTest4, test6Strings.at(3));
    unionTest(unTest4, test2Strings.at(3));
    std::cout << "-----------------------------------------------------------------------------------\n";
    genTests(unTest5, str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    unionTest(unTest5, test1Strings.at(5));
    unionTest(unTest5, test1Strings.at(1));
    unionTest(unTest5, test5Strings.at(0));
    std::cout << "-----------------------------------------------------------------------------------\n";
    genTests(unTest6, str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    unionTest(unTest6, test6Strings.at(0));
    unionTest(unTest6, test2Strings.at(3));
    unionTest(unTest6, test6Strings.at(3));
    std::cout << "-----------------------------------------------------------------------------------\n";

    auto concTest = [](auto nfa, str test){
        std::cout << "testing concat using " << nfa.getName() << " with string " << test.printable() << ": " << (backTracking(nfa, test) ? "true\n": "false\n");
    };
    //first 6 should pass
    concTest(concNFA(test1, test2), str(std::vector<Character>{zero, one, one, one, one, one, zero}));
    concTest(concNFA(test3, test5), str(std::vector<Character>{one, one, one, zero, zero, zero, zero}));
    concTest(concNFA(unTest, test6), str(std::vector<Character>{one, one, zero, zero, one, zero}));
    concTest(concNFA(unTest2, unTest3), str(std::vector<Character>{zero, one, one, one, one, one}));
    concTest(concNFA(unTest4, unTest5), str(std::vector<Character>{zero, one, zero, zero}));
    concTest(concNFA(test1, unTest6), str(std::vector<Character>{zero, one, one, one, zero, one, zero}));
    std::cout << "-----------------------------------------------------------------------------------\n";
    //these 6 should fail
    concTest(concNFA(test1, test2), str(std::vector<Character>{zero, one, one, one, one}));
    concTest(concNFA(test3, test5), str(std::vector<Character>{one}));
    concTest(concNFA(unTest, test6), str(std::vector<Character>{one}));
    concTest(concNFA(unTest2, unTest3), str(std::vector<Character>{zero}));
    concTest(concNFA(unTest4, unTest5), str(std::vector<Character>{zero, one}));
    concTest(concNFA(test1, unTest6), str(std::vector<Character>{zero, one, zero}));
    std::cout << "-----------------------------------------------------------------------------------\n";
    //extra conc tests
    genTests(concNFA(test1, test2), str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    genTests(concNFA(test3, test5), str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    genTests(concNFA(unTest, test6), str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    genTests(concNFA(unTest2, unTest3), str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    genTests(concNFA(unTest4, unTest5), str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    genTests(concNFA(test1, unTest6), str(std::vector<Character> {one, one, zero, zero, one, zero, one, zero, zero, one, one}));
    std::cout << "-----------------------------------------------------------------------------------\n";
    //task 37
    //works best when you have at least 2 accepted strings, as it uses them to build larger strings to test with
    //the most it will append strings together is 8, as I don't want the input to be so long as to cause problems
    //still takes a small bit of time to run though.
    auto kleeneTests = [=]<typename T>(NFA<T> input, std::vector<str> acceptedStrings, int toGen = 10){
        std::random_device rd;
        int passes = 0;
        str toUse;
        for(int j = 0; j < toGen; j++) {
            int appendTimes = (rd() % 8) + 1;
            for(int i = 0; i < appendTimes; i++)
                toUse.append(acceptedStrings.at(rd() % (int) acceptedStrings.size()));
            if(backTracking(input, toUse))
                passes++;
        }
        std::cout << "number of accepted strings: " << passes << " number that should pass: " << toGen << "\n";
    };
    // kleeneTests(kleeneStar(test1), std::vector<str>{str(std::vector<Character>{zero, one, one, one}), str(std::vector<Character>{one, zero, zero}), 
    //                                                str(std::vector<Character>{one, zero, zero, one})});
    // kleeneTests(kleeneStar(test2), std::vector<str>{str(std::vector<Character>{one,one,zero}), str(std::vector<Character>{one,one,zero,one})});
    // kleeneTests(kleeneStar(test3), std::vector<str>{str(std::vector<Character>{one,one}), str(std::vector<Character>{one,one,zero,one})});
    // kleeneTests(kleeneStar(test4), std::vector<str>{str(std::vector<Character>{}), str(std::vector<Character>{zero,zero})});
    // kleeneTests(kleeneStar(test5), std::vector<str>{str(std::vector<Character>{zero}), str(std::vector<Character>{one, one, zero})});
    kleeneTests(kleeneStar(test6), std::vector<str>{str(std::vector<Character>{}), str(std::vector<Character>{zero,one,zero})});
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 39
    testDFA(compile(test1));
    testDFA(compile(test2));
    testDFA(compile(test3));
    testDFA(compile(test4));
    testDFA(compile(test5));
    testDFA(compile(test6));

    //task 40
    //manual conversion of test1, and test3, and test4
    DFA<int> test1MC([](int state){return state >= 0 && state < 8;}, alpha, 0, [](int state, Character c){
        switch(state) {
            case 0:
                if(c.equals(Character("0"))){return 0;}
                else{return 1;}
            case 1:
                if(c.equals(Character("1"))){return 2;}
                else{return 3;}
            case 2:
                if(c.equals(Character("0"))){return 7;}
                else{return 6;}
            case 3:
                if(c.equals(Character("0"))){return 4;}
                else{return 5;}
            case 4:
                if(c.equals(Character("1"))){return 5;}
                else{return 4;}
            case 5:
                if(c.equals(Character("0"))){return 7;}
                else{return 6;}
            case 6:
                if(c.equals(Character("0"))){return 7;}
                else{return 6;}
            case 7:
                if(c.equals(Character("0"))){return 4;}
                else{return 5;} 
        }
        return -1;
    }, [](int state){return state > 3 && state < 8;});

    testDFA(test1MC);

    DFA<int> test2MC([](int state){return state >= 1 && state < 7;}, alpha, 1, [](int state, Character c){
        switch(state) {
            case 1:
                if(c.equals(Character("1"))){return 2;}
                else{return 1;}
            case 2:
                if(c.equals(Character("0"))){return 6;}
                else{return 3;}
            case 3:
                if(c.equals(Character("0"))){return 4;}
                else{return 3;}
            case 4:
                if(c.equals(Character("1"))){return 3;}
                else{return 5;}
            case 5:
                if(c.equals(Character("0"))){return 5;}
                else{return 3;}
            case 6:
                if(c.equals(Character("0"))){return 1;}
                else{return 3;}
        }
        return -1;
    }, [](int state){return state > 2 && state < 6;});

    testDFA(test2MC);

    DFA<int> test3MC([](int state){return state >= 1 && state < 7;}, singleCharacter, 1, [](int state, Character c){
        switch(state) {
            case 1:
                return 2;
            case 2:
                return 3;
            case 3:
                return 4;
            case 4:
                return 5;
            case 5:
                return 6;
            case 6:
                return 1;
        }
        return -1;
    }, [](int state){return state != 2 && state != 6;});

    testDFA(test3MC);
    
    //task 43, using task 42 to test
    regex test1reg("empty");
    std::cout << test1reg.printRegex(test1reg) << "\n";

    regex test2reg("epsilon");
    std::cout << test2reg.printRegex(test2reg) << "\n";

    regex test3reg("char", "1");
    std::cout << test3reg.printRegex(test3reg) << "\n";

    regex test4reg("circ", std::vector<regex>{
        regex("char", "0"),
        regex("char", "1")
    });
    std::cout << test4reg.printRegex(test4reg) << "\n";

    regex test5reg("star", std::vector<regex>{
        regex("char", "c")
    });
    std::cout << test5reg.printRegex(test5reg) << "\n";

    regex test6reg("union", std::vector<regex>{
        regex("char", "1"),
        regex("char", "0")
    });
    std::cout << test6reg.printRegex(test6reg) << "\n";

    regex test7reg("circ", std::vector<regex>{
        regex("star", std::vector<regex>{
            regex("union", std::vector<regex>{
                regex("char", "0"), regex("char", "1")
            })
        }),
        regex("circ", std::vector<regex>{
            regex("char", "1"),
            regex("circ", std::vector<regex>{
                regex("union", std::vector<regex>{
                    regex("char", "0"), regex("char", "1")
                }),
                regex("union", std::vector<regex>{
                    regex("char", "0"), regex("char", "1")
                })
            })
        })
    });
    std::cout << test1reg.printRegex(test7reg) << "\n";

    regex test8reg("union", std::vector<regex>{
        regex("circ", std::vector<regex>{
            regex("char", "1"),
            regex("union", std::vector<regex>{
                regex("char", "0"),
                regex("empty")
            })
        }),
        regex("star", std::vector<regex>{
            regex("union", std::vector<regex>{
                regex("char", "0"),
                regex("epsilon")
            })
        })
    });
    std::cout << test8reg.printRegex(test8reg) << "\n";

    regex test9reg("star", std::vector<regex>{
        regex("union", std::vector<regex>{
            regex("circ", std::vector<regex>{
                regex("char", "0"),
                regex("union", std::vector<regex>{
                    regex("empty"),
                    regex("epsilon")
                })
            }),
            regex("star", std::vector<regex>{
                regex("circ", std::vector<regex>{
                    regex("epsilon"),
                    regex("char", "1")
                })
            })
        })
    });
    std::cout << test9reg.printRegex(test9reg) << "\n";

    regex test10reg("circ", std::vector<regex>{
        regex("empty"),
        regex("union", std::vector<regex>{
            regex("char", "1"),
            regex("char", "0")
        })
    });
    std::cout << test10reg.printRegex(test10reg) << "\n";
}