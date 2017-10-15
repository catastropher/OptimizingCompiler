#include "Lexer.hpp"

int main()
{
    std::string input = "    12345";
    
    Lexer lexer(input);
    
    lexer.lexTokens();
    
	return 0;
}
