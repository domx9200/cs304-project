#include "character.hpp"
#include "alphabet.hpp"
#include "str.hpp"
#include "DFA.hpp"
#include <list>
#include <iostream>
#include <vector>
#include <optional>
#include <utility>
#include <algorithm>
#include <typeinfo>

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
str task12Helper(std::function<T(T, Character)> &transitions, std::function<bool(T)> &F, T qi, std::vector<Character> &sigma, std::vector<T> visited, Character c)
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
        auto it = std::find(visited.begin(), visited.end(), qNext);
        if(it == visited.end())
        {
            visited.push_back(qNext);
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
        if(qi != visited.at(0)) {
            visited.push_back(qi);
            output = task12Helper(transitions, F, qi, sigma, visited, sigma.at(i));
            if(output.getSize() > 0 && toFind.runDFA(output)) {
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
    return output;
}

//task 14
template <typename T, typename F>
DFA<std::pair<T,F>> unionDFA(DFA<T> DFA1, DFA<F> DFA2){

    std::function<bool(T)> Q1 = DFA1.getQ();
    std::function<bool(F)> Q2 = DFA2.getQ();
    std::function<T(T, Character)> D1 = DFA1.getDelta();
    std::function<F(F, Character)> D2 = DFA2.getDelta();
    std::function<bool(T)> F1 = DFA1.getF();
    std::function<bool(F)> F2 = DFA2.getF();

    DFA<std::pair<T,F>> output([Q1, Q2](std::pair<T,F> state){ return Q1(state.first) && Q2(state.second); }, 
               DFA1.getSigma(), std::pair<T,F>{DFA1.getStart(), DFA2.getStart()}, 
               [D1, D2](std::pair<T,F> state, Character c){
                   T q1 = D1(state.first, c);
                   F q2 = D2(state.second, c);
                   return std::pair<T,F>{q1,q2};
               }, [F1,F2](std::pair<T,F> state){return F1(state.first) || F2(state.second);});
    output.setName("Union of " + DFA1.getName() + " and " + DFA2.getName());
    return output;
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

    auto u = unionDFA(example1_12, oddOnesEvenTotal);
    auto u2 = unionDFA(example1_8, example1_4);
    auto u3 = unionDFA(example1_10, example1_12);
    auto u4 = unionDFA(noAccept, emptyOnly);
    for(int i = 0; i < 20; i++){
        std::cout << u.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u.runDFA(alpha.findNLexo(i)) << "\n";
        std::cout << u2.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u2.runDFA(alpha.findNLexo(i)) << "\n";
        std::cout << u3.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u3.runDFA(alpha.findNLexo(i)) << "\n";
        std::cout << u4.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u4.runDFA(alpha.findNLexo(i)) << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";
    std::cout << "attempting to union a unioned DFA with a non-unioned DFA.\n";
    auto u5 = unionDFA(u, emptyOnly); 
    auto u6 = unionDFA(u2, example1_10); 
    auto u7 = unionDFA(u3, noAccept); 
    auto u8 = unionDFA(u4, oddOnesEvenTotal); 
    for(int i = 0; i < 20; i++){
        std::cout << u5.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u5.runDFA(alpha.findNLexo(i)) << "\n";
        std::cout << u6.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u6.runDFA(alpha.findNLexo(i)) << "\n";
        std::cout << u7.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u7.runDFA(alpha.findNLexo(i)) << "\n";
        std::cout << u8.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u8.runDFA(alpha.findNLexo(i)) << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";
    std::cout << "attempting to union two unioned DFA's.\n";
    auto u9 = unionDFA(u, u2);
    auto u10 = unionDFA(u3, u4);
    auto u11 = unionDFA(u, u3);
    auto u12 = unionDFA(u2, u4);
    for(int i = 0; i < 20; i++){
        std::cout << u9.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u9.runDFA(alpha.findNLexo(i)) << "\n";
        std::cout << u10.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u10.runDFA(alpha.findNLexo(i)) << "\n";
        std::cout << u11.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u11.runDFA(alpha.findNLexo(i)) << "\n";
        std::cout << u12.getName() << " string = " << alpha.findNLexo(i).printable() << " output = " << u12.runDFA(alpha.findNLexo(i)) << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";
}