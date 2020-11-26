#ifndef REG_HPP
#define REG_HPP
#include <string>

class regex{
    std::string type;
    char c;
    regex* standardOut;
    regex* extraOut;
public:
    regex(std::string type)
        : type(type){};
    regex(std::string type, char c)
        : type(type), c(c){};
    regex(std::string type, regex* standardOut)
        : type(type), standardOut(standardOut){};
    regex(std::string type, regex* standardOut, regex* extraOut)
        : type(type), standardOut(standardOut), extraOut(extraOut){};
};
#endif