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
        gen.genCode(ast);
        
        std::cout << "----------------------------\n";
        
        for(auto line : gen.output)
        {
            std::cout << line << "\n";
        }
        
        writeFileContents("out.c", gen.output);
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
