#include "Lexer.hpp"

int main()
{
    std::string input = "let x = 5";
    
    Lexer lexer(input);
    
    lexer.lexTokens();
    
	return 0;
}
