#include <cstdio>

#include "File.hpp"

std::string readFileContents(std::string fileName)
{
    std::string contents;
    FILE* file = fopen(fileName.c_str(), "rb");
    
    if(!file)
        throw "Failed to open file: " + fileName;
    
    int c;
    while((c = fgetc(file)) != EOF)
        contents += (char)c;
    
    return contents;
}
