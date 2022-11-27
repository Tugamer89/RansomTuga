#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "..\commons\common.h"

std::string OpenFilename();
bool FileExists(const std::string& name);
std::vector<std::string> Split(const std::string& s,const char& delimiter);
