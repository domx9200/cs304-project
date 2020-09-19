#include "character.hpp"
#include "alphabet.hpp"
#include "str.hpp"
#include "DFA.hpp"
#include <list>
#include <iostream>
#include <vector>

bool detDFA(DFA toRun, str input)
{
    return toRun.runDFA(input);
}

str getTrace(DFA toRun, str input)
{
    toRun.runDFA(input);
    return toRun.getTrace();
}

int main()
{
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

    //end task 1-3
    //task 4-5
    std::vector<state> temp; std::vector<state> empty;
    state onlyState;
    onlyState.addIndex(0);
    onlyState.addName("onlyState");
    onlyState.addTransition("===",0);
    temp.push_back(onlyState);
    DFA noAccept(temp, temp.at(0), empty, alpha);
    std::cout << "dfa noAccept.runDFA(string): " << noAccept.runDFA(string) << "\n";
    std::cout << "trace: " << noAccept.getTrace().printable() << "\n";
    std::cout << "dfa noAccept.runDFA(alpha.findNLexo(9)): " << noAccept.runDFA(alpha.findNLexo(9)) << "\n";
    std::cout << "trace: " << noAccept.getTrace().printable() << "\n-----------------------------------------------------------------------------------\n";
    //task 4-5 end

    //task 6
    std::vector<state> states;
    std::vector<state> accepting;
    state statePass;
    statePass.addIndex(0);
    statePass.addName("0");
    statePass.addTransition("===",1);
    states.push_back(statePass);
    accepting.push_back(statePass);

    state stateFail;
    stateFail.addIndex(1);
    stateFail.addName("1");
    stateFail.addTransition("===",1);
    states.push_back(stateFail);
    DFA onlyEmpty(states, states.at(0), accepting, alpha);

    std::cout << "dfa onlyEmpty.runDFA(string): " << onlyEmpty.runDFA(string) << "\n";
    std::cout << "output trace: " << onlyEmpty.getTrace().printable() << "\n";
    std::cout << "dfa onlyEmpty.runDFA(alpha.findNLexo(0)) prints \"\": " << onlyEmpty.runDFA(alpha.findNLexo(0)) << "\n";
    std::cout << "output trace: " << onlyEmpty.getTrace().printable() << "\n-----------------------------------------------------------------------------------\n";
    //task 6 end

    //task 7 test
    DFA task7;
    task7 = task7.createCharDFA(Character("0"));
    std::cout << "dfa task7.runDFA(alpha.findNLexo(2)) prints \"1\": " << task7.runDFA(alpha.findNLexo(2)) << "\n";
    std::cout << "output trace: " << task7.getTrace().printable() << "\n";
    std::cout << "dfa task7.runDFA(alpha.findNLexo(1)) prints \"0\": " << task7.runDFA(alpha.findNLexo(1)) << "\n";
    std::cout << "output trace: " << task7.getTrace().printable() << "\n";
    std::cout << "dfa task7.runDFA(alpha.findNLexo(3)) prints \"00\": " << task7.runDFA(alpha.findNLexo(3)) << "\n";
    std::cout << "output trace: " << task7.getTrace().printable() << "\n-----------------------------------------------------------------------------------\n";
    //task 7 test end

    //task 9 is done alongside task 8, strings are lexo functions using
    //alphabet inside each dfa.

    //task 8 dfa1 : book example 1_2
    //modified to have closed be start point and open as accepting state
    DFA test; test.createExam1_2();
    alphabet testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "dfa exam1_2 test.runDFA(testAlpha.findNLexo(" << i << ")) prints " << testAlpha.findNLexo(i).printable() << ": " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "output trace: " << test.getTrace().printable() << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 8 dfa2 : book example 1_4
    test.createExam1_4();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "dfa exam1_4 test.runDFA(testAlpha.findNLexo(" << i << ")) prints " << testAlpha.findNLexo(i).printable() << ": " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "output trace: " << test.getTrace().printable() << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 8 dfa3 : book example 1_8
    test.createExam1_8();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "dfa exam1_8 test.runDFA(testAlpha.findNLexo(" << i << ")) prints " << testAlpha.findNLexo(i).printable() << ": " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "output trace: " << test.getTrace().printable() << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 8 dfa4 : book example 1_10
    test.createExam1_10();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "dfa exam1_10 test.runDFA(testAlpha.findNLexo(" << i << ")) prints " << testAlpha.findNLexo(i).printable() << ": " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "output trace: " << test.getTrace().printable() << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 8 dfa5 : book example 1_12
    test.createExam1_12();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "dfa exam1_10 test.runDFA(testAlpha.findNLexo(" << i << ")) prints " << testAlpha.findNLexo(i).printable() << ": " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "output trace: " << test.getTrace().printable() << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 8 dfa6 : book example 1_14
    test.createExam1_14();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "dfa exam1_10 test.runDFA(testAlpha.findNLexo(" << i << ")) prints " << testAlpha.findNLexo(i).printable() << ": " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "output trace: " << test.getTrace().printable() << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 8 dfa7 : made dfa 1
    //checks if hey is in the string
    test.createMade1();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "dfa exam1_10 test.runDFA(testAlpha.findNLexo(" << i << ")) prints " << testAlpha.findNLexo(i).printable() << ": " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "output trace: " << test.getTrace().printable() << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 8 dfa8 : made dfa 2
    //checks if there are an odd number of ones and is even.
    test.createMade2();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "dfa exam1_10 test.runDFA(testAlpha.findNLexo(" << i << ")) prints " << testAlpha.findNLexo(i).printable() << ": " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "output trace: " << test.getTrace().printable() << "\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    //task 10-11 check
    test.createExam1_2();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "running detDFA with exam1_2: " << detDFA(test, testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace function output: " << getTrace(test, testAlpha.findNLexo(i)).printable() << "\n";
        std::cout << "running without detDFA with exam1_2: " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace without function: " << test.getTrace().printable() << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    test.createExam1_4();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "running detDFA with exam1_4: " << detDFA(test, testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace function output: " << getTrace(test, testAlpha.findNLexo(i)).printable() << "\n";
        std::cout << "running without detDFA with exam1_4: " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace without function: " << test.getTrace().printable() << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    test.createExam1_8();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "running detDFA with exam1_8: " << detDFA(test, testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace function output: " << getTrace(test, testAlpha.findNLexo(i)).printable() << "\n";
        std::cout << "running without detDFA with exam1_8: " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace without function: " << test.getTrace().printable() << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    test.createExam1_10();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "running detDFA with exam1_10: " << detDFA(test, testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace function output: " << getTrace(test, testAlpha.findNLexo(i)).printable() << "\n";
        std::cout << "running without detDFA with exam1_10: " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace without function: " << test.getTrace().printable() << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    test.createExam1_12();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "running detDFA with exam1_12: " << detDFA(test, testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace function output: " << getTrace(test, testAlpha.findNLexo(i)).printable() << "\n";
        std::cout << "running without detDFA with exam1_12: " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace without function: " << test.getTrace().printable() << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    test.createExam1_14();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "running detDFA with exam1_14: " << detDFA(test, testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace function output: " << getTrace(test, testAlpha.findNLexo(i)).printable() << "\n";
        std::cout << "running without detDFA with exam1_14: " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace without function: " << test.getTrace().printable() << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    test.createMade1();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "running detDFA with made1: " << detDFA(test, testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace function output: " << getTrace(test, testAlpha.findNLexo(i)).printable() << "\n";
        std::cout << "running without detDFA with made1: " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace without function: " << test.getTrace().printable() << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";

    test.createMade2();
    testAlpha = test.getAlpha();
    for(int i = 0; i < 21; i++)
    {
        std::cout << "running detDFA with createMade2: " << detDFA(test, testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace function output: " << getTrace(test, testAlpha.findNLexo(i)).printable() << "\n";
        std::cout << "running without detDFA with createMade2: " << test.runDFA(testAlpha.findNLexo(i)) << "\n";
        std::cout << "trace without function: " << test.getTrace().printable() << "\n\n";
    }
    std::cout << "-----------------------------------------------------------------------------------\n";
}