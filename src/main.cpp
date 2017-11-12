#include <iostream>

#include "Lexer.hpp"
#include "File.hpp"
#include "Ast.hpp"
#include "Parser.hpp"
#include "CodeGenerator.hpp"
#include "Error.hpp"
#include "Optimizer.hpp"

void compileSource(std::string inputFile, std::string outputFile)
{
    std::string input;
    
    try
    {
        input = readFileContents(inputFile);
            
        Lexer lexer(input);
        std::vector<Token>& tokens = lexer.lexTokens();
        
        Parser parser(tokens);
        Ast& ast = parser.parse();
        
        ast.defaultInitializeVars();
        ast.splitIntoBasicBlocks();
        
        Optimizer optimizer(ast.getBody(), ast);
        optimizer.optimize();
        
        CodeGenerator gen;
        gen.genCode(ast);
        
        for(auto line : gen.output)
        {
            std::cout << line << "\n";
        }
        
        std::cout << "------------------\n";
        std::cout << "Compilation successful (written to " << outputFile << ")\n"; 
        
        writeFileContents(outputFile, gen.output);
    }
    catch(CompileError& err)
    {
        err.print(input);
    }
}

int main(int argc, char* argv[])
{
    printf("Here in %s\n", __FUNCTION__);
    
    if(argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " [inputFile] [outputFile]" << std::endl;
        return -1;
    }
    
    try
    {
        compileSource(argv[1], argv[2]);
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
