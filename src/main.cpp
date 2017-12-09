#include <iostream>
#include <cstring>

#include "Lexer.hpp"
#include "File.hpp"
#include "Ast.hpp"
#include "Parser.hpp"
#include "CodeGenerator.hpp"
#include "Error.hpp"
#include "Optimizer.hpp"

void compileSource(std::string inputFile, std::string outputFile, bool enableOptimizations, bool printResult)
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
        
        if(enableOptimizations)
        {
            Optimizer optimizer(ast.getBody(), ast);
            optimizer.optimize();
        }
        
        CodeGenerator gen;
        gen.genCode(ast);
        
        writeFileContents(outputFile, gen.output);
        
        if(printResult)
        {
            for(auto line : gen.output)
            {
                std::cout << line << "\n";
            }
        }
        
        std::cout << "\n";
        std::cout << "Compilation successful (written to " << outputFile << ")\n"; 
    }
    catch(CompileError& err)
    {
        err.print(input);
        throw;
    }
}

int main(int argc, char* argv[])
{
    bool enableOptimizations = true;
    bool printResult = false;
    
    if(argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " [inputFile] [outputFile]" << std::endl;
        return -1;
    }
    
    for(int i = 1; i < argc; ++i)
    {
        if(strcmp(argv[i], "--noopt") == 0)
            enableOptimizations = false;
        else if(strcmp(argv[i], "--print") == 0)
            printResult = true;
    }
    
    try
    {
        compileSource(argv[1], argv[2], enableOptimizations, printResult);
    }
    catch(const char* str)
    {
        std::cerr << str << std::endl;
        return -1;
    }
    catch(std::string str)
    {
        std::cerr << str << std::endl;
        return -1;
    }
    catch(...)
    {
        return -1;
    }
    
	return 0;
}
