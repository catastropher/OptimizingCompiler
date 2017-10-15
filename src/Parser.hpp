#pragma once

#include <vector>

#include "Token.hpp"

class Parser
{
public:
    Parser(std::vector<Token>& tokens_);
    
private:
    std::vector<Token>& tokens;
};

