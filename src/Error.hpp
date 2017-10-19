#pragma once

#include <string>
#include <iostream>

struct CompileError
{
    CompileError(std::string message_, std::string phase_, int line_, int col_)
        : message(message_), phase(phase_), line(line_), col(col_) { }
    
    void print(std::string& fileContents)
    {
        std::cerr << "Error on line " << line << ", col " << col << ": " << message << std::endl;
        printErrorLine(fileContents);
        std::cerr << phase << " failed" << std::endl;
    }
    
    void printErrorLine(std::string& fileContents)
    {
        const char* lineStart = &fileContents[0];
        
        int curLine = 1;
        int curCol = 1;
        
        while(curLine != line)
        {
            ++curCol;
            
            if(*lineStart == '\n')
            {
                ++curLine;
                curCol = 1;
            }
            
            ++lineStart;
        }
        
        while(*lineStart != '\n' && *lineStart != '\0')
            std::cerr << *lineStart++;
        
        std::cerr << std::endl;
        
        for(int i = 1; i < col; ++i)
            std::cerr << " ";
        
        std::cerr << "^\n";
    }
    
    std::string message;
    std::string phase;
    int line;
    int col;
};

