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

void writeFileContents(std::string fileName, std::vector<std::string>& lines)
{
    FILE* file = fopen(fileName.c_str(), "wb");
    
    if(!file)
        throw "Failed to open file for writing: " + fileName;
    
    for(int i = 0; i < lines.size(); ++i)
    {
        fwrite(&lines[i][0], lines[i].size(), 1, file);
        fputc('\n', file);
    }
}

