#include "character.hpp"
#include "alphabet.hpp"
#include "str.hpp"
#include "DFA.hpp"
#include "NFA.hpp"
#include <list>
#include <iostream>
#include <vector>
#include <optional>
#include <utility>
#include <algorithm>
#include <typeinfo>

template <typename T>
void testDFA(DFA<T> toTest)
{
    std::cout << "printing output of DFA " << toTest.getName() << "\n";
    for(int i = 0; i < 20; i++)
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
    //task 26
    //each nfa will be getting 6 test traces
    //traces are setup as the state it is currently at, and the character that will bring it to the next state.
    //the final state will have a character with the value "end", this is so the trace knows it's reached the end.
    std::vector<std::vector<std::pair<int, Character>>> test1Traces;

    // will fail, as it will end on failure state (-1)
    test1Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("")}, std::pair<int, Character>{-1, Character("end")}}); 
    
    // will result in a true, as it ends at state 3
    test1Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("0")}, std::pair<int, Character>{0, Character("1")},
                                                                 std::pair<int, Character>{1, Character("1")}, std::pair<int, Character>{2, Character("1")},
                                                                 std::pair<int, Character>{3, Character("end")}});

    //using the same set of inputs as above this will fail if it goes to 0 instead of 1
    test1Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("0")}, std::pair<int, Character>{0, Character("1")}, 
                                                                 std::pair<int, Character>{0, Character("1")}, std::pair<int, Character>{1, Character("1")},
                                                                 std::pair<int, Character>{2, Character("end")}});
    
    //again using the same inputs, this will also fail, in a different way, due to changing how many times 1 goes to 0
    test1Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("0")}, std::pair<int, Character>{0, Character("1")}, 
                                                                 std::pair<int, Character>{0, Character("1")}, std::pair<int, Character>{0, Character("1")},
                                                                 std::pair<int, Character>{1, Character("end")}});
    
    //test will fail due to not reaching accepting state
    test1Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("1")}, std::pair<int, Character>{0, Character("end")}});

    //same as above, however it does go to state 1 instead of staying at 0
    test1Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("1")}, std::pair<int, Character>{1, Character("end")}});

    NFA<int> test2([](int state){return state <= 3 && state >= 0;}, alpha, 0, [](int state, Character c){
        std::vector<int> possTrans;
        if(c.equals(Character(""))) { 
            possTrans.push_back(-1);
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
    //task 26
    std::vector<std::vector<std::pair<int, Character>>> test2Traces;

    //will fail, if an epsilon transition returns empty, it goes to a fail state (-1)
    test2Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("")}, std::pair<int, Character>{-1, Character("end")}});

    //will also fail
    test2Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("0")}, std::pair<int, Character>{0, Character("end")}});

    //will also fail, however the state brought to from the character 0 is 1, not 0
    test2Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("0")}, std::pair<int, Character>{1, Character("end")}});

    //this will pass
    test2Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("1")}, std::pair<int, Character>{0, Character("1")},
                                                                   std::pair<int, Character>{2, Character("0")}, std::pair<int, Character>{3, Character("end")}});

    //this will fail because at state 2, the input 0 had gone back to state 2
    test2Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("1")}, std::pair<int, Character>{0, Character("1")},
                                                                   std::pair<int, Character>{2, Character("0")}, std::pair<int, Character>{2, Character("end")}});

    //will pass
    test2Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("1")}, std::pair<int, Character>{0, Character("1")},
                                                                   std::pair<int, Character>{2, Character("0")}, std::pair<int, Character>{2, Character("1")},
                                                                   std::pair<int, Character>{3, Character("end")}});

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
    //task 26
    std::vector<std::vector<std::pair<int, Character>>> test3Traces;

    //will fail, the epsilon will have no transitions therefor reach a failure state (-1)
    test3Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{-1, Character("end")}});

    //will pass
    test3Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{1, Character("1")}, std::pair<int, Character>{2, Character("")},
                                                                 std::pair<int, Character>{3, Character("1")}, std::pair<int, Character>{4, Character("end")}});
    
    //the epsilon will have no transitions therefor reach a failure state (-1)
    test3Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{1, Character("1")}, std::pair<int, Character>{1, Character("")},
                                                                 std::pair<int, Character>{-1, Character("1")}, std::pair<int, Character>{-1, Character("end")}});

    //will fail, didn't reach an accepting state.
    test3Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{1, Character("1")}, std::pair<int, Character>{1, Character("1")},
                                                                 std::pair<int, Character>{2, Character("")}, std::pair<int, Character>{3, Character("end")}});

    //will fail, however will reach the same location despite 0 being in place of epsilon
    test3Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{1, Character("1")}, std::pair<int, Character>{1, Character("1")},
                                                                 std::pair<int, Character>{2, Character("0")}, std::pair<int, Character>{3, Character("end")}});
    
    //will pass, added the last character 1 which brought us to state 4, an accepting state
    test3Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{1, Character("1")}, std::pair<int, Character>{1, Character("1")},
                                                                 std::pair<int, Character>{2, Character("0")}, std::pair<int, Character>{3, Character("1")},
                                                                 std::pair<int, Character>{4, Character("end")}});

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
    //task 26
    std::vector<std::vector<std::pair<int, Character>>> test4Traces;

    //fails because it goes to a failure state, the first state, 0, only has epsilon transitions
    test4Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("0")}, std::pair<int, Character>{-1, Character("end")}});

    //passes because the epsilon transition brought us to state 1, which is accepting
    test4Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("")}, std::pair<int, Character>{1, Character("end")}});

    //passes as well, because the epsiilon transition brought us to state 3, which is also
    test4Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("")}, std::pair<int, Character>{3, Character("end")}});

    //fails because the end state isn't in accepting states
    test4Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("")}, std::pair<int, Character>{3, Character("0")},
                                                                 std::pair<int, Character>{4, Character("end")}});
    
    //fail because the end state isn't in accepting states
    test4Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("")}, std::pair<int, Character>{3, Character("0")},
                                                                 std::pair<int, Character>{4, Character("0")}, std::pair<int, Character>{5, Character("end")}});

    //passes because end state is in accepting states
    test4Traces.push_back(std::vector<std::pair<int, Character>>{std::pair<int, Character>{0, Character("")}, std::pair<int, Character>{1, Character("0")},
                                                                 std::pair<int, Character>{2, Character("0")}, std::pair<int, Character>{1, Character("end")}});

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
        return possTrans;
    }, [](int state){ return state == 1;});
    //task 26
    std::vector<std::vector<std::pair<int, Character>>> test5Traces;

    //passes, ended at an accepting state.
    test5Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{3, Character("0")},
                                                                  std::pair<int, Character>{1, Character("end")}});

    // //fails, goes to failure state if transition vector is empty
    test5Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{3, Character("1")},
                                                                  std::pair<int, Character>{-1, Character("end")}});

    // //fails, doesn't reach an accepting state
    test5Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("1")}, std::pair<int, Character>{2, Character("0")},
                                                                  std::pair<int, Character>{3, Character("end")}});

    // //is an accepting state, passes
    test5Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("1")}, std::pair<int, Character>{2, Character("0")},
                                                                  std::pair<int, Character>{2, Character("0")}, std::pair<int, Character>{3, Character("0")},
                                                                  std::pair<int, Character>{1, Character("end")}});

    // //fails, went to failing state by attempting epsilon transition without a transition
    test5Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{3, Character("")},
                                                                  std::pair<int, Character>{-1, Character("end")}});

    // //fails, isn't at an accepting state at the end of the string
    test5Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{3, Character("0")},
                                                                  std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{3, Character("end")}});

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
        return possTrans;
    }, [](int state){ return state == 3;});
    //task 26
    std::vector<std::vector<std::pair<int, Character>>> test6Traces;

    //fails, didn't end at an accepting state
    test6Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{3, Character("0")},
                                                                  std::pair<int, Character>{4, Character("end")}});

    //fails, goes to failure state if transition vector is empty
    test6Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{3, Character("1")},
                                                                  std::pair<int, Character>{-1, Character("end")}});

    //fails, doesn't reach an accepting state
    test6Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{2, Character("1")},
                                                                  std::pair<int, Character>{4, Character("end")}});

    // //is an accepting state, passes
    test6Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("0")}, std::pair<int, Character>{2, Character("1")},
                                                                  std::pair<int, Character>{4, Character("0")}, std::pair<int, Character>{3, Character("end")}});

    // //fails, wasn't at an accepting state
    test6Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{3, Character("")},
                                                                  std::pair<int, Character>{2, Character("end")}});

    // //fails, it found an empty transition table, so it goes to the failure state
    test6Traces.push_back(std::vector<std::pair<int, Character>> {std::pair<int, Character>{1, Character("")}, std::pair<int, Character>{3, Character("0")},
                                                                  std::pair<int, Character>{4, Character("")}, std::pair<int, Character>{-1, Character("end")}});
}