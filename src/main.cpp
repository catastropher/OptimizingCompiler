#include <iostream>

#include "Lexer.hpp"
#include "File.hpp"
#include "Ast.hpp"
#include "Parser.hpp"
#include "CodeGenerator.hpp"

int main()
{
    try
    {
        std::string input = readFileContents("../src/test.txt");
        
        Lexer lexer(input);
        std::vector<Token>& tokens = lexer.lexTokens();
        
        Parser parser(tokens);
        Ast& ast = parser.parse();
        
        CodeGenerator gen;
        ast.accept(gen);
        
        std::cout << "----------------------------\n";
        
        for(auto line : gen.output)
        {
            std::cout << line << "\n";
        }
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
