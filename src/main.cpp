#include <iostream>

#include "Lexer.hpp"
#include "File.hpp"
#include "Ast.hpp"

int main()
{
    try
    {
        std::string input = readFileContents("../src/movingedge.txt");
        Lexer lexer(input);
        lexer.lexTokens();
    }
    catch(const char* str)
    {
        std::cerr << str << std::endl;
    }
    catch(std::string str)
    {
        std::cerr << str << std::endl;
    }
    
	return 0;
}
