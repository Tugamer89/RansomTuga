#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "..\commons\common.h"

std::string openfilename();
bool fileExists(const std::string& name);
std::vector<std::string> split(const std::string& s, char delimiter);
