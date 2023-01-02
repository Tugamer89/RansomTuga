#pragma once
#include "..\commons\common.h"
#include <sstream>

std::string OpenFilename();
bool FileExists(const std::string& name);
std::vector<std::string> Split(const std::string& s,const char& delimiter);
void CopyToClipboard(const std::string& s);
