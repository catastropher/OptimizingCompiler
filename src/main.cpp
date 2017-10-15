#include "Lexer.hpp"

int main()
{
    std::string input = "for idx = 0 to bound - 1 by 1";
    
    Lexer lexer(input);
    
    lexer.lexTokens();
    
	return 0;
}
