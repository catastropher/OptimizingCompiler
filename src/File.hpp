#pragma once

#include <string>
#include <vector>

std::string readFileContents(std::string fileName);
void writeFileContents(std::string fileName, std::vector<std::string>& lines);

