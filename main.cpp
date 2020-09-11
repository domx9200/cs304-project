#include "character.hpp"
#include "alphabet.hpp"
#include "str.hpp"
#include <list>
#include <iostream>
#include <vector>

class DFA
{
protected:
    std::vector<Character> states;
    Character start;
    std::vector<Character> accept;
    int current;
public:
    DFA(){};
    bool runDFA(str input);
};

class DFANoAccept: public DFA
{
public:
    DFANoAccept()
    {
        states = std::vector<Character> {Character("0")};
        start = Character("0");
        current = 0;
    }

    bool runDFA(str input)
    {
        bool temp = false;
        while(input.getSize() != 0)
        {
            input.popValue();
        }
        for(int i = 0; i < (int) accept.size(); i++)
        {
            if(states.at(current).equals(accept.at(i)))
            {
                temp = true;
            }
        }
        temp = false;
        return temp;
    }
    
};

class DFAEmptyOnly: public DFA
{
public:
    DFAEmptyOnly()
    {
        states = std::vector<Character> {Character("0"), Character("1")};
        this->start = Character("0");
        this->accept = std::vector<Character> {Character("0")};
        current = 0;
    }

    bool runDFA(str input)
    {
        bool temp = false;
        while(input.getSize() != 0)
        {
            input.popValue();
            current = 1;
        }
        for(int i = 0; i < (int) accept.size(); i++)
        {
            if(states.at(current).equals(accept.at(i)))
            {
                temp = true;
            }
        }
        current = 0;
        return temp;
    }
};

class DFAZeroOnly: public DFA
{
public:
    DFAZeroOnly()
    {
        states = std::vector<Character> {Character("0"), Character("1"), Character("2")};
        start = Character("0");
        accept = std::vector<Character> {Character("1")};
        current = 0;
    }

    bool runDFA(str input)
    {
        bool temp = false;
        while(input.getSize() != 0)
        {
            Character temp = input.popValue();
            switch(current) 
            {
                case 0:
                    if(temp.equals(Character("0")))
                    {
                        current = 1;
                    }
                    else
                    {
                        current = 2;
                    } 
                    break;   
                case 1:
                    current = 2;
                    break;
            }
        }
        for(int i = 0; i < (int) accept.size(); i++)
        {
            if(states.at(current).equals(accept.at(i)))
            {
                temp = true;
            }
        }
        current = 0;
        return temp;
    }
};

class DFAFigure1_4: public DFA
{
public:
    DFAFigure1_4()
    {
        states = std::vector<Character> {Character("0"), Character("1"), Character("2")};
        start = Character("0");
        accept = std::vector<Character> {Character("1")};
        current = 0;
    }

    bool runDFA(str input)
    {
        bool temp = false;
        while(input.getSize() != 0)
        {
            Character temp = input.popValue();
            switch(current) 
            {
                case 0:
                    if(temp.equals(Character("1")))
                    {
                        current = 1;
                    }
                    break;   
                case 1:
                    if(temp.equals(Character("0")))
                    {
                        current = 2;
                    }
                    break;
                case 2:
                    current = 1;
                    break;
            }
        }
        for(int i = 0; i < (int) accept.size(); i++)
        {
            if(states.at(current).equals(accept.at(i)))
            {
                temp = true;
            }
        }
        current = 0;
        return temp;
    }
};

class DFAFigure1_2: public DFA
{
    //slightly modified to include a start state and an accepting state
    //that is, when it is open it is accepting.
public:
    DFAFigure1_2()
    {
        states = std::vector<Character> {Character("CLOSED"), Character("OPEN")};
        start = Character("CLOSED");
        accept = std::vector<Character> {Character("OPEN")};
        current = 0;
    }

    bool runDFA(str input)
    {
        bool temp = false;
        while(input.getSize() != 0)
        {
            Character temp = input.popValue();
            switch(current) 
            {
                case 0:
                    if(temp.equals(Character("FRONT")))
                    {
                        current = 1;
                    }
                    break;
                case 1:
                    if(temp.equals(Character("NEITHER")))
                    {
                        current = 0;
                    }
                    break;
            }
        }
        for(int i = 0; i < (int) accept.size(); i++)
        {
            if(states.at(current).equals(accept.at(i)))
            {
                temp = true;
            }
        }
        current = 0;
        return temp;
    }
};

class DFAFigure1_8: public DFA
{
public:
    DFAFigure1_8()
    {
        states = std::vector<Character> {Character("0"), Character("1")};
        start = Character("0");
        accept = std::vector<Character> {Character("1")};
        current = 0;
    }

