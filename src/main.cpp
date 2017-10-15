#include "Lexer.hpp"

int main()
{
    std::string input = "while (list[5] < bound)";
    
    Lexer lexer(input);
    
    lexer.lexTokens();
    
	return 0;
}
