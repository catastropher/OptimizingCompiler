#include <iostream>

#include "Lexer.hpp"
#include "File.hpp"
#include "Ast.hpp"
#include "Parser.hpp"

int main()
{
    try
    {
        std::string input = "5 * 3 % 4";
        
        Lexer lexer(input);
        std::vector<Token>& tokens = lexer.lexTokens();
        
        Parser parser(tokens);
        parser.parse();
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
