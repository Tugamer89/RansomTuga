#pragma once
#include "..\commons\common.h"
#include <atlstr.h>

std::string Exec(const char* cmd);
bool FileExists(const std::string& name);
std::vector<std::string> Split(const std::string& s, const char& delimiter);
std::vector<std::vector<std::string>> VectorSplitter(const std::vector<std::string>& baseVector, int parts);
void DeleteMe(const std::string& myPath);
void DecryptFiles(const std::vector<std::string>& files, const std::string& key, const std::string& iv);
bool CheckKey(const std::string& key, const std::string& iv);
void RestoreWallpaper();
std::string GetUsername();
void GetAllFiles(const std::string& username);
std::vector<std::string> GetFiles(const std::string& mainDir);
