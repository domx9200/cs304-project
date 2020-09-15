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
    str trace;
public:
    DFA(){};
    virtual bool runDFA(str input){return false;};
    str getTrace() {return trace;};
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
        trace.clearStr();
        trace.addCharToStr(states.at(current).getString());
        bool temp = false;
        while(input.getSize() != 0)
        {
            input.popValue();
            trace.addCharToStr(states.at(current).getString());
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
        trace.clearStr();
        trace.addCharToStr(states.at(current).getString());
        bool temp = false;
        while(input.getSize() != 0)
        {
            input.popValue();
            current = 1;
            trace.addCharToStr(states.at(current).getString());
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
        void clearStr();
        trace.addCharToStr(states.at(current).getString());
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
            trace.addCharToStr(states.at(current).getString());
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
        trace.clearStr();
        trace.addCharToStr(states.at(current).getString());
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
            trace.addCharToStr(states.at(current).getString());
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
        trace.clearStr();
        trace.addCharToStr(states.at(current).getString());
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
            trace.addCharToStr(states.at(current).getString());
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
        trace.clearStr();
        trace.addCharToStr(states.at(current).getString());
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
            trace.addCharToStr(states.at(current).getString());
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
        trace.clearStr();
        trace.addCharToStr(states.at(current).getString());
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
            trace.addCharToStr(states.at(current).getString());
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
        trace.clearStr();
        trace.addCharToStr(states.at(current).getString());
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
            trace.addCharToStr(states.at(current).getString());
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
        trace.clearStr();
        trace.addCharToStr(states.at(current).getString());
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
            trace.addCharToStr(states.at(current).getString());
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

bool detDFA(DFA *toRun, str input)
{
    return toRun->runDFA(input);
} 

str outputString(DFA *toRun, str input)
{
    detDFA(toRun, input);
    return toRun->getTrace();
}

int main()
{
    alphabet alpha;
    Character temp("0");
    Character temp2("1");
    alpha.addCharToAlphabet(temp);
    alpha.addCharToAlphabet(temp2);
    
    alpha.printAlphabet();
    std::cout << "\n";

    str string;
    string.addCharToStr("0");
    string.addCharToStr("1");
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
    str onlyZero;
    onlyZero.addCharToStr("0");

    DFAZeroOnly zero;
    std::cout << "output of DFAZeroOnly using lexo: " << zero.runDFA(lexo) << "\n";
    std::cout << "output of DFAZeroOnly using onlyZero: " << zero.runDFA(onlyZero) << "\n";
    std::cout << "output of DFAZeroOnly using string: " << zero.runDFA(string) << "\n";

    std::cout << "\n";
    DFAFigure1_4 test1;
    alphabet test1Alpha = alpha;
    for(int i = 0; i < 20; i++)
    {
        std::cout << "using "<< test1Alpha.findNLexo(i).printable() <<" at " << i << " for DFAFigure1_4:    " << test1.runDFA(test1Alpha.findNLexo(i)) << "\n";
        std::cout << "trace path using above: " << test1.getTrace().printable() << "\n\n";
    }

    std::cout << "\n";
    DFAFigure1_2 test2;
    alphabet test2Alpha;
    test2Alpha.addCharToAlphabet(Character("FRONT"));
    test2Alpha.addCharToAlphabet(Character("BOTH"));
    test2Alpha.addCharToAlphabet(Character("NEITHER"));
    test2Alpha.addCharToAlphabet(Character("REAR"));
    for(int i = 0; i < 20; i++)
    {
        std::cout << "using " << test2Alpha.findNLexo(i).printable() << " at " << i << " for DFAFigure1_2:    " << test2.runDFA(test2Alpha.findNLexo(i)) << "\n";
        std::cout << "trace path using above: " << test2.getTrace().printable() << "\n\n";
    }

    std::cout << "\n";
    DFAFigure1_8 test3;
    alphabet test3Alpha = alpha;
    for(int i = 0; i < 20; i++)
    {
        std::cout << "using " << test3Alpha.findNLexo(i).printable() << " at " << i << " for DFAFigure1_8:    " << test3.runDFA(test3Alpha.findNLexo(i)) << "\n";
        std::cout << "trace path using above: " << test3.getTrace().printable() << "\n\n";
    }

    std::cout << "\n";
    DFAFigure1_12 test4;
    alphabet test4Alpha = alpha;
    for(int i = 0; i < 20; i++)
    {
        std::cout << "using " << test4Alpha.findNLexo(i).printable() << " at " << i << " for DFAFigure1_12:   " << test4.runDFA(test4Alpha.findNLexo(i)) << "\n";
        std::cout << "trace path using above: " << test4.getTrace().printable() << "\n\n";
    }

    std::cout << "\n";
    DFAFigure1_14 test5;
    alphabet test5Alpha;
    test5Alpha.addCharToAlphabet(Character("RESET"));
    test5Alpha.addCharToAlphabet(Character("0"));
    test5Alpha.addCharToAlphabet(Character("1"));
    test5Alpha.addCharToAlphabet(Character("2"));
    for(int i = 0; i < 20; i++)
    {
        std::cout << "using " << test5Alpha.findNLexo(i).printable() << " at " << i << " for DFAFigure1_14:   " << test5.runDFA(test5Alpha.findNLexo(i)) << "\n";
        std::cout << "trace path using above: " << test5.getTrace().printable() << "\n\n";
    }

    std::cout << "\n";
    DFAFigure1_20 test6;
    alphabet test6Alpha = alpha;
    for(int i = 0; i < 20; i++)
    {
        std::cout << "using " << test6Alpha.findNLexo(i).printable() << " at " << i << " for DFAFigure1_20:   " << test6.runDFA(test6Alpha.findNLexo(i)) << "\n";
        std::cout << "trace path using above: " << test6.getTrace().printable() << "\n\n";
    }

    std::cout << "\nrunning DFAFigure1_4 with detDFA and outputString:\n";
    for(int i = 0; i < 20; i++)
    {
        std::cout << "string: " << test1Alpha.findNLexo(i).printable() << " result: " << detDFA(&test1, test1Alpha.findNLexo(i)) << "\n";
        std::cout << "trace: " << outputString(&test1, test1Alpha.findNLexo(i)).printable() << "\n\n";
    }

    std::cout << "\nrunning DFAFigure1_2 with detDFA and outputString:\n";
    for(int i = 0; i < 20; i++)
    {
        std::cout << "string: " << test2Alpha.findNLexo(i).printable() << " result: " << detDFA(&test2, test2Alpha.findNLexo(i)) << "\n";
        std::cout << "trace: " << outputString(&test2, test2Alpha.findNLexo(i)).printable() << "\n\n";
    }

    std::cout << "\nrunning DFAFigure1_8 with detDFA and outputString:\n";
    for(int i = 0; i < 20; i++)
    {
        std::cout << "string: " << test3Alpha.findNLexo(i).printable() << " result: " << detDFA(&test3, test3Alpha.findNLexo(i)) << "\n";
        std::cout << "trace: " << outputString(&test3, test3Alpha.findNLexo(i)).printable() << "\n\n";
    }

    std::cout << "\nrunning DFAFigure1_12 with detDFA and outputString:\n";
    for(int i = 0; i < 20; i++)
    {
        std::cout << "string: " << test4Alpha.findNLexo(i).printable() << " result: " << detDFA(&test4, test4Alpha.findNLexo(i)) << "\n";
        std::cout << "trace: " << outputString(&test4, test4Alpha.findNLexo(i)).printable() << "\n\n";
    }

    std::cout << "\nrunning DFAFigure1_14 with detDFA and outputString:\n";
    for(int i = 0; i < 20; i++)
    {
        std::cout << "string: " << test5Alpha.findNLexo(i).printable() << " result: " << detDFA(&test5, test5Alpha.findNLexo(i)) << "\n";
        std::cout << "trace: " << outputString(&test5, test5Alpha.findNLexo(i)).printable() << "\n\n";
    }

    std::cout << "\nrunning DFAFigure1_20 with detDFA and outputString:\n";
    for(int i = 0; i < 20; i++)
    {
        std::cout << "string: " << test6Alpha.findNLexo(i).printable() << " result: " << detDFA(&test6, test6Alpha.findNLexo(i)) << "\n";
        std::cout << "trace: " << outputString(&test6, test6Alpha.findNLexo(i)).printable() << "\n\n";
    }

    std::cout << "comparing results with DFAFigure1_20 to running with detDFA and without to show same output... also doing the same for outputString:\n";
    for(int i = 0; i < 20; i++)
    {
        std::cout << "non detDFA: " << test6.runDFA(test6Alpha.findNLexo(i)) << " with detDFA: " << detDFA(&test6, test6Alpha.findNLexo(i)) << "\n";
        std::cout << "non outputString: " << test6.getTrace().printable() << " with outputString: " << outputString(&test6, test6Alpha.findNLexo(i)).printable() << "\n";
    }
}