    bool runDFA(str input)
    {
        bool temp = false;
        while(input.getSize() != 0)
        {
            Character temp = input.popValue();
            switch(current) 
            {
                case 0:
                    if(temp.equals(Character("1")))
                    {
                        current = 1;
                    }
                    break;
                case 1:
                    if(temp.equals(Character("0")))
                    {
                        current = 0;
                    }
                    break;
            }
        }
        for(int i = 0; i < (int) accept.size(); i++)
        {
            if(states.at(current).equals(accept.at(i)))
            {
                temp = true;
            }
        }
        current = 0;
        return temp;
    }
};

class DFAFigure1_12: public DFA
{
public:
    DFAFigure1_12()
    {
        states = std::vector<Character> {Character("0"), Character("1"), Character("2"), Character("3"), Character("4")};
        start = Character("2");
        accept = std::vector<Character> {Character("1"), Character("3")};
        current = 2;
    }

    bool runDFA(str input)
    {
        bool temp = false;
        while(input.getSize() != 0)
        {
            Character temp = input.popValue();
            switch(current) 
            {
                case 2:
                    if(temp.equals(Character("0")))
                    {
                        current = 1;
                    }
                    else if(temp.equals(Character("1")))
                    {
                        current = 3;
                    }
                    break;
                case 1:
                    if(temp.equals(Character("1")))
                    {
                        current = 0;
                    }
                    break;
                case 0:
                    if(temp.equals(Character("0")))
                    {
                        current = 1;
                    }
                    break;
                case 3:
                    if(temp.equals(Character("0")))
                    {
                        current = 4;
                    }
                    break;
                case 4:
                    if(temp.equals(Character("1")))
                    {
                        current = 3;
                    }
                    break;
            }
        }
        for(int i = 0; i < (int) accept.size(); i++)
        {
            if(states.at(current).equals(accept.at(i)))
            {
                temp = true;
            }
        }
        current = 2;
        return temp;
    }
};

class DFAFigure1_14: public DFA
{
public:
    DFAFigure1_14()
    {
        states = std::vector<Character> {Character("0"), Character("1"), Character("2")};
        start = Character("0");
        accept = std::vector<Character> {Character("0")};
        current = 0;
    }

    bool runDFA(str input)
    {
        bool temp = false;
        while(input.getSize() != 0)
        {
            Character temp = input.popValue();
            switch(current) 
            {
                case 0:
                    if(temp.equals(Character("1")))
                    {
                        current = 1;
                    }
                    else if(temp.equals(Character("2")))
                    {
                        current = 2;
                    }
                    break;
                case 1:
                    if(temp.equals(Character("1")))
                    {
                        current = 2;
                    }
                    else if(temp.equals(Character("2")) || temp.equals(Character("RESET")))
                    {
                        current = 0;
                    }
                    break;
                case 2:
                    if(temp.equals(Character("1")) || temp.equals(Character("RESET")))
                    {
                        current = 0;
                    }
                    else if(temp.equals(Character("2")))
                    {
                        current = 1;
                    }
                    break;
            }
        }
        for(int i = 0; i < (int) accept.size(); i++)
        {
            if(states.at(current).equals(accept.at(i)))
            {
                temp = true;
            }
        }
        current = 0;
        return temp;
    }
};

class DFAFigure1_20: public DFA
{
public:
    DFAFigure1_20()
    {
        states = std::vector<Character> {Character("EVEN"), Character("ODD")};
        start = Character("EVEN");
        accept = std::vector<Character> {Character("ODD")};
        current = 0;
    }

    bool runDFA(str input)
    {
        bool temp = false;
        while(input.getSize() != 0)
        {
            Character temp = input.popValue();
            switch(current) 
            {
                case 0:
                    if(temp.equals(Character("1")))
                    {
                        current = 1;
                    }
                    break;
                case 1:
                    if(temp.equals(Character("1")))
                    {
                        current = 0;
                    }
                    break;
            }
        }
        for(int i = 0; i < (int) accept.size(); i++)
        {
            if(states.at(current).equals(accept.at(i)))
            {
                temp = true;
            }
        }
        current = 0;
        return temp;
    }
};

