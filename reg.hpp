#ifndef REG_HPP
#define REG_HPP
#include <string>
#include <optional>
#include <vector>
#include <iostream>

class regex{
    std::string type;
    std::string c;
    std::vector<regex> inputs;
public:
    regex(std::string type)
        : type(type){};
    regex(std::string type, std::string c)
        : type(type), c(c){};
    regex(std::string type, std::vector<regex> inputs)
        : type(type), inputs(inputs){};
    
    std::vector<regex> getInputs(){return inputs;};
    std::string getChar(){return c;};
    std::string getType(){return type;};

    std::string printRegex(regex current){
        if(current.getType().compare("char") == 0){
            return current.getChar();
        } else if(current.getType().compare("epsilon") == 0)
            return "ε";
        else if(current.getType().compare("empty") == 0)
            return "∅";
        else if(current.getType().compare("union") == 0) {
            std::string lhs = printRegex(current.getInputs().at(0));
            std::string rhs = printRegex(current.getInputs().at(1));
            return "(" + lhs + " U " + rhs + ")";
        } else if(current.getType().compare("star") == 0) {
            std::string input = printRegex(current.getInputs().at(0));
            return "(" + input + ")*";
        } else {
            std::string lhs = printRegex(current.getInputs().at(0));
            std::string rhs = printRegex(current.getInputs().at(1));
            return "(" + lhs + " circ " + rhs + ")";
        }
    };
};
#endif