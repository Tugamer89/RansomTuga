#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include "..\commons\common.h"

std::string openfilename();
bool fileExists(const std::string& name);
std::vector<std::string> split(const std::string& s, char delimiter);

#ifndef HEADERS_H
#define HEADERS_H
extern std::string logo;
#endif