int main()
{
    alphabet alpha;
    Character temp("0");
    Character temp2("1");
    Character temp3("2");
    alpha.addCharToAlphabet(temp);
    alpha.addCharToAlphabet(temp2);
    alpha.addCharToAlphabet(temp3);
    
    alpha.printAlphabet();
    std::cout << "\n";

    str string;
    string.addCharToStr(temp);
    string.addCharToStr(temp2);
    std::cout << "string prior to find lexo: ";
    string.printStr();
    std::cout << "\n";

    std::cout << "string after find lexo: ";
    str lexo = alpha.findNLexo(9);
    lexo.printStr();
    std::cout << "\n";
    str emptyString;

    DFANoAccept nAccept;
    std::cout << "output of DFANoAccept using variable string: " << nAccept.runDFA(string) << "\n";
    std::cout << "output of DFANoAccept using variable lexo: " << nAccept.runDFA(lexo) << "\n";

    DFAEmptyOnly empty;
    std::cout << "output of DFAEmptyOnly using variable string: " << empty.runDFA(string) << "\n";
    std::cout << "output of DFAEmptyOnly using variable emptystring: " << empty.runDFA(emptyString) << "\n";

    lexo.popValue();
    temp = Character("0");
    str onlyZero;
    onlyZero.addCharToStr(temp);

    DFAZeroOnly zero;
    std::cout << "output of DFAZeroOnly using lexo: " << zero.runDFA(lexo) << "\n";
    std::cout << "output of DFAZeroOnly using onlyZero: " << zero.runDFA(onlyZero) << "\n";
    std::cout << "output of DFAZeroOnly using string: " << zero.runDFA(string) << "\n";

    DFAFigure1_4 test1;
    str test1Pass;
    test1Pass.addCharToStr(Character("0"));
    test1Pass.addCharToStr(Character("1"));
    test1Pass.addCharToStr(Character("1"));
    test1Pass.addCharToStr(Character("0"));
    test1Pass.addCharToStr(Character("1"));
    std::cout << "output of DFAFigure1_4 using test1pass: " << test1.runDFA(test1Pass) << "\n";

    str test1fail = test1Pass;
    test1fail.addCharToStr(Character("0"));
    std::cout << "output of DFAFigure1_4 using test1fail: " << test1.runDFA(test1fail) << "\n";

    DFAFigure1_2 test2;
    str test2Pass;
    test2Pass.addCharToStr(Character("REAR"));
    test2Pass.addCharToStr(Character("FRONT"));
    test2Pass.addCharToStr(Character("FRONT"));
    std::cout << "output of DFAFigure1_2 using test2pass: " << test2.runDFA(test2Pass) << "\n";

    str test2Fail = test2Pass;
    test2Fail.addCharToStr(Character("NEITHER"));
    std::cout << "output of DFAFigure1_2 using test2Fail: " << test2.runDFA(test2Fail) << "\n";

    DFAFigure1_8 test3;
    str test3pass;
    test3pass.addCharToStr(Character("0"));
    test3pass.addCharToStr(Character("1"));
    test3pass.addCharToStr(Character("1"));
    std::cout << "output of DFAFigure1_8 using test3pass: " << test3.runDFA(test3pass) << "\n";

    str test3fail = test3pass;
    test3fail.addCharToStr(Character("0"));
    std::cout << "output of DFAFigure1_8 using test3fail: " << test3.runDFA(test3fail) << "\n";

    DFAFigure1_12 test4;
    str test4pass;
    test4pass.addCharToStr(Character("0"));
    test4pass.addCharToStr(Character("1"));
    test4pass.addCharToStr(Character("1"));
    test4pass.addCharToStr(Character("0"));
    std::cout << "output of DFAFigure1_12 using test4pass: " << test4.runDFA(test4pass) << "\n";

    str test4fail = test4pass;
    test4fail.addCharToStrFront(Character("1"));
    std::cout << "output of DFAFigure1_12 using test4fail: " << test4.runDFA(test4fail) << "\n";

    DFAFigure1_14 test5;
    str test5pass;
    test5pass.addCharToStr(Character("1"));
    test5pass.addCharToStr(Character("1"));
    test5pass.addCharToStr(Character("2"));
    test5pass.addCharToStr(Character("2"));
    std::cout << "output of DFAFigure1_14 using test5pass: " << test5.runDFA(test5pass) << "\n";

    str test5fail = test5pass;
    test5fail.addCharToStr(Character("1"));
    std::cout << "output of DFAFigure1_14 using test5fail: " << test5.runDFA(test5fail) << "\n";

    DFAFigure1_20 test6;
    str test6pass;
    test6pass.addCharToStr(Character("1"));
    test6pass.addCharToStr(Character("1"));
    test6pass.addCharToStr(Character("1"));
    std::cout << "output of DFAFigure1_20 using test6pass: " << test6.runDFA(test6pass) << "\n";

    str test6fail = test6pass;
    test6fail.addCharToStr(Character("1"));
    std::cout << "output of DFAFigure1_20 using test6fail: " << test6.runDFA(test6fail) << "\n";
}